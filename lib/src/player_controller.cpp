#include <rst/__controller/player_controller.h>

#include <rst/temp/singleton/game_time.h>
#include <rst/temp/singleton/input_system.h>


using namespace rst::input;
// todo: examine code
namespace rst
{
    PlayerController::~PlayerController( ) noexcept
    {
        if ( has_registered_device_.clone( ) )
        {
            auto& context = INPUT_SYSTEM.get_input_mapping_context( );
            context.unregister_device( *this );
            has_registered_device_.set( false );
        }
    }


    auto PlayerController::has_registered_device( ) const -> bool
    {
        return has_registered_device_.clone( );
    }


    auto PlayerController::try_register_device(
        DeviceType const device_type, std::chrono::milliseconds const timeout ) -> void
    {
        auto& context = INPUT_SYSTEM.get_input_mapping_context( );
        switch ( device_type )
        {
            case DeviceType::KEYBOARD:
                try_register_keyboard_impl( context, timeout );
                break;

            case DeviceType::GAMEPAD:
                try_register_gamepad_impl( context, timeout );
                break;
        }
    }


    auto PlayerController::device_registered( InputMappingContext& /* context */, DeviceInfo /* deviceInfo */ ) -> void { }


    auto PlayerController::try_register_keyboard_impl(
        InputMappingContext& context, std::chrono::milliseconds /* timeout */ ) -> void
    {
        constexpr DeviceInfo device_info{ DeviceType::KEYBOARD };

        context.register_device( *this, device_info );
        has_registered_device_.set( true );

        device_registered( context, device_info );
    }


    auto PlayerController::try_register_gamepad_impl( InputMappingContext& context, std::chrono::milliseconds timeout ) -> void
    {
        if ( timeout < duration_cast<std::chrono::milliseconds>(std::chrono::duration<float>(REGISTER_ATTEMPT_TIME_STEP_ * 2)) )
        {
            return;
        }
        try
        {
            DeviceInfo const device_info{ DeviceType::GAMEPAD, INPUT_SYSTEM.fetch_free_gamepad_id( ) };

            context.register_device( *this, device_info );

            has_registered_device_.set( true );

            device_registered( context, device_info );
        }
        catch ( std::runtime_error& )
        {
            has_registered_device_.set( false );
            GAME_TIME.set_timeout(REGISTER_ATTEMPT_TIME_STEP_, [&, this] {
                timeout -= duration_cast<std::chrono::milliseconds>(std::chrono::duration<float>(REGISTER_ATTEMPT_TIME_STEP_));
                try_register_gamepad_impl(context, timeout);
            });
        }
    }

}
