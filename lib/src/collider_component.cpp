#include <rst/temp/framework/component/physics/collider_component.h>

#include <rst/temp/singleton/game_instance.h>


namespace rst
{
    // +---------------------------+
    // | COLLIDER COMPONENT        |
    // +---------------------------+
    ColliderComponent::ColliderComponent( owner_t& owner, glm::vec2 const position )
        : Component{ owner }
    {
        s_colliders_.emplace_back( this );
        offset_to( position );
    }


    ColliderComponent::~ColliderComponent( )
    {
        std::erase( s_colliders_, this );
    }


    auto ColliderComponent::late_tick( ) -> void
    {
        // TODO: This should be done in a better way
        // check for overlaps with colliders that are not in the overlapping set or persisting overlaps
        if ( s_colliders_.size( ) < 2u )
        {
            return;
        }

        for ( size_t i{}; i < s_colliders_.size( ) - 1u; ++i )
        {
            for ( size_t j{ i + 1u }; j < s_colliders_.size( ); ++j )
            {
                auto& colliderA{ *s_colliders_[i] };
                auto& colliderB{ *s_colliders_[j] };

                if ( not colliderA.is_enabled( ) or not colliderB.is_enabled( ) )
                {
                    continue;
                }

                if ( colliderA.overlapping_colliders_.contains( &colliderB ) )
                {
                    colliderA.handle_persist_overlap( colliderB );
                }
                else
                {
                    colliderA.hit_test( colliderB );
                }
            }
        }
    }


    auto ColliderComponent::offset_by( glm::vec2 const offset ) -> void
    {
        offset_ += offset;
    }


    auto ColliderComponent::offset_to( glm::vec2 const offset ) -> void
    {
        offset_ = offset;
    }


    auto ColliderComponent::get_offset( ) const -> glm::vec2
    {
        return offset_;
    }


    auto ColliderComponent::set_enabled( bool const enabled ) -> void
    {
        enabled_ = enabled;
        if ( not enabled )
        {
            for ( auto* collider : overlapping_colliders_.data( ) )
            {
                clear_overlap( *collider );
            }
        }
    }


    auto ColliderComponent::is_enabled( ) const -> bool
    {
        return enabled_;
    }


    auto ColliderComponent::get_position( ) const -> glm::vec2
    {
        return offset_ + get_owner( ).get_world_transform( ).get_position( );
    }


    auto ColliderComponent::clear_overlap( ColliderComponent& other ) -> void
    {
        handle_end_overlap( other );
        other.handle_end_overlap( *this );
    }


    auto ColliderComponent::hit_test( ColliderComponent& other, bool const ignore_cache ) -> CollisionInfo
    {
        if ( not ignore_cache && overlapping_colliders_.contains( &other ) )
        {
            return { .status = CollisionStatus::CACHED };
        }

        if ( CollisionInfo info = hit_test_impl( other ); info.has_collided( ) )
        {
            if ( not ignore_cache )
            {
                handle_begin_overlap( other, info );

                // invert the normal to match the other collider's perspective
                info.normal = -info.normal;
                other.handle_begin_overlap( *this, info );
            }
            return info;
        }

        return { .status = CollisionStatus::NONE };
    }


    auto ColliderComponent::handle_persist_overlap( ColliderComponent& other ) -> void
    {
        if ( not hit_test( other, true ).has_collided( ) )
        {
            clear_overlap( other );
        }
    }


    auto ColliderComponent::handle_begin_overlap( ColliderComponent& other, CollisionInfo const& info ) -> void
    {
        on_begin_overlap_dispatcher_.broadcast( *this, other, info );
        overlapping_colliders_.insert( &other );
    }


    auto ColliderComponent::handle_end_overlap( ColliderComponent& other ) -> void
    {
        on_end_overlap_dispatcher_.broadcast( *this, other );
        overlapping_colliders_.remove( &other );
    }

}
