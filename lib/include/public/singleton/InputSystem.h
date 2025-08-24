#ifndef INPUTSYSTEM_H
#define INPUTSYSTEM_H

// +--------------------------------+
// | PROJECT HEADERS				|
// +--------------------------------+
#include <framework/binding/binding_device.h>
#include <framework/binding/binding_types.h>
#include <framework/binding/InputBuffer.h>
#include <framework/binding/InputMappingContext.h>
#include <singleton/Singleton.h>

// +--------------------------------+
// | GLM HEADERS					|
// +--------------------------------+
#include <glm.hpp>

// +--------------------------------+
// | STANDARD HEADERS				|
// +--------------------------------+
#include <unordered_set>


namespace engine
{
    // TODO: Pimpl InputSystem to hide SDL dependencies. Use service locator pattern
    // TODO: Add to service locator and separate input mapping context from input service
    class InputSystem final : public Singleton<InputSystem>
    {
        using device_id_t = decltype(binding::DeviceInfo::id);
        friend class Singleton;

    public:
        bool process_input( );

        [[nodiscard]] device_id_t fetch_free_gamepad_id();

        [[nodiscard]] InputMappingContext& get_input_mapping_context( );

    private:
        bool request_quit_{ false };

        InputMappingContext input_mapping_context_{};

        binding::InputBuffer keyboard_buffer_{};
        binding::InputBuffer gamepad_buffer_{};
        std::unordered_map<SDL_JoystickID, device_id_t> joystick_id_to_device_id_;

        InputSystem( ) = default;

        void poll( );

        void forward_code_to_contexts( binding::UniformBindingCode code, binding::TriggerEvent trigger,
                                       binding::DeviceInfo deviceInfo );
        void forward_state_to_contexts( binding::DeviceType deviceType );

        [[nodiscard]] binding::InputBuffer& select_buffer( binding::DeviceType deviceType );

        [[nodiscard]] std::vector<device_id_t> get_connected_gamepad_ids();

    };

    extern InputSystem& INPUT_SYSTEM;

}

#endif //!INPUTSYSTEM_H
