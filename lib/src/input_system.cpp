#include <rst/temp/singleton/input_system.h>

#include <rst/__input/binding_device.h>
#include <rst/__input/input_buffer.h>
#include <rst/__input/input_mapping_context.h>

#include <SDL.h>
#include <rst/__diagnostic/__warren/startle.h>


using namespace rst::input;

namespace rst
{
    auto input_system::process_input( ) -> bool
    {
        // poll SDL events
        poll( );

        // dispatch all input actions
        input_mapping_context_.dispatch( );

        return !request_quit_;
    }


    auto input_system::fetch_free_gamepad_id( ) -> device_id_type
    {
        auto ids = connected_gamepad_ids( );

        // remove already-used gamepad IDs
        for ( auto const& device : input_mapping_context_.devices( ) )
        {
            if ( device.device_info( ).type == device_type::gamepad )
            {
                std::erase( ids, device.device_info( ).id );
            }
            if ( ids.empty( ) )
            {
                break;
            }
        }

        // throw an error if no gamepad ID is available
        if ( ids.empty( ) )
        {
            startle( "no gamepad available." );
        }

        // return the first one (they're already sorted)
        return ids.front( );
    }


    auto input_system::input_mapping_context( ) -> rst::input_mapping_context&
    {
        return input_mapping_context_;
    }


    auto input_system::poll( ) -> void
    {
        SDL_Event e;
        while ( SDL_PollEvent( &e ) )
        {
            unicode key{ static_cast<key_type>( e.key.keysym.sym ) };
            unicode btn{ static_cast<key_type>( e.cbutton.button ) };

            switch ( e.type )
            {
                case SDL_QUIT:
                {
                    request_quit_ = true;
                    return;
                }

                case SDL_KEYDOWN:
                {
                    forward_code_to_contexts( key, trigger::triggered, { device_type::keyboard } );
                    break;
                }

                case SDL_KEYUP:
                {
                    forward_code_to_contexts( key, trigger::released, { device_type::keyboard } );
                    break;
                }

                case SDL_CONTROLLERBUTTONDOWN:
                case SDL_CONTROLLERBUTTONUP:
                {
                    if ( auto it = joystick_id_to_device_id_.find( e.cbutton.which ); it != joystick_id_to_device_id_.end( ) )
                    {
                        trigger const event{
                            e.type == SDL_CONTROLLERBUTTONDOWN ? trigger::triggered : trigger::released
                        };
                        forward_code_to_contexts( btn, event, { device_type::gamepad, it->second } );
                    }
                    break;
                }

                default: break;
            }
        }

        // dispatch all keys and buttons pressed every frame
        forward_state_to_contexts( device_type::keyboard );
        forward_state_to_contexts( device_type::gamepad );
    }


    auto input_system::forward_code_to_contexts(
        unicode const code, trigger const trigger, device_info const device_info ) -> void
    {
        input_buffer& buffer{ select_buffer( device_info.type ) };

        // ReSharper disable once CppIncompleteSwitchStatement
        // ReSharper disable once CppDefaultCaseNotHandledInSwitchStatement
        switch ( trigger )
        {
            case trigger::triggered:
            {
                // we must trigger only if the key/button is not already pressed (was released)
                if ( not buffer.is_pressed( code, device_info.id ) )
                {
                    buffer.trigger( code, device_info.id );
                    input_mapping_context_.signal( code, trigger::triggered, device_info );
                }
                break;
            }

            case trigger::pressed:
            {
                input_mapping_context_.signal( code, trigger::pressed, device_info );
                break;
            }

            case trigger::released:
            {
                buffer.release( code, device_info.id );
                input_mapping_context_.signal( code, trigger::released, device_info );
                break;
            }
        }
    }


    auto input_system::forward_state_to_contexts( device_type const device_type ) -> void
    {
        for ( auto const& [code, device_id] : select_buffer( device_type ) )
        {
            input_mapping_context_.signal( code, trigger::pressed, { device_type, device_id } );
        }
    }


    auto input_system::select_buffer( device_type const device_type ) -> input_buffer&
    {
        switch ( device_type )
        {
            case device_type::keyboard: return keyboard_buffer_;
            case device_type::gamepad: return gamepad_buffer_;
        }
        throw std::invalid_argument( "Invalid device type." );
    }


    auto input_system::connected_gamepad_ids( ) -> std::vector<device_id_type>
    {
        std::vector<device_id_type> gamepad_ids{};
        for ( int i{}; i < SDL_NumJoysticks( ); ++i )
        {
            if ( SDL_IsGameController( i ) )
            {
                if ( SDL_GameController* controller = SDL_GameControllerOpen( i ) )
                {
                    SDL_Joystick* joystick                 = SDL_GameControllerGetJoystick( controller );
                    SDL_JoystickID instance_id             = SDL_JoystickInstanceID( joystick );
                    joystick_id_to_device_id_[instance_id] = static_cast<device_id_type>( i );
                    gamepad_ids.push_back( static_cast<device_id_type>( i ) );
                }
            }
        }
        return gamepad_ids;
    }


    input_system& INPUT_SYSTEM = input_system::instance( );
}
