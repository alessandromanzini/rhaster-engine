#include <framework/behaviour/fsm/__state_machine/StateStack.h>

#include <framework/behaviour/fsm/__transition/base_transitions.h>


namespace engine::fsm
{
    // +---------------------------+
    // | SINGLE STATE STACK        |
    // +---------------------------+
    void SingleStateStack::trigger_on_enter( Blackboard& blackboard ) const
    {
        state->on_enter( blackboard );
    }


    void SingleStateStack::trigger_on_exit( Blackboard& blackboard ) const
    {
        state->on_exit( blackboard );
    }


    void SingleStateStack::trigger_tick( Blackboard& blackboard ) const
    {
        state->tick( blackboard );
    }


    // +---------------------------+
    // | MULTI STATE STACK         |
    // +---------------------------+
    void MultiStateStack::trigger_on_enter( Blackboard& blackboard ) const
    {
        for ( auto& state : states )
        {
            state->on_enter( blackboard );
        }
    }


    void MultiStateStack::trigger_on_exit( Blackboard& blackboard ) const
    {
        for ( auto& state : states )
        {
            state->on_exit( blackboard );
        }
    }


    void MultiStateStack::trigger_tick( Blackboard& blackboard ) const
    {
        for ( auto& state : states )
        {
            state->tick( blackboard );
        }
    }

}
