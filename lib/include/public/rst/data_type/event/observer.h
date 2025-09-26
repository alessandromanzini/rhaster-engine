#ifndef RST_OBSERVER_H
#define RST_OBSERVER_H

#include <rst/pch.h>

#include <rst/data_type/event/subject.h>


namespace rst
{
    class observer
    {
        friend class subject;

    public:
        observer( )          = default;
        virtual ~observer( ) = default;

        observer( observer const& )                        = delete;
        observer( observer&& ) noexcept                    = delete;
        auto operator=( observer const& ) -> observer&     = delete;
        auto operator=( observer&& ) noexcept -> observer& = delete;

        virtual auto notify( earmark event, event::broadcast_value_type value ) -> void = 0;

    private:
        observer* next_ptr_{ nullptr };
    };
}


#endif //!RST_OBSERVER_H
