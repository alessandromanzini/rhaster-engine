// ReSharper disable CppClassCanBeFinal
#ifndef RST_FSM_BASE_TRANSITIONS_H
#define RST_FSM_BASE_TRANSITIONS_H

#include <rst/framework/data_type.h>


namespace rst::fsm
{
    class State
    {
    public:
        State( )          = default;
        virtual ~State( ) = default;

        State( State const& )                        = delete;
        State( State&& ) noexcept                    = delete;
        auto operator=( State const& ) -> State&     = delete;
        auto operator=( State&& ) noexcept -> State& = delete;

        virtual auto on_enter( Blackboard& ) -> void { }
        virtual auto on_exit( Blackboard& ) -> void { }
        virtual auto tick( Blackboard& ) -> void { }
    };


    class Condition
    {
    public:
        Condition( )          = default;
        virtual ~Condition( ) = default;

        Condition( Condition const& )                        = delete;
        Condition( Condition&& ) noexcept                    = delete;
        auto operator=( Condition const& ) -> Condition&     = delete;
        auto operator=( Condition&& ) noexcept -> Condition& = delete;

        virtual auto evaluate( Blackboard& blackboard ) const -> bool = 0;
    };
}


#endif //!RST_FSM_BASE_TRANSITIONS_H
