#ifndef RST_ECS_REGISTRY_H
#define RST_ECS_REGISTRY_H

#include <rst/pch.h>

#include <rst/data_type/sparse_set.h>
#include <rst/data_type/unique_ref.h>
#include <rst/meta/reference.h>
#include <rst/__core/ecs/component_pool.h>
#include <rst/__core/ecs/entity.h>
#include <rst/__core/ecs/entity_allocator.h>
#include <rst/__core/ecs/view.h>


// todo: add concepts for components (default constructible, move assignable, etc)
// todo: add event system for entity/component lifecycle (on_construct, on_destroy, on_update, etc)
namespace rst::ecs
{
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
         * @return A reference to the entity allocator.
         */
        [[nodiscard]] auto entity_alloc( ) -> entity_allocator&
        {
            return entity_alloc_;
        }


        /**
         * @tparam TComponent
         * @tparam TArgs
         * @param entity
         * @param args
         * @return A component of type TComponent for the given entity, constructing it with the provided arguments.
         */
        template <meta::non_reference TComponent, typename... TArgs> requires std::constructible_from<TComponent, TArgs...>
        auto emplace( entity_type const entity, TArgs&&... args ) -> TComponent&
        {
            return ensure_pool<TComponent>( ).insert_or_replace( entity, std::forward<TArgs>( args )... );
        }


        /**
         * Removes the component of type TComponent from the given entity.
         * @tparam TComponent
         * @param entity
         */
        template <meta::non_reference TComponent>
        auto remove( entity_type const entity ) -> void
        {
            ensure_pool<TComponent>( ).remove( entity );
        }


        /**
         * @tparam TComponents
         * @param entity
         * @return True if the entity has the components, false otherwise.
         */
        template <meta::non_reference... TComponents>
        [[nodiscard]] auto has( entity_type const entity ) const -> bool
        {
            return ( has_impl<TComponents>( entity ) && ... );
        }


        /**
         * @tparam TComponents
         * @return A view to the TComponents.
         */
        template <meta::non_reference... TComponents>
        [[nodiscard]] auto view( ) -> view<TComponents...>
        {
            return ecs::view<TComponents...>{ { ensure_pool<TComponents>( )... } };
        }

    private:
        std::unordered_map<meta::hash::hash_type, unique_ref<internal::base_component_pool_type>> pools_{};
        entity_allocator entity_alloc_{};


        template <meta::non_reference TComponent>
        [[nodiscard]] auto ensure_pool( ) -> internal::component_pool_type<TComponent>&
        {
            meta::hash::hash_type const type_hash = meta::hash::type_hash_v<TComponent>;
            auto [it, inserted] = pools_.try_emplace( type_hash, ref::make_unique<internal::component_pool_type<TComponent>>( ) );
            return static_cast<internal::component_pool_type<TComponent>&>( it->second.value( ) );
        }


        template <meta::non_reference TComponent>
        [[nodiscard]] auto has_impl( entity_type const entity ) const -> bool
        {
            meta::hash::hash_type const type_hash = meta::hash::type_hash_v<TComponent>;

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
