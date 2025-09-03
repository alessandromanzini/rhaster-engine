#include <rst/__behaviour/fsm/state_machine/state_stack.h>

#include <rst/__behaviour/fsm/transition/base_transition.h>


namespace rst::fsm
{
    // +---------------------------+
    // | SINGLE STATE STACK        |
    // +---------------------------+
    auto single_state_stack::trigger_on_enter( blackboard& blackboard ) const -> void
    {
        state->on_enter( blackboard );
    }


    auto single_state_stack::trigger_on_exit( blackboard& blackboard ) const -> void
    {
        state->on_exit( blackboard );
    }


    auto single_state_stack::trigger_tick( blackboard& blackboard ) const -> void
    {
        state->tick( blackboard );
    }


    // +---------------------------+
    // | MULTI STATE STACK         |
    // +---------------------------+
    auto multi_state_stack::trigger_on_enter( blackboard& blackboard ) const -> void
    {
        for ( auto& state : states )
        {
            state->on_enter( blackboard );
        }
    }


    auto multi_state_stack::trigger_on_exit( blackboard& blackboard ) const -> void
    {
        for ( auto& state : states )
        {
            state->on_exit( blackboard );
        }
    }


    auto multi_state_stack::trigger_tick( blackboard& blackboard ) const -> void
    {
        for ( auto& state : states )
        {
            state->tick( blackboard );
        }
    }

}
