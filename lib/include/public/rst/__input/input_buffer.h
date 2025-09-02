#ifndef RST_INPUT_INPUT_BUFFER_H
#define RST_INPUT_INPUT_BUFFER_H

#include <rst/pch.h>

#include <rst/__input/input_type.h>


namespace rst::input
{
    class InputBuffer final
    {
    public:
        using PressedKeysRange = std::set<std::pair<UniformBindingCode, DeviceId>>;

        auto trigger( UniformBindingCode code, DeviceId device_id ) -> void;
        auto release( UniformBindingCode code, DeviceId device_id ) -> void;
        [[nodiscard]] auto is_pressed( UniformBindingCode code, DeviceId device_id ) const -> bool;
        [[nodiscard]] auto pressed_this_frame( ) const -> std::ranges::ref_view<PressedKeysRange const>;

    private:
        PressedKeysRange pressed_this_frame_{};
    };
}


#endif //!RST_INPUT_INPUT_BUFFER_H
