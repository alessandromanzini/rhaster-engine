#ifndef RST_SUBJECT_H
#define RST_SUBJECT_H

#include <rst/pch.h>


namespace rst
{
    class Observer;
}

namespace rst
{
    namespace event
    {
        using BroadcastValue = std::variant<bool, int, float, double, std::string_view, Uid>;
    }

    class Subject final
    {
    public:
        Subject( )           = default;
        ~Subject( ) noexcept = default;

        Subject( Subject const& )                        = delete;
        Subject( Subject&& ) noexcept                    = delete;
        auto operator=( Subject const& ) -> Subject&     = delete;
        auto operator=( Subject&& ) noexcept -> Subject& = delete;

        auto add_observer( Observer& observer ) -> void;
        auto remove_observer( Observer const& observer ) -> void;

        auto broadcast( Uid event, event::BroadcastValue const& value = {} ) const -> void;

    private:
        Observer* head_ptr_{ nullptr };
    };
}


#endif //!RST_SUBJECT_H
