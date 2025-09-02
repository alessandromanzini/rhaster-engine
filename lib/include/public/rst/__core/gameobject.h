#ifndef RST_GAMEOBJECT_H
#define RST_GAMEOBJECT_H

#include <rst/pch.h>

#include <rst/framework/data_type.h>
#include <rst/meta/type_hash.h>
#include <rst/__core/component.h>
#include <rst/__core/deletable.h>
#include <rst/__core/gameobject.h>
#include <rst/__core/transform.h>
#include <rst/__event/multicast_delegate.h>


//TODO: make component get const correct
//TODO: deletion should automatically populate the delete list
namespace rst
{
    class Scene;
    class Component;
    class GameObjectView;
}


namespace rst
{
    class GameObject final : public Deletable
    {
        Dispatcher<> on_deletion_dispatcher_{};

    public:
        MulticastDelegate<> on_deletion{ on_deletion_dispatcher_ };

        explicit GameObject( Scene& scene );
        ~GameObject( ) noexcept override;

        GameObject( GameObject const& other )                        = delete;
        GameObject( GameObject&& other ) noexcept                    = delete;
        auto operator=( GameObject const& other ) -> GameObject&     = delete;
        auto operator=( GameObject&& other ) noexcept -> GameObject& = delete;

        auto fixed_tick( ) -> void;
        auto tick( ) -> void;
        auto render( ) const -> void;

        auto cleanup( ) -> void;

        [[nodiscard]] auto get_owning_scene( ) -> Scene&;
        [[nodiscard]] auto get_owning_scene( ) const -> Scene const&;

        auto set_tag( UID tag ) -> void;
        [[nodiscard]] auto get_tag( ) const -> UID;

        auto set_visibility( bool visible ) -> void;

        auto set_parent( GameObject* parent, bool keep_world_position = true ) -> void;

        [[nodiscard]] auto get_world_transform( ) -> Transform const&;
        [[nodiscard]] auto get_local_transform( ) const -> Transform const&;
        auto set_world_transform( Transform const& transform ) -> void;
        auto set_local_transform( Transform const& transform ) -> void;
        auto set_local_transform( Transform&& transform ) -> void;

        auto get_transform_operator( ) -> TransformOperator;

        template <typename TComponent, typename... TArgs>
        auto add_component( TArgs&&... args ) -> TComponent&;

        template <typename TComponent>
        [[nodiscard]] auto get_component( ) const -> OptionalRef<TComponent>;

        template <typename TComponent>
        [[nodiscard]] auto get_components( ) const -> OptionalRef<TComponent>;

        auto remove_component( Component& component ) -> void;

        [[nodiscard]] auto create_child( ) -> GameObject&;
        [[nodiscard]] auto get_children( ) -> std::vector<GameObject*>&;
        [[nodiscard]] auto get_children( ) const -> std::vector<GameObject*> const&;
        auto collect_children( std::vector<GameObject*>& children ) const -> void;

        auto mark_for_deletion( ) -> void override;

    private:
        UID tag_{ NULL_UID };
        bool visible_{ true };

        Scene& scene_ref_;
        GameObject* parent_ptr_{ nullptr };

        Transform local_transform_{};
        Transform world_transform_{};
        bool transform_dirty_{ false };

        std::multimap<uint64_t, std::unique_ptr<Component>> components_{};
        Deleter<Component> deleter_{};

        std::vector<GameObject*> children_{};

        [[nodiscard]] auto is_child( GameObject const* gameobject ) const -> bool;
        auto add_child( GameObject* gameobject ) -> void;
        auto remove_child( GameObject* gameobject ) -> void;
        [[nodiscard]] auto has_children( ) const -> bool;

        auto set_transform_dirty( ) -> void;

        auto update_world_transform( ) -> void;
    };


    template <typename TComponent, typename... TArgs>
    auto GameObject::add_component( TArgs&&... args ) -> TComponent&
    {
        // initializing component with GameObjectView and arguments' perfect forwarding.
        // we insert the element in the multimap with its compile-time type hash as the key.
        auto component = std::make_unique<TComponent>( *this, std::forward<TArgs>( args )... );
        auto it        = components_.insert( std::make_pair( meta::hash::type_hash<TComponent>( ), std::move( component ) ) );
        return static_cast<TComponent&>( *( it->second ) );
    }


    template <typename TComponent>
    [[nodiscard]] auto GameObject::get_component( ) const -> OptionalRef<TComponent>
    {
        auto it = components_.find( meta::hash::type_hash<TComponent>( ) );
        if ( it == components_.end( ) )
        {
            return nullptr;
        }
        return static_cast<TComponent*>( it->second.get( ) );
    }


    template <typename TComponent>
    [[nodiscard]] auto GameObject::get_components( ) const -> OptionalRef<TComponent>
    {
        throw std::runtime_error( "Not implemented!" );
    }
}


#endif //!RST_GAMEOBJECT_H
