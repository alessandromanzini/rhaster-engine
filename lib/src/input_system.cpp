#include <rst/temp/singleton/input_system.h>

#include <rst/__input/binding_device.h>
#include <rst/__input/input_buffer.h>
#include <rst/__input/input_mapping_context.h>

#include <SDL.h>


using namespace rst::input;

namespace rst
{
    auto InputSystem::process_input( ) -> bool
    {
        // poll SDL events
        poll( );

        // dispatch all input actions
        input_mapping_context_.dispatch( );

        return !request_quit_;
    }


    auto InputSystem::fetch_free_gamepad_id( ) -> DeviceId
    {
        auto ids = get_connected_gamepad_ids( );

        // remove already-used gamepad IDs
        for ( auto const& device : input_mapping_context_.get_devices( ) )
        {
            if ( device.get_device_info( ).type == DeviceType::GAMEPAD )
            {
                std::erase( ids, device.get_device_info( ).id );
            }
            if ( ids.empty( ) )
            {
                break;
            }
        }

        // throw an error if no gamepad ID is available
        if ( ids.empty( ) )
        {
            throw std::runtime_error( "No gamepad id available." );
        }

        // return the first one (they're already sorted)
        return ids.front( );
    }


    auto InputSystem::get_input_mapping_context( ) -> InputMappingContext&
    {
        return input_mapping_context_;
    }


    auto InputSystem::poll( ) -> void
    {
        SDL_Event e;
        while ( SDL_PollEvent( &e ) )
        {
            UniformBindingCode key{ static_cast<KeyCode>( e.key.keysym.sym ) };
            UniformBindingCode btn{ static_cast<KeyCode>( e.cbutton.button ) };

            switch ( e.type )
            {
                case SDL_QUIT:
                {
                    request_quit_ = true;
                    return;
                }

                case SDL_KEYDOWN:
                {
                    forward_code_to_contexts( key, TriggerEvent::TRIGGERED, { DeviceType::KEYBOARD } );
                    break;
                }

                case SDL_KEYUP:
                {
                    forward_code_to_contexts( key, TriggerEvent::RELEASED, { DeviceType::KEYBOARD } );
                    break;
                }

                case SDL_CONTROLLERBUTTONDOWN:
                case SDL_CONTROLLERBUTTONUP:
                {
                    if ( auto it = joystick_id_to_device_id_.find( e.cbutton.which ); it != joystick_id_to_device_id_.end( ) )
                    {
                        TriggerEvent const event{
                            e.type == SDL_CONTROLLERBUTTONDOWN ? TriggerEvent::TRIGGERED : TriggerEvent::RELEASED
                        };
                        forward_code_to_contexts( btn, event, { DeviceType::GAMEPAD, it->second } );
                    }
                    break;
                }

                case SDL_MOUSEBUTTONDOWN:
                {
                    printf( "Mouse button %d pressed at (%d, %d)\n", e.button.button, e.button.x, e.button.y );
                }

                default: break;
            }
        }

        // dispatch all keys and buttons pressed every frame
        forward_state_to_contexts( DeviceType::KEYBOARD );
        forward_state_to_contexts( DeviceType::GAMEPAD );
    }


    auto InputSystem::forward_code_to_contexts(
        UniformBindingCode const code, TriggerEvent const trigger, DeviceInfo const device_info ) -> void
    {
        InputBuffer& buffer{ select_buffer( device_info.type ) };

        // ReSharper disable once CppIncompleteSwitchStatement
        // ReSharper disable once CppDefaultCaseNotHandledInSwitchStatement
        switch ( trigger )
        {
            case TriggerEvent::TRIGGERED:
            {
                // we must trigger only if the key/button is not already pressed (was released)
                if ( not buffer.is_pressed( code, device_info.id ) )
                {
                    buffer.trigger( code, device_info.id );
                    input_mapping_context_.signal( code, TriggerEvent::TRIGGERED, device_info );
                }
                break;
            }

            case TriggerEvent::PRESSED:
            {
                input_mapping_context_.signal( code, TriggerEvent::PRESSED, device_info );
                break;
            }

            case TriggerEvent::RELEASED:
            {
                buffer.release( code, device_info.id );
                input_mapping_context_.signal( code, TriggerEvent::RELEASED, device_info );
                break;
            }
        }
    }


    auto InputSystem::forward_state_to_contexts( DeviceType const device_type ) -> void
    {
        for ( auto const& [code, device_id] : select_buffer( device_type ).pressed_this_frame( ) )
        {
            input_mapping_context_.signal( code, TriggerEvent::PRESSED, { device_type, device_id } );
        }
    }


    auto InputSystem::select_buffer( DeviceType const device_type ) -> InputBuffer&
    {
        switch ( device_type )
        {
            case DeviceType::KEYBOARD: return keyboard_buffer_;
            case DeviceType::GAMEPAD: return gamepad_buffer_;
        }
        throw std::invalid_argument( "Invalid device type." );
    }


    auto InputSystem::get_connected_gamepad_ids( ) -> std::vector<DeviceId>
    {
        std::vector<DeviceId> gamepad_ids{};
        for ( int i{}; i < SDL_NumJoysticks( ); ++i )
        {
            if ( SDL_IsGameController( i ) )
            {
                if ( SDL_GameController* controller = SDL_GameControllerOpen( i ) )
                {
                    SDL_Joystick* joystick                 = SDL_GameControllerGetJoystick( controller );
                    SDL_JoystickID instance_id             = SDL_JoystickInstanceID( joystick );
                    joystick_id_to_device_id_[instance_id] = static_cast<DeviceId>( i );
                    gamepad_ids.push_back( static_cast<DeviceId>( i ) );
                }
            }
        }
        return gamepad_ids;
    }


    InputSystem& INPUT_SYSTEM = InputSystem::get_instance( );
}
