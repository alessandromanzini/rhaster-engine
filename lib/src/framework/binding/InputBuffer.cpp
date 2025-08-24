#include <framework/binding/InputBuffer.h>


namespace engine::binding
{
    void InputBuffer::trigger( const UniformBindingCode code, device_id_t deviceId )
    {
        pressed_this_frame_.insert( std::make_pair( code, deviceId ) );
    }


    void InputBuffer::release( const UniformBindingCode code, device_id_t deviceId )
    {
        pressed_this_frame_.erase( std::make_pair( code, deviceId ) );
    }


    bool InputBuffer::is_pressed( UniformBindingCode code, device_id_t deviceId ) const
    {
        return pressed_this_frame_.contains( std::make_pair( code, deviceId ) );
    }


    const InputBuffer::pressed_set_t& InputBuffer::get_pressed_this_frame( ) const
    {
        return pressed_this_frame_;
    }

}
