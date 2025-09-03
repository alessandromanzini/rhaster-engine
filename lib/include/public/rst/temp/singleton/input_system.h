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
    class input_system final : public singleton<input_system>
    {
        friend class singleton;

    public:
        auto process_input( ) -> bool;

        [[nodiscard]] auto fetch_free_gamepad_id( ) -> input::device_id_type;

        [[nodiscard]] auto input_mapping_context( ) -> input_mapping_context&;

    private:
        bool request_quit_{ false };

        rst::input_mapping_context input_mapping_context_{};

        input::input_buffer keyboard_buffer_{};
        input::input_buffer gamepad_buffer_{};
        std::unordered_map<SDL_JoystickID, input::device_id_type> joystick_id_to_device_id_;

        input_system( ) = default;

        auto poll( ) -> void;

        auto forward_code_to_contexts(
            input::unicode code, input::trigger trigger, input::device_info device_info ) -> void;
        auto forward_state_to_contexts( input::device_type device_type ) -> void;

        [[nodiscard]] auto select_buffer( input::device_type device_type ) -> input::input_buffer&;

        [[nodiscard]] auto connected_gamepad_ids( ) -> std::vector<input::device_id_type>;
    };

    extern input_system& INPUT_SYSTEM;
}


#endif //!RST_INPUT_SYSTEM_H
