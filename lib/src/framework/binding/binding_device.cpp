#include <framework/binding/binding_device.h>

// +---------------------------+
// | PROJECT HEADERS           |
// +---------------------------+
#include <framework/binding/binding_controls.h>
#include <framework/controller/PlayerController.h>


namespace engine::binding
{
    // +---------------------------+
    // | COMMAND SET               |
    // +---------------------------+
    void CommandSet::set( CommandInfo&& info )
    {
        switch ( info.trigger )
        {
            case TriggerEvent::TRIGGERED:
                triggered_commands_.push_back( std::move( info.command ) );
                break;
            case TriggerEvent::PRESSED:
                pressed_commands_.push_back( std::move( info.command ) );
                break;
            case TriggerEvent::RELEASED:
                released_commands_.push_back( std::move( info.command ) );
                break;
            default:
                assert( false && "Invalid trigger event!" );
        }
    }


    void CommandSet::execute( const input_value_variant_t value, const TriggerEvent trigger ) const
    {
        for ( const auto& command : select_command_list( trigger ) )
        {
            std::visit(
                [&]( auto&& fn )
                    {
                        using function_sig_t = std::decay_t<decltype( fn )>;
                        using param_t        = std::tuple_element_t<0, typename meta::function_traits<function_sig_t>::params_t>;
                        fn( convert_input_value<param_t>( value ) );
                    },
                command
            );
        }
    }


    const CommandSet::command_list_t& CommandSet::select_command_list( const TriggerEvent trigger ) const
    {
        switch ( trigger )
        {
            case TriggerEvent::TRIGGERED:
                return triggered_commands_;
            case TriggerEvent::PRESSED:
                return pressed_commands_;
            case TriggerEvent::RELEASED:
                return released_commands_;
        }
        throw std::invalid_argument( "Invalid trigger event!" );
    }


    // +---------------------------+
    // | DEVICE CONTEXT            |
    // +---------------------------+
    DeviceContext::DeviceContext( PlayerController& controller, const DeviceInfo deviceInfo )
        : device_info_{ deviceInfo }
        , controller_ref_{ controller } { }


    PlayerController& DeviceContext::get_controller( )
    {
        return controller_ref_;
    }


    const PlayerController& DeviceContext::get_controller( ) const
    {
        return controller_ref_;
    }


    bool DeviceContext::is_device_suitable( const DeviceInfo deviceInfo ) const
    {
        switch ( deviceInfo.type )
        {
            case DeviceType::KEYBOARD:
                return device_info_.type == DeviceType::KEYBOARD;
            case DeviceType::GAMEPAD:
                return device_info_.type == DeviceType::GAMEPAD && device_info_.id == deviceInfo.id;
            default:
                return false;
        }
    }


    void DeviceContext::bind_command( const UID uid, CommandInfo&& commandInfo )
    {
        auto& commands = command_sets_[uid];
        commands.set( std::move( commandInfo ) );
    }


    void DeviceContext::signal_input( const InputSnapshot& input )
    {
        if ( not command_sets_.contains( input.uid ) )
        {
            return;
        }

        // If the input is not already in the queue, we add it
        if ( const auto queued = find_queued_input( input ); not queued.has_value( ) )
        {
            signaled_inputs_queue_.push_back( input );
        }
        else
        {
            // If the input is already in the queue, we update its value
            merge_value_to_snapshot( *queued.value( ), input.value );
        }
    }


    void DeviceContext::execute_commands( )
    {
        while ( not signaled_inputs_queue_.empty( ) )
        {
            const auto& [uid, value, trigger] = signaled_inputs_queue_.front( );
            signaled_inputs_queue_.pop_front( );

            command_sets_.at( uid ).execute( value, trigger );
        }
    }


    std::optional<decltype(DeviceContext::signaled_inputs_queue_)::iterator> DeviceContext::find_queued_input(
        const InputSnapshot& target )
    {
        if ( auto it = std::ranges::find( signaled_inputs_queue_, target );
            it != signaled_inputs_queue_.end( ) )
        {
            return it;
        }
        return std::nullopt;
    }

}
