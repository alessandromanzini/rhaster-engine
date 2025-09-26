#include <rst/__input/controller/player_controller.h>

#include <rst/temp/singleton/game_time.h>
#include <rst/temp/singleton/input_system.h>


using namespace rst::input;
// todo: examine code
namespace rst
{
    player_controller::~player_controller( ) noexcept
    {
        if ( has_registered_device_.clone( ) )
        {
            auto& context = INPUT_SYSTEM.input_mapping_context( );
            context.unregister_device( *this );
            has_registered_device_.set( false );
        }
    }


    auto player_controller::has_registered_device( ) const -> bool
    {
        return has_registered_device_.clone( );
    }


    auto player_controller::try_register_device( device_type const device_type, std::chrono::milliseconds const timeout ) -> void
    {
        auto& context = INPUT_SYSTEM.input_mapping_context( );
        switch ( device_type )
        {
            case device_type::keyboard: try_register_keyboard_impl( context, timeout );
                break;

            case device_type::gamepad: try_register_gamepad_impl( context, timeout );
                break;
        }
    }


    auto player_controller::device_registered( input_mapping_context& /* context */, device_info /* deviceInfo */ ) -> void { }


    auto player_controller::try_register_keyboard_impl(
        input_mapping_context& context, std::chrono::milliseconds /* timeout */ ) -> void
    {
        constexpr device_info device_info{ device_type::keyboard };

        context.register_device( *this, device_info );
        has_registered_device_.set( true );

        device_registered( context, device_info );
    }


    auto player_controller::try_register_gamepad_impl( input_mapping_context& context, std::chrono::milliseconds timeout ) -> void
    {
        if ( timeout < duration_cast<std::chrono::milliseconds>(
                 std::chrono::duration<float>( register_attempt_time_step_ * 2 ) ) )
        {
            return;
        }
        try
        {
            device_info const device_info{ device_type::gamepad, INPUT_SYSTEM.fetch_free_gamepad_id( ) };

            context.register_device( *this, device_info );

            has_registered_device_.set( true );

            device_registered( context, device_info );
        }
        catch ( std::runtime_error& )
        {
            has_registered_device_.set( false );
            GAME_TIME.set_timeout(
                register_attempt_time_step_, [&, this]
                {
                    timeout -= duration_cast<std::chrono::milliseconds>(
                        std::chrono::duration<float>( register_attempt_time_step_ ) );
                    try_register_gamepad_impl( context, timeout );
                } );
        }
    }
}
