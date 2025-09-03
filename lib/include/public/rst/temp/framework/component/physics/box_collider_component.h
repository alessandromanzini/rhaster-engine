#ifndef RST_BOX_COLLIDER_COMPONENT_H
#define RST_BOX_COLLIDER_COMPONENT_H

#include <rst/pch.h>

#include <rst/temp/framework/component/physics/collider_component.h>


namespace rst
{
    // TODO: Add support for inclusive and exclusive edges
    class box_collider_component final : public collider_component
    {
    public:
        explicit box_collider_component( owner_type& owner, glm::vec2 size, glm::vec2 position = {} );
        [[nodiscard]] auto pivots( ) const -> std::span<glm::vec2 const> override;

    private:
        std::array<glm::vec2, 4> pivots_{};

        [[nodiscard]] auto hit_test_impl( collider_component& other ) const -> collision_info override;

    };

}


#endif //!RST_BOX_COLLIDER_COMPONENT_H
