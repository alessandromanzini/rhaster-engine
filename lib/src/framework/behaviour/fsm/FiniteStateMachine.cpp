#include <framework/behaviour/fsm/FiniteStateMachine.h>

#include <cassert>


namespace engine
{
    FiniteStateMachine::FiniteStateMachine( Blackboard& blackboard )
        : blackboard_ref_{ blackboard } { }


    void FiniteStateMachine::start( const UID startStateId )
    {
        assert( not started_ && "FiniteStateMachine::start: State machine already started!" );
        change_state( startStateId );
        started_ = true;
    }


    void FiniteStateMachine::force_transition( const UID stateId )
    {
        change_state( stateId );
    }


    void FiniteStateMachine::tick( )
    {
        if ( not started_ )
        {
            return;
        }

        do
        {
            evaluate_transitions( );

            // Tick the (new or unchanged) state
            stacks_.at( current_state_id_ ).trigger_tick( blackboard_ref_ );

        } while ( is_intermediate( current_state_id_ ) );
    }


    fsm::State* FiniteStateMachine::get_current_state( ) const
    {
        if ( current_state_id_ != NULL_UID )
        {
            return stacks_.at( current_state_id_ ).state.get( );
        }
        return nullptr;
    }


    UID FiniteStateMachine::get_current_state_id( ) const
    {
        return current_state_id_;
    }


    fsm::State& FiniteStateMachine::create_state_impl( const UID stateId, std::unique_ptr<fsm::State>&& state )
    {
        assert( not stacks_.contains( stateId ) && "FiniteStateMachine::create_state_impl: State already exists!" );
        return *( stacks_[stateId].state = std::move( state ) );
    }


    void FiniteStateMachine::add_transition_impl( const UID from, UID to, std::unique_ptr<fsm::Condition>&& condition )
    {
        stacks_[from].transitions.push_back( std::make_pair( std::move( condition ), to ) );
    }


    void FiniteStateMachine::evaluate_transitions( )
    {
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
    }


    void FiniteStateMachine::change_state( const UID uid )
    {
        assert( stacks_.contains( uid ) && "FiniteStateMachine::change_state: Invalid state!" );

        if ( current_state_id_ == uid )
        {
            return;
        }

        // 1. On exit the current state
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
