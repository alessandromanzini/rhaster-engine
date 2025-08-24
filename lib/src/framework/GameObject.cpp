#include <framework/GameObject.h>

// +--------------------------------+
// | PROJECT HEADERS				|
// +--------------------------------+
#include <framework/GameObjectView.h>
#include <framework/Scene.h>
#include <framework/component/Component.h>
#include <singleton/Renderer.h>
#include <singleton/ResourceManager.h>


namespace engine
{
    // GameObject::GameObject( )
    //     : view_ptr_( new GameObjectView{ *this } ) { }


    GameObject::GameObject( Scene& scene )
        : scene_ref_{ scene } { }


    GameObject::~GameObject( ) noexcept
    {
        // Notify hierarchy that this object is being deleted
        if ( parent_ptr_ != nullptr )
        {
            parent_ptr_->remove_child( this );
        }
        for ( size_t i{}; i < children_.size( ); ++i )
        {
            children_[i]->set_parent( nullptr, false );
        }

        // Broadcast that owner is being deleted
        on_deletion_dispatcher_.broadcast( );
    }


    void GameObject::fixed_tick( )
    {
        for ( const auto& [key, component] : components_ )
        {
            component->fixed_tick( );
        }
    }


    void GameObject::tick( )
    {
        for ( const auto& [key, component] : components_ )
        {
            component->tick( );
        }
    }


    void GameObject::render( ) const
    {
        if ( not visible_ )
        {
            return;
        }
        for ( const auto& [key, component] : components_ )
        {
            component->render( );
        }
    }


    void GameObject::cleanup( )
    {
        if ( deleter_.is_cleanup_needed( ) )
        {
            deleter_.cleanup( components_ );
        }
    }


    Scene& GameObject::get_owning_scene( )
    {
        return scene_ref_;
    }


    const Scene& GameObject::get_owning_scene( ) const
    {
        return scene_ref_;
    }


    void GameObject::set_tag( const UID tag )
    {
        tag_ = tag;
    }


    UID GameObject::get_tag( ) const
    {
        return tag_;
    }


    void GameObject::set_visibility( const bool visible )
    {
        visible_ = visible;
        for ( const auto child : children_ )
        {
            child->set_visibility( visible );
        }
    }


    void GameObject::set_parent( GameObject* const parent, const bool keepWorldPosition /* = true */ )
    {
        // Parent validation
        if ( is_child( parent ) || parent == this || parent == parent_ptr_ )
        {
            return;
        }

        if ( parent == nullptr )
        {
            // If becoming root object, local transform becomes world space transform
            set_local_transform( get_world_transform( ) );
        }
        else
        {
            // If parent is provided, we need to adjust the local transform to keep the world position
            // and set it dirty to recalculate the world transform.
            if ( keepWorldPosition )
            {
                // TODO: test if rotation is canceled
                const auto translation = get_world_transform( ).get_position( ) - parent->get_world_transform( ).get_position( );
                set_local_transform( Transform::from_translation( translation ) );
            }
            set_transform_dirty( );
        }

        // Re-parenting logic
        if ( parent_ptr_ != nullptr ) { parent_ptr_->remove_child( this ); }
        if ( parent != nullptr ) { parent->add_child( this ); }
        parent_ptr_ = parent;
    }


    const Transform& GameObject::get_world_transform( )
    {
        update_world_transform( );
        return world_transform_;
    }


    const Transform& GameObject::get_local_transform( ) const
    {
        return local_transform_;
    }


    void GameObject::set_world_transform( const Transform& transform )
    {
        if ( parent_ptr_ == nullptr )
        {
            set_local_transform( transform );
        }
        else
        {
            const glm::mat3x3 parentInverse{ inverse( parent_ptr_->get_world_transform( ).get_matrix( ) ) };
            set_local_transform( parentInverse * transform.get_matrix( ) );
        }
    }


    void GameObject::set_local_transform( const Transform& transform )
    {
        local_transform_ = transform;
        set_transform_dirty( );
    }


    void GameObject::set_local_transform( Transform&& transform )
    {
        local_transform_ = transform;
        set_transform_dirty( );
    }


    TransformOperator GameObject::get_transform_operator( )
    {
        return TransformOperator{ *this };
    }


    void GameObject::remove_component( Component& component )
    {
        deleter_.mark_element_for_deletion( &component );
    }


    GameObject& GameObject::create_child( )
    {
        auto& child = scene_ref_.create_object( );
        child.set_parent( this, false );
        return child;
    }


    std::vector<GameObject*>& GameObject::get_children( )
    {
        return children_;
    }


    const std::vector<GameObject*>& GameObject::get_children( ) const
    {
        return children_;
    }


    void GameObject::collect_children( std::vector<GameObject*>& children ) const
    {
        for ( auto* child : children_ )
        {
            children.push_back( child );
            child->collect_children( children );
        }
    }


    void GameObject::mark_for_deletion( )
    {
        scene_ref_.remove( *this );
    }


    bool GameObject::is_child( const GameObject* const gameObject ) const
    {
        return std::ranges::any_of( children_,
                                    [gameObject]( const auto* child )
                                        {
                                            return child == gameObject || child->is_child( gameObject );
                                        } );
    }


    void GameObject::add_child( GameObject* gameObject )
    {
        children_.push_back( gameObject );
    }


    void GameObject::remove_child( GameObject* gameObject )
    {
        // todo: check this
        if (children_.size()) 
        {
            std::erase( children_, gameObject );
        }
    }


    bool GameObject::has_children( ) const
    {
        return !children_.empty( );
    }


    void GameObject::set_transform_dirty( )
    {
        transform_dirty_ = true;

        // TODO: optimize this
        for ( const auto child : children_ )
        {
            child->set_transform_dirty( );
        }
    }


    void GameObject::update_world_transform( )
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
