#include <rst/__input/input_mapping_context.h>

#include <rst/meta/algorithm.h>
#include <rst/__controller/player_controller.h>
#include <rst/__input/input_merge.h>


using namespace rst::input;

// todo: allow thread safe access to the device contexts
namespace rst
{
    // +--------------------------------+
    // | PUBLIC SCOPE                   |
    // +--------------------------------+
    auto InputMappingContext::register_input_action( InputAction const& action, UniformBindingCode const code ) -> void
    {
        // assert( not action_binds_.contains( code ) && "Input action already registered on this code!" );
        action_binds_[code].push_back( action );
    }


    auto InputMappingContext::register_device( PlayerController& controller, DeviceInfo const device_info ) -> void
    {
        device_contexts_.emplace_back( controller, device_info );
    }


    auto InputMappingContext::unregister_device( PlayerController const& controller ) -> void
    {
        std::erase_if(
            device_contexts_, [&controller]( DeviceContext const& ctx )
            {
                return ctx.get_controller( ) == controller;
            } );
    }


    auto InputMappingContext::signal(
        UniformBindingCode const code, TriggerEvent const trigger, DeviceInfo const device_info ) -> void
    {
        if ( not action_binds_.contains( code ) )
        {
            return;
        }

        auto& actions    = action_binds_.at( code );
        bool const value = trigger_to_value( trigger );

        // for every input action bound to the ia, we signal the device context to stack the input (will be dispatched later)
        for ( auto& device : device_contexts_ )
        {
            if ( not device.is_device_suitable( device_info ) )
            {
                continue;
            }

            for ( auto const& [uid, modifiers] : actions )
            {
                device.signal_input( { uid, apply_modifiers_to_value( value, modifiers ), trigger } );
            }
        }
    }


    auto InputMappingContext::dispatch( ) -> void
    {
        // dispatch all accumulated inputs
        for ( auto& device : device_contexts_ )
        {
            device.execute_commands( );
        }
    }


    auto InputMappingContext::get_devices( ) const -> std::list<DeviceContext> const&
    {
        return device_contexts_;
    }


    // +--------------------------------+
    // | PRIVATE SCOPE                  |
    // +--------------------------------+
    auto InputMappingContext::bind_to_input_action_impl(
        PlayerController const& controller, UID const uid, InputCommandVariant&& command, TriggerEvent const trigger ) -> void
    {
        DeviceContext* const context = meta::find_or_none(
            device_contexts_, [&]( DeviceContext const& ctx ) { return ctx.get_controller( ) == controller; } );

        assert( context != nullptr && "Can't bind IA: Device context has not been registered for this controller!" );
        context->bind_command( uid, { std::move( command ), trigger } );
    }
}
