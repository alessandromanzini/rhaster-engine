// ReSharper disable CppClassCanBeFinal
#ifndef RST_FSM_BASE_TRANSITIONS_H
#define RST_FSM_BASE_TRANSITIONS_H

#include <rst/data_type/blackboard.h>


namespace rst::fsm
{
    class state
    {
    public:
        state( )          = default;
        virtual ~state( ) = default;

        state( state const& )                        = delete;
        state( state&& ) noexcept                    = delete;
        auto operator=( state const& ) -> state&     = delete;
        auto operator=( state&& ) noexcept -> state& = delete;

        virtual auto on_enter( blackboard& ) -> void { }
        virtual auto on_exit( blackboard& ) -> void { }
        virtual auto tick( blackboard& ) -> void { }
    };


    class condition
    {
    public:
        condition( )          = default;
        virtual ~condition( ) = default;

        condition( condition const& )                        = delete;
        condition( condition&& ) noexcept                    = delete;
        auto operator=( condition const& ) -> condition&     = delete;
        auto operator=( condition&& ) noexcept -> condition& = delete;

        virtual auto evaluate( blackboard& blackboard ) const -> bool = 0;
    };
}


#endif //!RST_FSM_BASE_TRANSITIONS_H
