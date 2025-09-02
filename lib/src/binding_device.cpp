#include <rst/__input/binding_device.h>

#include <rst/meta/algorithm.h>
#include <rst/__controller/player_controller.h>
#include <rst/__input/input_merge.h>


namespace rst::input
{
    // +---------------------------+
    // | COMMAND SET               |
    // +---------------------------+
    auto CommandSet::set( CommandInfo&& info ) -> void
    {
        switch ( info.trigger )
        {
            case TriggerEvent::TRIGGERED: triggered_commands_.push_back( std::move( info.command ) );
                break;
            case TriggerEvent::PRESSED: pressed_commands_.push_back( std::move( info.command ) );
                break;
            case TriggerEvent::RELEASED: released_commands_.push_back( std::move( info.command ) );
                break;
            default: assert( false && "Invalid trigger event!" );
        }
    }


    auto CommandSet::execute( InputValueVariant const value, TriggerEvent const trigger ) const -> void
    {
        for ( auto const& command : select_command_list( trigger ) )
        {
            std::visit(
                [&]<typename T>( T&& fn )
                {
                    using FunctionSigType = std::decay_t<T>;
                    using ParamType       = std::tuple_element_t<0, typename meta::function_traits<FunctionSigType>::params_t>;
                    fn( convert_input_value<ParamType>( value ) );
                }, command );
        }
    }


    auto CommandSet::select_command_list( TriggerEvent const trigger ) const -> CommandList const&
    {
        switch ( trigger )
        {
            case TriggerEvent::TRIGGERED: return triggered_commands_;
            case TriggerEvent::PRESSED: return pressed_commands_;
            case TriggerEvent::RELEASED: return released_commands_;
        }
        throw std::invalid_argument( "Invalid trigger event!" );
    }


    // +---------------------------+
    // | DEVICE CONTEXT            |
    // +---------------------------+
    DeviceContext::DeviceContext( PlayerController& controller, DeviceInfo const device_info )
        : device_info_{ device_info }
        , controller_ref_{ controller } { }


    auto DeviceContext::get_controller( ) -> PlayerController&
    {
        return controller_ref_;
    }


    auto DeviceContext::get_controller( ) const -> PlayerController const&
    {
        return controller_ref_;
    }


    auto DeviceContext::is_device_suitable( DeviceInfo const device_info ) const -> bool
    {
        switch ( device_info.type )
        {
            case DeviceType::KEYBOARD: return device_info_.type == DeviceType::KEYBOARD;
            case DeviceType::GAMEPAD: return device_info_.type == DeviceType::GAMEPAD && device_info_.id == device_info.id;
            default: return false;
        }
    }


    auto DeviceContext::bind_command( Uid const uid, CommandInfo&& command_info ) -> void
    {
        command_sets_[uid].set( std::move( command_info ) );
    }


    auto DeviceContext::signal_input( InputSnapshot const& input ) -> void
    {
        if ( not command_sets_.contains( input.uid ) )
        {
            return;
        }

        // if the input is not already in the queue, we add it
        InputSnapshot* const queued = meta::find_or_none(
            signaled_inputs_queue_, [&]( InputSnapshot const& snap ) { return snap.uid == input.uid; } );
        if ( not queued )
        {
            signaled_inputs_queue_.push_back( input );
        }
        else
        {
            // if the input is already in the queue, we update its value
            merge_value_to_snapshot( *queued, input.value );
        }
    }


    auto DeviceContext::execute_commands( ) -> void
    {
        while ( not signaled_inputs_queue_.empty( ) )
        {
            auto const& [uid, value, trigger] = signaled_inputs_queue_.front( );
            signaled_inputs_queue_.pop_front( );

            command_sets_.at( uid ).execute( value, trigger );
        }
    }
}
