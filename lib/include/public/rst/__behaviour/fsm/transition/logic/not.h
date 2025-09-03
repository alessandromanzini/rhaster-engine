#ifndef RST_FSM_LOGIC_NOT_H
#define RST_FSM_LOGIC_NOT_H

#include <rst/pch.h>

#include <rst/framework/behaviour/fsm/__transition/base_transition.h>


namespace rst::fsm::logic
{
    template <typename TCondition>
    struct Not final : condition
    {
        Not( ) : condition_{} { }
        auto evaluate( blackboard& blackboard ) const -> bool override { return not condition_.evaluate( blackboard ); }

    private:
        TCondition const condition_;
    };
}


#endif //!RST_FSM_LOGIC_NOT_H
