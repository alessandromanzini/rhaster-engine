// ReSharper disable CppClassCanBeFinal
#ifndef RST_PLAYER_CONTROLLER_H
#define RST_PLAYER_CONTROLLER_H

#include <rst/pch.h>

#include <rst/__input/controller/base_controller.h>

#include <rst/data_type/safe_resource.h>
#include <rst/__input/input_mapping_context.h>


namespace rst
{
    class player_controller : public base_controller
    {
    public:
        ~player_controller( ) noexcept override;

        [[nodiscard]] auto has_registered_device( ) const -> bool;

    protected:
        player_controller( ) = default;

        auto try_register_device(
            input::device_type device_type, std::chrono::milliseconds timeout = std::chrono::milliseconds{ 2000 } ) -> void;

        /**
         * This method is called when a device is registered.
         * @param context The input mapping context that registered the device
         * @param device_info Info of the newly registered device
         */
        virtual auto device_registered( input_mapping_context& context, input::device_info device_info ) -> void;

    private:
        static constexpr float register_attempt_time_step_{ 0.1f };

        thread::safe_resource<bool> has_registered_device_{ false };

        auto try_register_keyboard_impl( input_mapping_context& context, std::chrono::milliseconds timeout ) -> void;
        auto try_register_gamepad_impl( input_mapping_context& context, std::chrono::milliseconds timeout ) -> void;
    };
}


#endif //!RST_PLAYER_CONTROLLER_H
