#include <framework/component/physics/ColliderComponent.h>
#include <singleton/GameInstance.h>


namespace engine
{
    // +---------------------------+
    // | COLLIDER COMPONENT        |
    // +---------------------------+
    ColliderComponent::ColliderComponent( owner_t& owner, const glm::vec2 position )
        : Component{ owner }
    {
        s_colliders_.emplace_back( this );
        offset_to( position );
    }


    ColliderComponent::~ColliderComponent( )
    {
        std::erase( s_colliders_, this );
    }


    void ColliderComponent::late_tick( )
    {
        // TODO: This should be done in a better way
        // Check for overlaps with colliders that are not in the overlapping set or persisting overlaps
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


    void ColliderComponent::offset_by( const glm::vec2 offset )
    {
        offset_ += offset;
    }


    void ColliderComponent::offset_to( const glm::vec2 offset )
    {
        offset_ = offset;
    }


    glm::vec2 ColliderComponent::get_offset( ) const
    {
        return offset_;
    }


    void ColliderComponent::set_enabled( const bool enabled )
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


    bool ColliderComponent::is_enabled( ) const
    {
        return enabled_;
    }


    glm::vec2 ColliderComponent::get_position( ) const
    {
        return offset_ + get_owner( ).get_world_transform( ).get_position( );
    }


    void ColliderComponent::clear_overlap( ColliderComponent& other )
    {
        handle_end_overlap( other );
        other.handle_end_overlap( *this );
    }


    CollisionInfo ColliderComponent::hit_test( ColliderComponent& other, const bool ignoreCache )
    {
        if ( not ignoreCache && overlapping_colliders_.contains( &other ) )
        {
            return { .status = CollisionStatus::CACHED };
        }

        if ( CollisionInfo info = hit_test_impl( other ); info.has_collided( ) )
        {
            if ( not ignoreCache )
            {
                handle_begin_overlap( other, info );

                // Invert the normal to match the other collider's perspective
                info.normal = -info.normal;
                other.handle_begin_overlap( *this, info );
            }
            return info;
        }

        return { .status = CollisionStatus::NONE };
    }


    void ColliderComponent::handle_persist_overlap( ColliderComponent& other )
    {
        if ( not hit_test( other, true ).has_collided( ) )
        {
            clear_overlap( other );
        }
    }


    void ColliderComponent::handle_begin_overlap( ColliderComponent& other, const CollisionInfo& info )
    {
        on_begin_overlap_dispatcher_.broadcast( *this, other, info );
        overlapping_colliders_.insert( &other );
    }


    void ColliderComponent::handle_end_overlap( ColliderComponent& other )
    {
        on_end_overlap_dispatcher_.broadcast( *this, other );
        overlapping_colliders_.remove( &other );
    }

}
