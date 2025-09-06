#ifndef RST_ECS_REGISTRY_H
#define RST_ECS_REGISTRY_H

#include <rst/pch.h>

#include <rst/data_type/sparse_set.h>
#include <rst/data_type/unique_ref.h>
#include <rst/__core/ecs/entity.h>
#include <rst/__core/ecs/entity_allocator.h>
#include <rst/__core/ecs/view.h>


// todo: add concepts for components (default constructible, move assignable, etc)
// todo: add event system for entity/component lifecycle (on_construct, on_destroy, on_update, etc)
// todo: add linked_entity class that forwards remove emplace and view methods
namespace rst::ecs
{
    class registry final
    {
        // ReSharper disable once CppRedundantTemplateArguments
        template <typename TComponent>
        using component_pool_type = sparse_set<TComponent, entity_type>;

    public:
        registry( ) = default;
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
        template <typename TComponent, typename... TArgs> requires std::constructible_from<TComponent, TArgs...>
        auto emplace( entity_type const entity, TArgs&&... args ) -> TComponent&
        {
            return ensure_pool<TComponent>( ).insert( entity, std::forward<TArgs>( args )... );
        }


        /**
         * Removes the component of type TComponent from the given entity.
         * @tparam TComponent
         * @param entity
         */
        template <typename TComponent>
        auto remove( entity_type const entity ) -> void
        {
            ensure_pool<TComponent>( ).remove( entity );
        }


        /**
         * @tparam TComponents
         * @param entity
         * @return A reference to the component of type TComponent associated with the given entity.
         */
        template <typename... TComponents>
        [[nodiscard]] auto view( entity_type const entity ) -> view<TComponents...>
        {
            return ecs::view<TComponents...>{ *this, entity };
        }


        /**
         * @tparam TComponent
         * @param entity
         * @return True if the entity has the component, false otherwise.
         */
        template <typename TComponent>
        [[nodiscard]] auto has( entity_type const entity ) const -> bool
        {
            meta::hash::hash_type const type_hash = meta::hash::type_hash<TComponent>( );

            auto const it = pools_.find( type_hash );
            return it != pools_.end( ) && it->second->has( entity );
        }

    private:
        std::unordered_map<meta::hash::hash_type, unique_ref<base_sparse_set<entity_type>>> pools_{};
        entity_allocator entity_alloc_{};


        template <typename TComponent>
        [[nodiscard]] auto ensure_pool( ) -> component_pool_type<TComponent>&
        {
            meta::hash::hash_type const type_hash = meta::hash::type_hash<TComponent>( );
            auto [it, inserted] = pools_.try_emplace( type_hash, ref::make_unique<component_pool_type<TComponent>>( ) );
            return static_cast<component_pool_type<TComponent>&>( it->second.value( ) );
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


    // todo: remove this code
    // template <typename TComponent> auto registry::get( entity_type const entity ) -> TComponent&
    // {
    //     return assure_pool<TComponent>( ).get( entity );
    // }
    //
    //
    // template <typename TComponent>
    // auto registry::get( entity_type const entity ) const -> std::expected<TComponent const&, std::runtime_error>
    // {
    //     meta::hash::hash_type const type_hash = meta::hash::type_hash<TComponent>( );
    //     if ( auto const it = pools_.find( type_hash ); it != pools_.end( ) )
    //     {
    //         return static_cast<component_pool_type<TComponent> const&>( *it->second ).get( entity );
    //     }
    //     return std::runtime_error{ "registry::get: component type not found" };
    // }
}


#endif //!RST_ECS_REGISTRY_H
