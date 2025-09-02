#include <rst/__behaviour/fsm/state_machine/finite_multi_state_machine.h>

#include <rst/__behaviour/fsm/transition/base_transition.h>


namespace rst
{
    FiniteMultiStateMachine::FiniteMultiStateMachine( Blackboard& blackboard ) : blackboard_ref_{ blackboard } { }


    auto FiniteMultiStateMachine::start( Uid const start_state_id ) -> void
    {
        assert( not started_ && "FiniteMultiStateMachine::start: State machine already started!" );
        change_state( start_state_id );
        started_ = true;
    }


    auto FiniteMultiStateMachine::force_transition( Uid const state_id ) -> void
    {
        change_state( state_id );
    }


    auto FiniteMultiStateMachine::tick( ) -> void
    {
        if ( not started_ )
        {
            return;
        }

        // check the transitions map for a TransitionState pair
        if ( auto const it = stacks_.find( current_state_id_ ); it != stacks_.end( ) )
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
        stacks_.at( current_state_id_ ).trigger_tick( blackboard_ref_ );
    }


    auto FiniteMultiStateMachine::create_state_impl( Uid const state_id, std::unique_ptr<fsm::State>&& state ) -> fsm::State&
    {
        return *( stacks_[state_id].states.emplace_back( std::move( state ) ) );
    }


    auto FiniteMultiStateMachine::add_transition_impl( Uid const from, Uid to, std::unique_ptr<fsm::Condition>&& cnd ) -> void
    {
        stacks_[from].transitions.push_back( std::make_pair( std::move( cnd ), to ) );
    }


    auto FiniteMultiStateMachine::change_state( Uid const uid ) -> void
    {
        assert( stacks_.contains( uid ) && "FiniteMultiStateMachine::change_state: Invalid state!" );

        if ( current_state_id_ == uid )
        {
            return;
        }

        // 1. On exit the current state (if any)
        if ( current_state_id_ != uid::NONE )
        {
            stacks_.at( current_state_id_ ).trigger_on_exit( blackboard_ref_ );
        }

        // 2. Set the new state
        current_state_id_ = uid;

        // 3. On enter the new state
        stacks_.at( current_state_id_ ).trigger_on_enter( blackboard_ref_ );
    }
}
