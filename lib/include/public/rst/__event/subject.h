#ifndef RST_SUBJECT_H
#define RST_SUBJECT_H

#include <rst/pch.h>


namespace rst
{
    class observer;
}

namespace rst
{
    namespace event
    {
        using broadcast_value_type = std::variant<bool, int, float, double, std::string_view, earmark>;
    }

    class subject final
    {
    public:
        subject( )           = default;
        ~subject( ) noexcept = default;

        subject( subject const& )                        = delete;
        subject( subject&& ) noexcept                    = delete;
        auto operator=( subject const& ) -> subject&     = delete;
        auto operator=( subject&& ) noexcept -> subject& = delete;

        auto add_observer( observer& observer ) -> void;
        auto remove_observer( observer const& observer ) -> void;

        auto broadcast( earmark event, event::broadcast_value_type const& value = {} ) const -> void;

    private:
        observer* head_ptr_{ nullptr };
    };
}


#endif //!RST_SUBJECT_H
