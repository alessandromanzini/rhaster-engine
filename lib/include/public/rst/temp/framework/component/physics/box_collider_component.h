#ifndef RST_BOX_COLLIDER_COMPONENT_H
#define RST_BOX_COLLIDER_COMPONENT_H

#include <rst/pch.h>

#include <rst/temp/framework/component/physics/collider_component.h>


namespace rst
{
    // TODO: Add support for inclusive and exclusive edges
    class BoxColliderComponent final : public ColliderComponent
    {
    public:
        explicit BoxColliderComponent( owner_t& owner, glm::vec2 size, glm::vec2 position = {} );
        [[nodiscard]] auto get_pivots( ) const -> std::span<glm::vec2 const> override;

    private:
        std::array<glm::vec2, 4> pivots_{};

        [[nodiscard]] auto hit_test_impl( ColliderComponent& other ) const -> CollisionInfo override;

    };

}


#endif //!RST_BOX_COLLIDER_COMPONENT_H
