#include <rst/__input/input_mapping_context.h>

#include <rst/meta/algorithm.h>
#include <rst/__input/input_merge.h>
#include <rst/__input/controller/player_controller.h>


using namespace rst::input;

// todo: allow thread safe access to the device contexts
namespace rst
{
    // +--------------------------------+
    // | PUBLIC SCOPE                   |
    // +--------------------------------+
    auto input_mapping_context::register_input_action( input_action const& action, unicode const code ) -> void
    {
        // assert( not action_binds_.contains( code ) && "Input action already registered on this code!" );
        action_binds_[code].push_back( action );
    }


    auto input_mapping_context::register_device( player_controller& controller, device_info const device_info ) -> void
    {
        device_contexts_.emplace_back( controller, device_info );
    }


    auto input_mapping_context::unregister_device( player_controller const& controller ) -> void
    {
        std::erase_if(
            device_contexts_, [&controller]( device_context const& ctx )
            {
                return ctx.controller( ) == controller;
            } );
    }


    auto input_mapping_context::signal(
        unicode const code, trigger const trigger, device_info const device_info ) -> void
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

            for ( auto const& [mark, modifiers] : actions )
            {
                device.signal_input( { mark, apply_modifiers_to_value( value, modifiers ), trigger } );
            }
        }
    }


    auto input_mapping_context::dispatch( ) -> void
    {
        // dispatch all accumulated inputs
        for ( auto& device : device_contexts_ )
        {
            device.execute_commands( );
        }
    }


    auto input_mapping_context::devices( ) const -> std::list<device_context> const&
    {
        return device_contexts_;
    }


    // +--------------------------------+
    // | PRIVATE SCOPE                  |
    // +--------------------------------+
    auto input_mapping_context::bind_to_input_action_impl(
        player_controller const& controller, earmark const mark, input_command_type&& command, trigger const trigger ) -> void
    {
        device_context* const context = meta::find_or_none(
            device_contexts_, [&]( device_context const& ctx ) { return ctx.controller( ) == controller; } );

        assert( context != nullptr && "Can't bind IA: Device context has not been registered for this controller!" );
        context->bind_command( mark, { std::move( command ), trigger } );
    }
}
