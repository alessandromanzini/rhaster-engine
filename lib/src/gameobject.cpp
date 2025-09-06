#include <rst/__core/gameobject.h>

#include <rst/temp/singleton/renderer.h>

#include <rst/__core/component.h>
#include <rst/__core/scene.h>


namespace rst
{
    gameobject::gameobject( scene& scene ) : scene_ref_{ scene } { }


    gameobject::~gameobject( ) noexcept
    {
        // notify hierarchy that this object is being deleted
        if ( parent_ptr_ != nullptr )
        {
            parent_ptr_->remove_child( this );
        }
        for ( gameobject* child : children_ )
        {
            child->set_parent( nullptr, false );
        }

        // broadcast that owner is being deleted
        on_deletion.broadcast( );
    }


    auto gameobject::fixed_tick( ) -> void
    {
        for ( auto const& component : components_ | std::views::values )
        {
            component->fixed_tick( );
        }
    }


    auto gameobject::tick( ) -> void
    {
        for ( auto const& component : components_ | std::views::values )
        {
            component->tick( );
        }
    }


    auto gameobject::render( ) const -> void
    {
        if ( not visible_ )
        {
            return;
        }
        for ( auto const& component : components_ | std::views::values )
        {
            component->render( );
        }
    }


    auto gameobject::cleanup( ) -> void
    {
        if ( deleter_.is_cleanup_needed( ) )
        {
            deleter_.cleanup( components_ );
        }
    }


    auto gameobject::owning_scene( ) -> scene&
    {
        return scene_ref_;
    }


    auto gameobject::owning_scene( ) const -> scene const&
    {
        return scene_ref_;
    }


    auto gameobject::set_tag( earmark const tag ) -> void
    {
        tag_ = tag;
    }


    auto gameobject::tag( ) const -> earmark
    {
        return tag_;
    }


    auto gameobject::set_visibility( bool const visible ) -> void
    {
        visible_ = visible;
        for ( auto const child : children_ )
        {
            child->set_visibility( visible );
        }
    }


    auto gameobject::set_parent( gameobject* const parent, bool const keep_world_position /* = true */ ) -> void
    {
        // parent validation
        if ( is_child( parent ) || parent == this || parent == parent_ptr_ )
        {
            return;
        }

        if ( parent == nullptr )
        {
            // if becoming root object, local transform becomes world space transform
            set_local_transform( world_transform( ) );
        }
        else
        {
            // if parent is provided, we need to adjust the local transform to keep the world position and set it dirty to
            // recalculate the world transform.
            if ( keep_world_position )
            {
                // TODO: test if rotation is canceled
                auto const translation = world_transform( ).position( ) - parent->world_transform( ).position( );
                set_local_transform( transform::from_translation( translation ) );
            }
            set_transform_dirty( );
        }

        // parenting logic
        if ( parent_ptr_ != nullptr ) { parent_ptr_->remove_child( this ); }
        if ( parent != nullptr ) { parent->add_child( this ); }
        parent_ptr_ = parent;
    }


    auto gameobject::world_transform( ) -> transform const&
    {
        update_world_transform( );
        return world_transform_;
    }


    auto gameobject::local_transform( ) const -> transform const&
    {
        return local_transform_;
    }


    auto gameobject::set_world_transform( transform const& transform ) -> void
    {
        if ( parent_ptr_ == nullptr )
        {
            set_local_transform( transform );
        }
        else
        {
            glm::mat3x3 const parent_inverse{ glm::inverse( parent_ptr_->world_transform( ).matrix( ) ) };
            set_local_transform( parent_inverse * transform.matrix( ) );
        }
    }


    auto gameobject::set_local_transform( transform const& transform ) -> void
    {
        local_transform_ = transform;
        set_transform_dirty( );
    }


    auto gameobject::set_local_transform( transform&& transform ) -> void
    {
        local_transform_ = transform;
        set_transform_dirty( );
    }


    auto gameobject::transform_operator( ) -> rst::transform_operator
    {
        return rst::transform_operator{ *this };
    }


    auto gameobject::remove_component( component& component ) -> void
    {
        deleter_.mark_element_for_deletion( &component );
    }


    auto gameobject::create_child( ) -> gameobject&
    {
        auto& child = scene_ref_.create_object( );
        child.set_parent( this, false );
        return child;
    }


    auto gameobject::children( ) -> std::vector<gameobject*>&
    {
        return children_;
    }


    auto gameobject::children( ) const -> std::vector<gameobject*> const&
    {
        return children_;
    }


    auto gameobject::collect_children( std::vector<gameobject*>& children ) const -> void
    {
        for ( auto* child : children_ )
        {
            children.push_back( child );
            child->collect_children( children );
        }
    }


    auto gameobject::mark_for_deletion( ) -> void
    {
        scene_ref_.remove( *this );
    }


    auto gameobject::is_child( gameobject const* const gameobject ) const -> bool
    {
        return std::ranges::any_of(
            children_,
            [gameobject]( auto const* child )
            {
                return child == gameobject || child->is_child( gameobject );
            } );
    }


    auto gameobject::add_child( gameobject* gameobject ) -> void
    {
        children_.push_back( gameobject );
    }


    auto gameobject::remove_child( gameobject* gameobject ) -> void
    {
        // todo: check this
        if ( children_.size( ) )
        {
            std::erase( children_, gameobject );
        }
    }


    auto gameobject::has_children( ) const -> bool
    {
        return !children_.empty( );
    }


    auto gameobject::set_transform_dirty( ) -> void
    {
        transform_dirty_ = true;

        // TODO: optimize this
        for ( auto const child : children_ )
        {
            child->set_transform_dirty( );
        }
    }


    auto gameobject::update_world_transform( ) -> void
    {
        if ( transform_dirty_ )
        {
            if ( parent_ptr_ == nullptr )
            {
                world_transform_ = local_transform( );
            }
            else
            {
                world_transform_ = parent_ptr_->world_transform( ) * local_transform_;
            }
            transform_dirty_ = false;
        }
    }
}
