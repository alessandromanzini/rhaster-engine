#include <rst/__input/input_buffer.h>


namespace rst::input
{
    auto InputBuffer::trigger( UniformBindingCode const code, DeviceId device_id ) -> void
    {
        pressed_this_frame_.insert( std::make_pair( code, device_id ) );
    }


    auto InputBuffer::release( UniformBindingCode const code, DeviceId device_id ) -> void
    {
        pressed_this_frame_.erase( std::make_pair( code, device_id ) );
    }


    auto InputBuffer::is_pressed( UniformBindingCode code, DeviceId device_id ) const -> bool
    {
        return pressed_this_frame_.contains( std::make_pair( code, device_id ) );
    }


    auto InputBuffer::pressed_this_frame( ) const -> std::ranges::ref_view<PressedKeysRange const>
    {
        return pressed_this_frame_ | std::views::all;
    }
}
