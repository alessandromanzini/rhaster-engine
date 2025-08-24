#include <framework/GameObject.h>
#include <framework/component/physics/BoxColliderComponent.h>


namespace engine
{
    BoxColliderComponent::BoxColliderComponent( owner_t& owner, const glm::vec2 size, const glm::vec2 position )
        : ColliderComponent{ owner, position }
        , pivots_{
            glm::vec2{ 0.f, 0.f },
            glm::vec2{ size.x, 0.f },
            glm::vec2{ size.x, size.y },
            glm::vec2{ 0.f, size.y }
        } { }


    std::span<const glm::vec2> BoxColliderComponent::get_pivots( ) const
    {
        return std::span<const glm::vec2>{ pivots_ };
    }


    CollisionInfo BoxColliderComponent::hit_test_impl( ColliderComponent& other ) const
    {
        const glm::vec2 selfPosition  = get_position( );
        const glm::vec2 otherPosition = other.get_position( );

        const glm::vec2 aMin = selfPosition + pivots_[0];
        const glm::vec2 aMax = selfPosition + pivots_[2];

        const auto otherPivots = other.get_pivots( );
        const glm::vec2 bMin   = otherPosition + otherPivots[0];
        const glm::vec2 bMax   = otherPosition + otherPivots[2];

        if ( aMax.x < bMin.x || aMin.x > bMax.x || aMax.y < bMin.y || aMin.y > bMax.y )
        {
            return CollisionInfo{ .status = CollisionStatus::NONE };
        }

        // Calculate overlap in each direction
        const float overlapX = std::min( aMax.x, bMax.x ) - std::max( aMin.x, bMin.x );
        const float overlapY = std::min( aMax.y, bMax.y ) - std::max( aMin.y, bMin.y );

        glm::vec2 normal{};
        float depth{};

        if ( overlapX < overlapY )
        {
            normal = selfPosition.x < otherPosition.x ? glm::vec2{ -1.f, 0.f } : glm::vec2{ 1.f, 0.f };
            depth  = overlapX;
        }
        else
        {
            normal = selfPosition.y < otherPosition.y ? glm::vec2{ -0.f, -1.f } : glm::vec2{ 0.f, 1.f };
            depth  = overlapY;
        }

        const glm::vec2 contactMin   = max( aMin, bMin );
        const glm::vec2 contactMax   = min( aMax, bMax );
        const glm::vec2 contactPoint = ( contactMin + contactMax ) * 0.5f;

        return CollisionInfo{
            .status = CollisionStatus::FULL,
            .contact_point = contactPoint,
            .normal = normal,
            .depth = depth
        };
    }

}
