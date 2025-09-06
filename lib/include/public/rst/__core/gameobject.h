#ifndef RST_GAMEOBJECT_H
#define RST_GAMEOBJECT_H

#include <rst/pch.h>

#include <rst/meta/type_hash.h>
#include <rst/data_type/deleter.h>
#include <rst/data_type/optional_ref.h>
#include <rst/__core/component.h>
#include <rst/__core/deletable.h>
#include <rst/__core/gameobject.h>
#include <rst/__core/transform.h>
#include <rst/__event/multicast_delegate.h>


//TODO: make component get const correct
//TODO: deletion should automatically populate the delete list
namespace rst
{
    class scene;
    class component;
}


namespace rst
{
    class gameobject final : public deletable
    {
    public:
        multicast_delegate<> on_deletion{};

        explicit gameobject( scene& scene );
        ~gameobject( ) noexcept override;

        gameobject( gameobject const& other )                        = delete;
        gameobject( gameobject&& other ) noexcept                    = delete;
        auto operator=( gameobject const& other ) -> gameobject&     = delete;
        auto operator=( gameobject&& other ) noexcept -> gameobject& = delete;

        auto fixed_tick( ) -> void;
        auto tick( ) -> void;
        auto render( ) const -> void;

        auto cleanup( ) -> void;

        [[nodiscard]] auto owning_scene( ) -> scene&;
        [[nodiscard]] auto owning_scene( ) const -> scene const&;

        auto set_tag( earmark tag ) -> void;
        [[nodiscard]] auto tag( ) const -> earmark;

        auto set_visibility( bool visible ) -> void;

        auto set_parent( gameobject* parent, bool keep_world_position = true ) -> void;

        [[nodiscard]] auto world_transform( ) -> transform const&;
        [[nodiscard]] auto local_transform( ) const -> transform const&;
        auto set_world_transform( transform const& transform ) -> void;
        auto set_local_transform( transform const& transform ) -> void;
        auto set_local_transform( transform&& transform ) -> void;

        auto transform_operator( ) -> transform_operator;

        template <typename TComponent, typename... TArgs>
        auto add_component( TArgs&&... args ) -> TComponent&;

        template <typename TComponent>
        [[nodiscard]] auto find_component( ) const -> OptionalRef<TComponent>;

        template <typename TComponent>
        [[nodiscard]] auto find_components( ) const -> OptionalRef<TComponent>;

        auto remove_component( component& component ) -> void;

        [[nodiscard]] auto create_child( ) -> gameobject&;
        [[nodiscard]] auto children( ) -> std::vector<gameobject*>&;
        [[nodiscard]] auto children( ) const -> std::vector<gameobject*> const&;
        auto collect_children( std::vector<gameobject*>& children ) const -> void;

        auto mark_for_deletion( ) -> void override;

    private:
        earmark tag_{ earmark::null };
        bool visible_{ true };

        scene& scene_ref_;
        gameobject* parent_ptr_{ nullptr };

        transform local_transform_{};
        transform world_transform_{};
        bool transform_dirty_{ false };

        std::multimap<meta::hash::hash_type, std::unique_ptr<component>> components_{};
        deleter<component> deleter_{};

        std::vector<gameobject*> children_{};

        [[nodiscard]] auto is_child( gameobject const* gameobject ) const -> bool;
        auto add_child( gameobject* gameobject ) -> void;
        auto remove_child( gameobject* gameobject ) -> void;
        [[nodiscard]] auto has_children( ) const -> bool;

        auto set_transform_dirty( ) -> void;

        auto update_world_transform( ) -> void;
    };


    template <typename TComponent, typename... TArgs>
    auto gameobject::add_component( TArgs&&... args ) -> TComponent&
    {
        // initializing component with GameObjectView and arguments' perfect forwarding.
        // we insert the element in the multimap with its compile-time type hash as the key.
        auto component = std::make_unique<TComponent>( *this, std::forward<TArgs>( args )... );
        auto it        = components_.insert( std::make_pair( meta::hash::type_hash<TComponent>( ), std::move( component ) ) );
        return static_cast<TComponent&>( *it->second );
    }


    template <typename TComponent>
    [[nodiscard]] auto gameobject::find_component( ) const -> OptionalRef<TComponent>
    {
        auto it = components_.find( meta::hash::type_hash<TComponent>( ) );
        if ( it == components_.end( ) )
        {
            return nullptr;
        }
        return static_cast<TComponent*>( it->second.get( ) );
    }


    template <typename TComponent>
    [[nodiscard]] auto gameobject::find_components( ) const -> OptionalRef<TComponent>
    {
        throw std::runtime_error( "Not implemented!" );
    }
}


#endif //!RST_GAMEOBJECT_H
