#include <rst/__behaviour/fsm/state_machine/state_stack.h>

#include <rst/__behaviour/fsm/transition/base_transition.h>


namespace rst::fsm
{
    // +---------------------------+
    // | SINGLE STATE STACK        |
    // +---------------------------+
    auto SingleStateStack::trigger_on_enter( Blackboard& blackboard ) const -> void
    {
        state->on_enter( blackboard );
    }


    auto SingleStateStack::trigger_on_exit( Blackboard& blackboard ) const -> void
    {
        state->on_exit( blackboard );
    }


    auto SingleStateStack::trigger_tick( Blackboard& blackboard ) const -> void
    {
        state->tick( blackboard );
    }


    // +---------------------------+
    // | MULTI STATE STACK         |
    // +---------------------------+
    auto MultiStateStack::trigger_on_enter( Blackboard& blackboard ) const -> void
    {
        for ( auto& state : states )
        {
            state->on_enter( blackboard );
        }
    }


    auto MultiStateStack::trigger_on_exit( Blackboard& blackboard ) const -> void
    {
        for ( auto& state : states )
        {
            state->on_exit( blackboard );
        }
    }


    auto MultiStateStack::trigger_tick( Blackboard& blackboard ) const -> void
    {
        for ( auto& state : states )
        {
            state->tick( blackboard );
        }
    }

}
