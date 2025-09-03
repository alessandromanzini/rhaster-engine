#ifndef RST_RIGID_BODY_COMPONENT_H
#define RST_RIGID_BODY_COMPONENT_H

#include <rst/pch.h>

#include <rst/__core/component.h>


namespace rst
{
    // TODO: Improve this class -> add more physics parameters
    class physics_component final : public component
    {
    public:
        explicit physics_component( owner_type& owner );

        auto fixed_tick( ) -> void override;

        auto set_simulate_physics( bool simulate ) -> void;
        [[nodiscard]] auto is_simulating_physics( ) const -> bool;

        [[nodiscard]] auto velocity( ) const -> glm::vec2;

        auto add_force( glm::vec2 force, bool acceleration = false ) -> void;

    private:
        bool simulate_physics_{ true };
        glm::vec2 velocity_{};
    };
}


#endif //!RST_RIGID_BODY_COMPONENT_H
