#include <framework/behaviour/fsm/FiniteMultiStateMachine.h>

#include <cassert>
#include <stdexcept>


namespace engine
{
    FiniteMultiStateMachine::FiniteMultiStateMachine( Blackboard& blackboard )
        : blackboard_ref_{ blackboard } { }


    void FiniteMultiStateMachine::start( const UID startStateId )
    {
        assert( not started_ && "FiniteMultiStateMachine::start: State machine already started!" );
        change_state( startStateId );
        started_ = true;
    }


    void FiniteMultiStateMachine::force_transition( UID stateId )
    {
        change_state( stateId );
    }


    void FiniteMultiStateMachine::tick( )
    {
        if ( not started_ )
        {
            return;
        }

        // Check the transitions map for a TransitionState pair
        if ( const auto it = stacks_.find( current_state_id_ ); it != stacks_.end( ) )
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

        // Tick the (new or unchanged) state
        stacks_.at( current_state_id_ ).trigger_tick( blackboard_ref_ );
    }


    fsm::State& FiniteMultiStateMachine::create_state_impl( const UID stateId, std::unique_ptr<fsm::State>&& state )
    {
        return *( stacks_[stateId].states.emplace_back( std::move( state ) ) );
    }


    void FiniteMultiStateMachine::add_transition_impl( const UID from, UID to, std::unique_ptr<fsm::Condition>&& condition )
    {
        stacks_[from].transitions.push_back( std::make_pair( std::move( condition ), to ) );
    }


    void FiniteMultiStateMachine::change_state( const UID uid )
    {
        assert( stacks_.contains( uid ) && "FiniteMultiStateMachine::change_state: Invalid state!" );

        if ( current_state_id_ == uid )
        {
            return;
        }

        // 1. On exit the current state (if any)
        if ( current_state_id_ != NULL_UID )
        {
            stacks_.at( current_state_id_ ).trigger_on_exit( blackboard_ref_ );
        }

        // 2. Set the new state
        current_state_id_ = uid;

        // 3. On enter the new state
        stacks_.at( current_state_id_ ).trigger_on_enter( blackboard_ref_ );
    }


}
