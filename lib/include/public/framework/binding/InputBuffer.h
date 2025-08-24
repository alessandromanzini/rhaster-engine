#ifndef INPUTBUFFER_H
#define INPUTBUFFER_H

// +--------------------------------+
// | PROJECT HEADERS				|
// +--------------------------------+
#include <framework/binding/binding_device.h>
#include <framework/binding/binding_types.h>

// +--------------------------------+
// | STANDARD HEADERS				|
// +--------------------------------+
#include <set>



namespace engine::binding
{
    class InputBuffer final
    {
        using device_id_t   = decltype(DeviceInfo::id);
        using pressed_set_t = std::set<std::pair<UniformBindingCode, device_id_t>>;

    public:
        void trigger( UniformBindingCode code, device_id_t deviceId );
        void release( UniformBindingCode code, device_id_t deviceId );

        [[nodiscard]] bool is_pressed( UniformBindingCode code, device_id_t deviceId ) const;

        [[nodiscard]] const pressed_set_t& get_pressed_this_frame( ) const;

    private:
        pressed_set_t pressed_this_frame_{};

    };

}


#endif //!INPUTBUFFER_H
