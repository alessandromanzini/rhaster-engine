#ifndef RST_INPUT_SYSTEM_H
#define RST_INPUT_SYSTEM_H

#include <rst/pch.h>

#include <rst/temp/singleton/Singleton.h>
#include <rst/__input/binding_device.h>
#include <rst/__input/input_buffer.h>
#include <rst/__input/input_mapping_context.h>
#include <rst/__input/input_type.h>


namespace rst
{
    // TODO: Pimpl InputSystem to hide SDL dependencies. Use service locator pattern
    // TODO: Add to service locator and separate input mapping context from input service
    class InputSystem final : public Singleton<InputSystem>
    {
        friend class Singleton;

    public:
        auto process_input( ) -> bool;

        [[nodiscard]] auto fetch_free_gamepad_id( ) -> input::DeviceId;

        [[nodiscard]] auto get_input_mapping_context( ) -> InputMappingContext&;

    private:
        bool request_quit_{ false };

        InputMappingContext input_mapping_context_{};

        input::InputBuffer keyboard_buffer_{};
        input::InputBuffer gamepad_buffer_{};
        std::unordered_map<SDL_JoystickID, input::DeviceId> joystick_id_to_device_id_;

        InputSystem( ) = default;

        auto poll( ) -> void;

        auto forward_code_to_contexts(
            input::UniformBindingCode code, input::TriggerEvent trigger, input::DeviceInfo device_info ) -> void;
        auto forward_state_to_contexts( input::DeviceType device_type ) -> void;

        [[nodiscard]] auto select_buffer( input::DeviceType device_type ) -> input::InputBuffer&;

        [[nodiscard]] auto get_connected_gamepad_ids( ) -> std::vector<input::DeviceId>;
    };

    extern InputSystem& INPUT_SYSTEM;
}


#endif //!RST_INPUT_SYSTEM_H
