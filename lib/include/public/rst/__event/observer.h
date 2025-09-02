#ifndef RST_OBSERVER_H
#define RST_OBSERVER_H

#include <rst/pch.h>

#include <rst/__event/subject.h>


namespace rst
{
    class Observer
    {
        friend class Subject;

    public:
        Observer( )          = default;
        virtual ~Observer( ) = default;

        Observer( Observer const& )                        = delete;
        Observer( Observer&& ) noexcept                    = delete;
        auto operator=( Observer const& ) -> Observer&     = delete;
        auto operator=( Observer&& ) noexcept -> Observer& = delete;

        virtual auto notify( UID event, event::BroadcastValue value ) -> void = 0;

    private:
        Observer* next_ptr_{ nullptr };
    };
}


#endif //!RST_OBSERVER_H
