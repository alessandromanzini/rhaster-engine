#include <rst/__behaviour/fsm/state_machine/finite_multi_state_machine.h>

#include <rst/__behaviour/fsm/transition/base_transition.h>


namespace rst
{
    finite_multi_state_machine::finite_multi_state_machine( blackboard& blackboard ) : blackboard_ref_{ blackboard } { }


    auto finite_multi_state_machine::start( earmark const start_state_id ) -> void
    {
        assert( not started_ && "FiniteMultiStateMachine::start: State machine already started!" );
        change_state( start_state_id );
        started_ = true;
    }


    auto finite_multi_state_machine::force_transition( earmark const state_id ) -> void
    {
        change_state( state_id );
    }


    auto finite_multi_state_machine::tick( ) -> void
    {
        if ( not started_ )
        {
            return;
        }

        // check the transitions map for a TransitionState pair
        if ( auto const it = stacks_.find( current_state_mark_ ); it != stacks_.end( ) )
        {
            for ( auto& stack = it->second; auto& [condition, state] : stack.transitions )
            {
                if ( condition->evaluate( blackboard_ref_ ) )
                {
                    change_state( state );
                    break;
                }
            }
        }

        // tick the (new or unchanged) state
        stacks_.at( current_state_mark_ ).trigger_tick( blackboard_ref_ );
    }


    auto finite_multi_state_machine::create_state_impl( earmark const state_id, std::unique_ptr<fsm::state>&& state ) -> fsm::state&
    {
        return *stacks_[state_id].states.emplace_back( std::move( state ) );
    }


    auto finite_multi_state_machine::add_transition_impl( earmark const from, earmark to, std::unique_ptr<fsm::condition>&& cnd ) -> void
    {
        stacks_[from].transitions.push_back( std::make_pair( std::move( cnd ), to ) );
    }


    auto finite_multi_state_machine::change_state( earmark const mark ) -> void
    {
        assert( stacks_.contains( mark ) && "FiniteMultiStateMachine::change_state: Invalid state!" );

        if ( current_state_mark_ == mark )
        {
            return;
        }

        // 1. On exit the current state (if any)
        if ( current_state_mark_ != earmark::null )
        {
            stacks_.at( current_state_mark_ ).trigger_on_exit( blackboard_ref_ );
        }

        // 2. Set the new state
        current_state_mark_ = mark;

        // 3. On enter the new state
        stacks_.at( current_state_mark_ ).trigger_on_enter( blackboard_ref_ );
    }
}
