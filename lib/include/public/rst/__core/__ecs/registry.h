#ifndef RST_ECS_REGISTRY_H
#define RST_ECS_REGISTRY_H

#include <rst/pch.h>

#include <rst/data_type/sparse_set.h>
#include <rst/data_type/unique_ref.h>
#include <rst/__core/__ecs/component_constraints.h>
#include <rst/__core/__ecs/entity.h>
#include <rst/__core/__ecs/entity_allocator.h>
#include <rst/__core/__ecs/registry_pool.h>
#include <rst/__core/__ecs/view.h>


// todo: add concepts for components (default constructible, move assignable, etc), for view creation check components existence
// todo: add event system for entity/component lifecycle (on_construct, on_destroy, on_update, etc)
namespace rst::ecs
{
    /**
     * @brief Central coordinator for the Entity Component System (ECS) managing entities, components, and their relationships.
     *
     * The registry is the main interface for ECS operations, providing entity management, component storage,
     * and efficient querying capabilities. It automatically manages component pools using sparse sets for
     * optimal performance in entity-component systems.
     *
     * Key features:
     * - Entity lifecycle management with automatic cleanup
     * - Type-safe component operations with compile-time checks
     * - Efficient component storage using sparse sets
     * - Automatic memory management with RAII principles
     * - Event-driven entity destruction for consistency
     * - Template-based API for zero-cost abstractions
     *
     * @code
     * // basic registry usage
     * rst::ecs::registry registry{};
     * 
     * // create entities using the entity allocator
     * auto entity1 = registry.entity_alloc().create();
     * auto entity2 = registry.entity_alloc().create();
     * 
     * // add components to entities
     * struct position{ float x, y; };
     * struct velocity{ float dx, dy; };
     * 
     * auto& pos1 = registry.emplace<position>(entity1, 10.0f, 20.0f);
     * auto& vel1 = registry.emplace<velocity>(entity1, 1.0f, 0.5f);
     * 
     * auto& pos2 = registry.emplace<position>(entity2, 5.0f, 15.0f);
     * // entity2 doesn't have velocity component
     * 
     * // check for component existence
     * if (registry.has<position>(entity1)) {
     *     // entity1 has position component
     * }
     * 
     * if (registry.has<position, velocity>(entity1)) {
     *     // entity1 has both position and velocity
     * }
     * 
     * // create views for system processing
     * auto movement_view = registry.view<position, velocity>();
     * 
     * // remove components
     * registry.remove<velocity>(entity1);
     * 
     * // entity destruction automatically removes all components
     * registry.entity_alloc().destroy(entity1);
     * @endcode
     */
    class registry final
    {
    public:
        registry( )
        {
            entity_alloc_.on_destruction.bind( this, &registry::destroy_entity );
            entity_alloc_.on_clear.bind( this, &registry::clear_entities );
        }


        ~registry( ) noexcept = default;

        registry( registry const& )                        = delete;
        registry( registry&& ) noexcept                    = delete;
        auto operator=( registry const& ) -> registry&     = delete;
        auto operator=( registry&& ) noexcept -> registry& = delete;

        /**
         * @brief Gets a reference to the entity allocator for entity lifecycle management.
         * 
         * @complexity O(1)
         * @return Reference to the internal entity allocator.
         */
        [[nodiscard]] auto entity_alloc( ) -> entity_allocator&
        {
            return entity_alloc_;
        }


        /**
         * @brief Constructs a component of type TComponent for the given entity in-place.
         * 
         * If the entity already has a component of this type, it will be replaced.
         * The component is constructed using perfect forwarding to avoid unnecessary copies.
         *
         * @complexity O(1) amortized (may trigger pool resize)
         * @tparam TComponent Non-const, non-reference type of the component to emplace.
         * @tparam TArgs Constructor argument types (automatically deduced).
         * @param entity The entity to attach the component to.
         * @param args Constructor arguments forwarded to TComponent's constructor.
         * @return Reference to the newly created component.
         */
        template <detail::ecs_component TComponent, typename... TArgs> requires std::constructible_from<TComponent, TArgs...>
        auto emplace( entity_type const entity, TArgs&&... args ) -> TComponent&
        {
            return ensure_pool<TComponent>( ).insert_or_replace( entity, std::forward<TArgs>( args )... );
        }


        /**
         * @brief Removes the component of type TComponent from the given entity.
         * 
         * If the entity doesn't have the specified component, this operation has no effect.
         * Uses swap-and-pop removal for O(1) performance.
         *
         * @complexity O(1)
         * @tparam TComponent The component type to remove.
         * @param entity The entity to remove the component from.
         */
        template <detail::ecs_component TComponent>
        auto remove( entity_type const entity ) -> void
        {
            ensure_pool<TComponent>( ).remove( entity );
        }


        /**
         * @brief Checks if an entity has all specified component types.
         * 
         * Uses logical AND to check multiple components - returns true only if
         * the entity has ALL the specified components.
         *
         * @complexity O(k) where k is the number of component types
         * @tparam TComponents The component types to check for.
         * @param entity The entity to check.
         * @return True if the entity has all specified components, false otherwise.
         */
        template <detail::viewable_ecs_component... TComponents>
        [[nodiscard]] auto has( entity_type const entity ) const -> bool
        {
            return ( has_impl<TComponents>( entity ) && ... );
        }


        /**
         * @brief Creates a view for iterating over entities with all specified components.
         * 
         * Views provide efficient iteration over entities that possess all the required
         * component types. The view uses sparse set intersection for optimal performance.
         *
         * @complexity O(1) - view creation is lightweight, iteration cost depends on entity count.
         * @tparam TComponents The component types that entities must have.
         * @return A view object for iterating over matching entities.
         */
        template <detail::viewable_ecs_component... TComponents>
        [[nodiscard]] auto view( ) -> view<TComponents...>
        {
            return ecs::view<TComponents...>{ ensure_pool<TComponents>( )... };
        }

    private:
        std::unordered_map<meta::hash::hash_type, unique_ref<detail::base_reg_pool_type>> pools_{};
        entity_allocator entity_alloc_{};


        /**
         * @brief Ensures that a component pool for TComponent exists, creating it if necessary. Pool creation is always non-const, to
         * allow "replace" operations.
         *
         * @note Const correctness is enforced at template constraint level, as the methods can only be called using the correct
         *
         * @tparam TComponent
         * @return The component pool for TComponent.
         */
        template <detail::viewable_ecs_component TComponent>
        [[nodiscard]] auto ensure_pool( ) -> detail::reg_pool_type<TComponent>&
        {
            meta::hash::hash_type const type_hash = meta::hash::type_hash_v<TComponent>;
            auto [it, inserted] = pools_.try_emplace( type_hash, ref::make_unique<detail::reg_pool_type<TComponent>>( ) );
            return static_cast<detail::reg_pool_type<TComponent>&>( it->second.value( ) );
        }


        template <detail::viewable_ecs_component TComponent>
        [[nodiscard]] auto has_impl( entity_type const entity ) const -> bool
        {
            meta::hash::hash_type const type_hash = meta::hash::type_hash_v<TComponent, true>;

            auto const it = pools_.find( type_hash );
            return it != pools_.end( ) && it->second->has( entity );
        }


        auto destroy_entity( entity_type const entity ) -> void
        {
            for ( auto& pool : pools_ | std::views::values )
            {
                pool->remove( entity );
            }
        }


        auto clear_entities( ) -> void
        {
            for ( auto& pool : pools_ | std::views::values )
            {
                pool->clear( );
            }
        }
    };
}


#endif //!RST_ECS_REGISTRY_H
