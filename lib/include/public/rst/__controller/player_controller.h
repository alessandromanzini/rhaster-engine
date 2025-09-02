// ReSharper disable CppClassCanBeFinal
#ifndef RST_PLAYER_CONTROLLER_H
#define RST_PLAYER_CONTROLLER_H

#include <rst/pch.h>

#include <rst/__controller/base_controller.h>

#include <rst/framework/data_type.h>
#include <rst/__input/input_mapping_context.h>


namespace rst
{
    class PlayerController : public BaseController
    {
    public:
        ~PlayerController( ) noexcept override;

        [[nodiscard]] auto has_registered_device( ) const -> bool;

    protected:
        PlayerController( ) = default;

        auto try_register_device(
            input::DeviceType device_type, std::chrono::milliseconds timeout = std::chrono::milliseconds{ 2000 } ) -> void;

        /**
         * This method is called when a device is registered.
         * @param context The input mapping context that registered the device
         * @param device_info Info of the newly registered device
         */
        virtual auto device_registered( InputMappingContext& context, input::DeviceInfo device_info ) -> void;

    private:
        static constexpr float REGISTER_ATTEMPT_TIME_STEP_{ 0.1f };

        thread::SafeResource<bool> has_registered_device_{ false };

        auto try_register_keyboard_impl( InputMappingContext& context, std::chrono::milliseconds timeout ) -> void;
        auto try_register_gamepad_impl( InputMappingContext& context, std::chrono::milliseconds timeout ) -> void;
    };
}


#endif //!RST_PLAYER_CONTROLLER_H
