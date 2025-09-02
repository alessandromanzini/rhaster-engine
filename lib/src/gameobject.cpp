#include <rst/__core/gameobject.h>

#include <rst/temp/singleton/renderer.h>
#include <rst/temp/singleton/resource_manager.h>
#include <rst/__core/component.h>
#include <rst/__core/scene.h>


namespace rst
{
    // GameObject::GameObject( )
    //     : view_ptr_( new GameObjectView{ *this } ) { }


    GameObject::GameObject( Scene& scene )
        : scene_ref_{ scene } { }


    GameObject::~GameObject( ) noexcept
    {
        // notify hierarchy that this object is being deleted
        if ( parent_ptr_ != nullptr )
        {
            parent_ptr_->remove_child( this );
        }
        for ( size_t i{}; i < children_.size( ); ++i )
        {
            children_[i]->set_parent( nullptr, false );
        }

        // broadcast that owner is being deleted
        on_deletion_dispatcher_.broadcast( );
    }


    auto GameObject::fixed_tick( ) -> void
    {
        for ( auto const& [key, component] : components_ )
        {
            component->fixed_tick( );
        }
    }


    auto GameObject::tick( ) -> void
    {
        for ( auto const& [key, component] : components_ )
        {
            component->tick( );
        }
    }


    auto GameObject::render( ) const -> void
    {
        if ( not visible_ )
        {
            return;
        }
        for ( auto const& [key, component] : components_ )
        {
            component->render( );
        }
    }


    auto GameObject::cleanup( ) -> void
    {
        if ( deleter_.is_cleanup_needed( ) )
        {
            deleter_.cleanup( components_ );
        }
    }


    auto GameObject::get_owning_scene( ) -> Scene&
    {
        return scene_ref_;
    }


    auto GameObject::get_owning_scene( ) const -> Scene const&
    {
        return scene_ref_;
    }


    auto GameObject::set_tag( UID const tag ) -> void
    {
        tag_ = tag;
    }


    auto GameObject::get_tag( ) const -> UID
    {
        return tag_;
    }


    auto GameObject::set_visibility( bool const visible ) -> void
    {
        visible_ = visible;
        for ( auto const child : children_ )
        {
            child->set_visibility( visible );
        }
    }


    auto GameObject::set_parent( GameObject* const parent, bool const keep_world_position /* = true */ ) -> void
    {
        // parent validation
        if ( is_child( parent ) || parent == this || parent == parent_ptr_ )
        {
            return;
        }

        if ( parent == nullptr )
        {
            // if becoming root object, local transform becomes world space transform
            set_local_transform( get_world_transform( ) );
        }
        else
        {
            // if parent is provided, we need to adjust the local transform to keep the world position
            // and set it dirty to recalculate the world transform.
            if ( keep_world_position )
            {
                // TODO: test if rotation is canceled
                auto const translation = get_world_transform( ).get_position( ) - parent->get_world_transform( ).get_position( );
                set_local_transform( Transform::from_translation( translation ) );
            }
            set_transform_dirty( );
        }

        // re-parenting logic
        if ( parent_ptr_ != nullptr ) { parent_ptr_->remove_child( this ); }
        if ( parent != nullptr ) { parent->add_child( this ); }
        parent_ptr_ = parent;
    }


    auto GameObject::get_world_transform( ) -> Transform const&
    {
        update_world_transform( );
        return world_transform_;
    }


    auto GameObject::get_local_transform( ) const -> Transform const&
    {
        return local_transform_;
    }


    auto GameObject::set_world_transform( Transform const& transform ) -> void
    {
        if ( parent_ptr_ == nullptr )
        {
            set_local_transform( transform );
        }
        else
        {
            glm::mat3x3 const parentInverse{ inverse( parent_ptr_->get_world_transform( ).get_matrix( ) ) };
            set_local_transform( parentInverse * transform.get_matrix( ) );
        }
    }


    auto GameObject::set_local_transform( Transform const& transform ) -> void
    {
        local_transform_ = transform;
        set_transform_dirty( );
    }


    auto GameObject::set_local_transform( Transform&& transform ) -> void
    {
        local_transform_ = transform;
        set_transform_dirty( );
    }


    auto GameObject::get_transform_operator( ) -> TransformOperator
    {
        return TransformOperator{ *this };
    }


    auto GameObject::remove_component( Component& component ) -> void
    {
        deleter_.mark_element_for_deletion( &component );
    }


    auto GameObject::create_child( ) -> GameObject&
    {
        auto& child = scene_ref_.create_object( );
        child.set_parent( this, false );
        return child;
    }


    auto GameObject::get_children( ) -> std::vector<GameObject*>&
    {
        return children_;
    }


    auto GameObject::get_children( ) const -> std::vector<GameObject*> const&
    {
        return children_;
    }


    auto GameObject::collect_children( std::vector<GameObject*>& children ) const -> void
    {
        for ( auto* child : children_ )
        {
            children.push_back( child );
            child->collect_children( children );
        }
    }


    auto GameObject::mark_for_deletion( ) -> void
    {
        scene_ref_.remove( *this );
    }


    auto GameObject::is_child( GameObject const* const game_object ) const -> bool
    {
        return std::ranges::any_of( children_,
                                    [game_object]( auto const* child )
                                        {
                                            return child == game_object || child->is_child( game_object );
                                        } );
    }


    auto GameObject::add_child( GameObject* gameobject ) -> void
    {
        children_.push_back( gameobject );
    }


    auto GameObject::remove_child( GameObject* gameobject ) -> void
    {
        // todo: check this
        if (children_.size())
        {
            std::erase( children_, gameobject );
        }
    }


    auto GameObject::has_children( ) const -> bool
    {
        return !children_.empty( );
    }


    auto GameObject::set_transform_dirty( ) -> void
    {
        transform_dirty_ = true;

        // TODO: optimize this
        for ( auto const child : children_ )
        {
            child->set_transform_dirty( );
        }
    }


    auto GameObject::update_world_transform( ) -> void
    {
        if ( transform_dirty_ )
        {
            if ( parent_ptr_ == nullptr )
            {
                world_transform_ = get_local_transform( );
            }
            else
            {
                world_transform_ = parent_ptr_->get_world_transform( ) * local_transform_;
            }
            transform_dirty_ = false;
        }
    }

}
