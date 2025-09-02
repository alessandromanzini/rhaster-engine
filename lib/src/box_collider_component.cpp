#include <rst/temp/framework/component/physics/box_collider_component.h>

#include <rst/__core/gameobject.h>


namespace rst
{
    BoxColliderComponent::BoxColliderComponent( owner_t& owner, glm::vec2 const size, glm::vec2 const position )
        : ColliderComponent{ owner, position }
        , pivots_{
            glm::vec2{ 0.f, 0.f },
            glm::vec2{ size.x, 0.f },
            glm::vec2{ size.x, size.y },
            glm::vec2{ 0.f, size.y }
        } { }


    auto BoxColliderComponent::get_pivots( ) const -> std::span<glm::vec2 const>
    {
        return std::span<glm::vec2 const>{ pivots_ };
    }


    auto BoxColliderComponent::hit_test_impl( ColliderComponent& other ) const -> CollisionInfo
    {
        glm::vec2 const selfPosition  = get_position( );
        glm::vec2 const otherPosition = other.get_position( );

        glm::vec2 const aMin = selfPosition + pivots_[0];
        glm::vec2 const aMax = selfPosition + pivots_[2];

        auto const otherPivots = other.get_pivots( );
        glm::vec2 const bMin   = otherPosition + otherPivots[0];
        glm::vec2 const bMax   = otherPosition + otherPivots[2];

        if ( aMax.x < bMin.x || aMin.x > bMax.x || aMax.y < bMin.y || aMin.y > bMax.y )
        {
            return CollisionInfo{ .status = CollisionStatus::NONE };
        }

        // calculate overlap in each direction
        float const overlapX = std::min( aMax.x, bMax.x ) - std::max( aMin.x, bMin.x );
        float const overlapY = std::min( aMax.y, bMax.y ) - std::max( aMin.y, bMin.y );

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

        glm::vec2 const contactMin   = max( aMin, bMin );
        glm::vec2 const contactMax   = min( aMax, bMax );
        glm::vec2 const contactPoint = ( contactMin + contactMax ) * 0.5f;

        return CollisionInfo{
            .status = CollisionStatus::FULL,
            .contact_point = contactPoint,
            .normal = normal,
            .depth = depth
        };
    }

}
