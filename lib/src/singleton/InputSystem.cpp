#include <singleton/InputSystem.h>

// +--------------------------------+
// | PROJECT HEADERS				|
// +--------------------------------+
#include <framework/binding/binding_device.h>
#include <framework/binding/InputMappingContext.h>
#include <singleton/UIController.h>

// +--------------------------------+
// | SDL HEADERS					|
// +--------------------------------+
#include <SDL.h>

// +--------------------------------+
// | GLM HEADERS					|
// +--------------------------------+
#include <glm.hpp>

// +--------------------------------+
// | STANDARD HEADERS				|
// +--------------------------------+
#include <algorithm>
#include <cassert>
#include <vector>

using namespace engine::binding;


namespace engine
{
    InputSystem& INPUT_SYSTEM = InputSystem::get_instance( );


    bool InputSystem::process_input( )
    {
        // Poll SDL events
        poll( );

        // Dispatch all input actions
        input_mapping_context_.dispatch( );

        return !request_quit_;
    }


    InputSystem::device_id_t InputSystem::fetch_free_gamepad_id( )
    {
        auto ids = get_connected_gamepad_ids( );

        // Remove already-used gamepad IDs
        for ( const auto& device : input_mapping_context_.get_devices( ) )
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

        // Throw an error if no gamepad ID is available
        if ( ids.empty( ) )
        {
            throw std::runtime_error( "No gamepad id available." );
        }

        // Return the first one (they're already sorted)
        return ids.front( );
    }


    InputMappingContext& InputSystem::get_input_mapping_context( )
    {
        return input_mapping_context_;
    }


    void InputSystem::poll( )
    {
        SDL_Event e;
        while ( SDL_PollEvent( &e ) )
        {
            const UniformBindingCode key{ static_cast<binding::key_t>( e.key.keysym.sym ) };
            const UniformBindingCode btn{ static_cast<binding::btn_t>( e.cbutton.button ) };

            switch ( e.type )
            {
                case SDL_QUIT:
                    request_quit_ = true;
                    return;

                case SDL_KEYDOWN:
                    forward_code_to_contexts( key, TriggerEvent::TRIGGERED, { DeviceType::KEYBOARD } );
                    break;

                case SDL_KEYUP:
                    forward_code_to_contexts( key, TriggerEvent::RELEASED, { DeviceType::KEYBOARD } );
                    break;

                case SDL_CONTROLLERBUTTONDOWN:
                case SDL_CONTROLLERBUTTONUP:
                    if ( auto it = joystick_id_to_device_id_.find( e.cbutton.which );
                        it != joystick_id_to_device_id_.end( ) )
                    {
                        const TriggerEvent event{
                            e.type == SDL_CONTROLLERBUTTONDOWN ? TriggerEvent::TRIGGERED : TriggerEvent::RELEASED
                        };
                        forward_code_to_contexts( btn, event, { DeviceType::GAMEPAD, it->second } );
                    }
                    break;

                case SDL_MOUSEBUTTONDOWN:
                    printf( "Mouse button %d pressed at (%d, %d)\n", e.button.button, e.button.x, e.button.y );

                default:
                    break;
            }

            // Process events for ImGui
            UI_CONTROLLER.process_input( e );
        }

        // Dispatch all keys and buttons pressed every frame
        forward_state_to_contexts( DeviceType::KEYBOARD );
        forward_state_to_contexts( DeviceType::GAMEPAD );
    }


    void InputSystem::forward_code_to_contexts( const UniformBindingCode code,
                                                const TriggerEvent trigger,
                                                const DeviceInfo deviceInfo )
    {
        InputBuffer& buffer{ select_buffer( deviceInfo.type ) };

        // ReSharper disable once CppIncompleteSwitchStatement
        // ReSharper disable once CppDefaultCaseNotHandledInSwitchStatement
        switch ( trigger )
        {
            case TriggerEvent::TRIGGERED:
                // We must trigger only if the key/button is not already pressed (was released)
                if ( not buffer.is_pressed( code, deviceInfo.id ) )
                {
                    buffer.trigger( code, deviceInfo.id );
                    input_mapping_context_.signal( code, TriggerEvent::TRIGGERED, deviceInfo );
                }
                break;
            case TriggerEvent::PRESSED:
                input_mapping_context_.signal( code, TriggerEvent::PRESSED, deviceInfo );
                break;
            case TriggerEvent::RELEASED:
                buffer.release( code, deviceInfo.id );
                input_mapping_context_.signal( code, TriggerEvent::RELEASED, deviceInfo );
                break;
        }
    }


    void InputSystem::forward_state_to_contexts( const DeviceType deviceType )
    {
        const InputBuffer& buffer{ select_buffer( deviceType ) };
        for ( const auto& [code, deviceId] : buffer.get_pressed_this_frame( ) )
        {
            input_mapping_context_.signal( code, TriggerEvent::PRESSED, { deviceType, deviceId } );
        }
    }


    InputBuffer& InputSystem::select_buffer( const DeviceType deviceType )
    {
        switch ( deviceType )
        {
            case DeviceType::KEYBOARD:
                return keyboard_buffer_;
            case DeviceType::GAMEPAD:
                return gamepad_buffer_;
        }
        throw std::invalid_argument( "Invalid device type." );
    }


    std::vector<InputSystem::device_id_t> InputSystem::get_connected_gamepad_ids( )
    {
        std::vector<device_id_t> gamepadIds{};
        for ( int i{}; i < SDL_NumJoysticks( ); ++i )
        {
            if ( SDL_IsGameController( i ) )
            {
                if ( SDL_GameController* controller = SDL_GameControllerOpen( i ) )
                {
                    SDL_Joystick* joystick                = SDL_GameControllerGetJoystick( controller );
                    SDL_JoystickID instanceId             = SDL_JoystickInstanceID( joystick );
                    joystick_id_to_device_id_[instanceId] = static_cast<device_id_t>( i );
                    gamepadIds.push_back( static_cast<device_id_t>( i ) );
                }
            }
        }
        return gamepadIds;
    }

}
