#ifndef BOXCOLLIDERCOMPONENT_H
#define BOXCOLLIDERCOMPONENT_H

#include <framework/component/physics/ColliderComponent.h>

#include <array>


namespace engine
{
    // TODO: Add support for inclusive and exclusive edges
    class BoxColliderComponent final : public ColliderComponent
    {
    public:
        explicit BoxColliderComponent( owner_t& owner, glm::vec2 size, glm::vec2 position = {} );
        [[nodiscard]] std::span<const glm::vec2> get_pivots( ) const override;

    private:
        std::array<glm::vec2, 4> pivots_{};

        [[nodiscard]] CollisionInfo hit_test_impl( ColliderComponent& other ) const override;

    };

}


#endif //!BOXCOLLIDERCOMPONENT_H
