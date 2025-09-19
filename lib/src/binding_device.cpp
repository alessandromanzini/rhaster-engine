#include <rst/__input/binding_device.h>

#include <rst/meta/algorithm.h>
#include <rst/__input/input_merge.h>
#include <rst/__input/controller/player_controller.h>


namespace rst::input
{
    // +---------------------------+
    // | COMMAND SET               |
    // +---------------------------+
    auto command_set::set( command_info&& info ) -> void
    {
        switch ( info.trigger )
        {
            case trigger::triggered: triggered_commands_.push_back( std::move( info.command ) );
                break;
            case trigger::pressed: pressed_commands_.push_back( std::move( info.command ) );
                break;
            case trigger::released: released_commands_.push_back( std::move( info.command ) );
                break;
            default: assert( false && "Invalid trigger event!" );
        }
    }


    auto command_set::execute( input_value_type const value, trigger const trigger ) const -> void
    {
        for ( auto const& command : select_command_list( trigger ) )
        {
            std::visit(
                [&]<typename T>( T&& fn )
                {
                    using function_sig_t = std::decay_t<T>;
                    using param_t        = std::tuple_element_t<0, typename meta::function_traits<function_sig_t>::params_t>;
                    fn( convert_input_value<param_t>( value ) );
                }, command );
        }
    }


    auto command_set::select_command_list( trigger const trigger ) const -> list_type const&
    {
        switch ( trigger )
        {
            case trigger::triggered: return triggered_commands_;
            case trigger::pressed: return pressed_commands_;
            case trigger::released: return released_commands_;
        }
        throw std::invalid_argument( "Invalid trigger event!" );
    }


    // +---------------------------+
    // | DEVICE CONTEXT            |
    // +---------------------------+
    device_context::device_context( player_controller& controller, input::device_info const device_info )
        : device_info_{ device_info }
        , controller_ref_{ controller } { }


    auto device_context::controller( ) -> player_controller&
    {
        return controller_ref_;
    }


    auto device_context::controller( ) const -> player_controller const&
    {
        return controller_ref_;
    }


    auto device_context::is_device_suitable( input::device_info const device_info ) const -> bool
    {
        switch ( device_info.type )
        {
            case device_type::keyboard: return device_info_.type == device_type::keyboard;
            case device_type::gamepad: return device_info_.type == device_type::gamepad && device_info_.id == device_info.id;
            default: return false;
        }
    }


    auto device_context::bind_command( earmark const mark, command_info&& command_info ) -> void
    {
        command_sets_[mark].set( std::move( command_info ) );
    }


    auto device_context::signal_input( input_reduction const& input ) -> void
    {
        if ( not command_sets_.contains( input.mark ) )
        {
            return;
        }

        // if the input is not already in the queue, we add it
        input_reduction* const queued = meta::find_or_none(
            signaled_inputs_queue_, [&]( input_reduction const& snap ) { return snap.mark == input.mark; } );
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


    auto device_context::execute_commands( ) -> void
    {
        while ( not signaled_inputs_queue_.empty( ) )
        {
            auto const& [mark, value, trigger] = signaled_inputs_queue_.front( );
            signaled_inputs_queue_.pop_front( );

            command_sets_.at( mark ).execute( value, trigger );
        }
    }
}
