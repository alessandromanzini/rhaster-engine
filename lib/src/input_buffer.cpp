#include <rst/__input/input_buffer.h>


namespace rst::input
{
    auto input_buffer::trigger( unicode const code, device_id_type device_id ) -> void
    {
        pressed_this_frame_.insert( std::make_pair( code, device_id ) );
    }


    auto input_buffer::release( unicode const code, device_id_type device_id ) -> void
    {
        pressed_this_frame_.erase( std::make_pair( code, device_id ) );
    }


    auto input_buffer::is_pressed( unicode code, device_id_type device_id ) const -> bool
    {
        return pressed_this_frame_.contains( std::make_pair( code, device_id ) );
    }


    auto input_buffer::begin( ) -> range_type::iterator { return pressed_this_frame_.begin( ); }
    auto input_buffer::end( ) -> range_type::iterator { return pressed_this_frame_.end( ); }
    auto input_buffer::begin( ) const -> range_type::const_iterator { return pressed_this_frame_.begin( ); }
    auto input_buffer::end( ) const -> range_type::const_iterator { return pressed_this_frame_.end( ); }
}
