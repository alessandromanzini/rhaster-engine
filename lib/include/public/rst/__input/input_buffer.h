#ifndef RST_INPUT_INPUT_BUFFER_H
#define RST_INPUT_INPUT_BUFFER_H

#include <rst/pch.h>

#include <rst/__input/input_type.h>


namespace rst::input
{
    class input_buffer final
    {
    public:
        using range_type = std::set<std::pair<unicode, device_id_type>>;

        auto trigger( unicode code, device_id_type device_id ) -> void;
        auto release( unicode code, device_id_type device_id ) -> void;
        [[nodiscard]] auto is_pressed( unicode code, device_id_type device_id ) const -> bool;

        auto begin( ) -> range_type::iterator;
        auto end( ) -> range_type::iterator;

        auto begin( ) const -> range_type::const_iterator;
        auto end( ) const -> range_type::const_iterator;

    private:
        range_type pressed_this_frame_{};
    };
}


#endif //!RST_INPUT_INPUT_BUFFER_H
