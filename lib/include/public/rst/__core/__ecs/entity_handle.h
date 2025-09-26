#ifndef RST_ECS_ENTITY_HANDLE_H
#define RST_ECS_ENTITY_HANDLE_H

#include <rst/pch.h>

#include <rst/__core/__ecs/entity.h>
#include <rst/__core/__ecs/registry.h>


namespace rst::ecs
{
    class entity_handle final
    {
    public:
        entity_handle( registry& reg, entity_type const entity ) noexcept
            : registry_ref_{ reg }
            , entity_{ entity } { }


        ~entity_handle( ) noexcept = default;

        entity_handle( entity_handle const& )                        = delete;
        entity_handle( entity_handle&& ) noexcept                    = delete;
        auto operator=( entity_handle const& ) -> entity_handle&     = delete;
        auto operator=( entity_handle&& ) noexcept -> entity_handle& = delete;


        /**
         * @tparam TComponent
         * @tparam TArgs
         * @param args
         * @return A component of type TComponent for the given entity, constructing it with the provided arguments.
         */
        template <typename TComponent, typename... TArgs> requires std::constructible_from<TComponent, TArgs...>
        auto emplace( TArgs&&... args ) -> TComponent&
        {
            return registry_ref_.emplace<TComponent>( entity_, std::forward<TArgs>( args )... );
        }


        /**
         * Removes the component of type TComponent from the given entity.
         * @tparam TComponent
         * @param entity
         */
        template <typename TComponent>
        auto remove( entity_type const entity ) const -> void { registry_ref_.remove<TComponent>( entity ); }


        /**
         * @tparam TComponents
         * @return A reference to the component of type TComponent associated with the given entity.
         */
        template <typename... TComponents>
        [[nodiscard]] auto view( ) const -> view<TComponents...> { return registry_ref_.view<TComponents...>( entity_ ); }


        /**
        * @tparam TComponents
        * @return True if the entity has the components, false otherwise.
        */
        template <typename... TComponents>
        [[nodiscard]] auto has( ) const -> bool { return registry_ref_.has<TComponents...>( entity_ ); }


        /**
         * @return The underlying entity_type.
         */
        auto get( ) const noexcept -> entity_type { return entity_; }

    private:
        registry& registry_ref_;
        entity_type const entity_;
    };
}


#endif //!RST_ECS_ENTITY_HANDLE_H
