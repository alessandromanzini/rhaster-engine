#include <framework/binding/InputMappingContext.h>

// +--------------------------------+
// | PROJECT HEADERS				|
// +--------------------------------+
#include <framework/binding/binding_controls.h>
#include <framework/controller/PlayerController.h>

// +--------------------------------+
// | STANDARD HEADERS				|
// +--------------------------------+
#include <cassert>

using namespace engine::binding;


// todo> allow thread safe access to the device contexts
namespace engine
{
    // +--------------------------------+
    // | PUBLIC SCOPE					|
    // +--------------------------------+
    void InputMappingContext::register_input_action( const InputAction& action, const UniformBindingCode code )
    {
        // assert( not action_binds_.contains( code ) && "Input action already registered on this code!" );
        action_binds_[code].push_back( action );
    }


    void InputMappingContext::register_input_action( const UID uid, const UniformBindingCode code )
    {
        register_input_action( InputAction{ uid }, code );
    }


    void InputMappingContext::register_device( PlayerController& controller, const DeviceInfo deviceInfo )
    {
        device_contexts_.emplace_back( controller, deviceInfo );
    }


    void InputMappingContext::unregister_device( const PlayerController& controller )
    {
        if ( const auto it = find_device_context( device_contexts_, controller ); it.has_value( ) )
        {
            device_contexts_.erase( it.value( ) );
        }
    }


    void InputMappingContext::signal( const UniformBindingCode code, const TriggerEvent trigger, const DeviceInfo deviceInfo )
    {
        if ( not action_binds_.contains( code ) )
        {
            return;
        }

        auto& actions    = action_binds_.at( code );
        const bool value = trigger_to_value( trigger );

        // For every input action bound to the ia, we signal the device context to stack the input (will be dispatched later)
        for ( auto& device : device_contexts_ )
        {
            if ( not device.is_device_suitable( deviceInfo ) )
            {
                continue;
            }

            for ( const auto& [uid, modifiers] : actions )
            {
                device.signal_input( { uid, apply_modifiers_to_value( value, modifiers ), trigger } );
            }
        }
    }


    void InputMappingContext::dispatch( )
    {
        // Dispatch all accumulated inputs
        for ( auto& device : device_contexts_ )
        {
            device.execute_commands( );
        }
    }


    const std::list<DeviceContext>& InputMappingContext::get_devices( ) const
    {
        return device_contexts_;
    }


    // +--------------------------------+
    // | PRIVATE SCOPE					|
    // +--------------------------------+
    InputMappingContext::optional_device_it InputMappingContext::find_device_context( device_contexts_container_t& contexts,
        const PlayerController& controller ) const
    {
        const auto it = std::ranges::find_if( contexts,
                                              [&controller]( const auto& context )
                                                  {
                                                      return context.get_controller( ) == controller;
                                                  } );

        if ( it != contexts.end( ) )
        {
            return it;
        }
        return std::nullopt;
    }


    void InputMappingContext::bind_to_input_action_impl( const PlayerController& controller,
                                                         const UID uid,
                                                         input_command_variant_t&& command,
                                                         const TriggerEvent trigger )
    {
        const auto deviceIt = find_device_context( device_contexts_, controller );
        assert( deviceIt.has_value() && "Can't bind IA: Device context has not been registered for this controller!" );

        deviceIt.value( )->bind_command( uid, { std::move( command ), trigger } );
    }

}
