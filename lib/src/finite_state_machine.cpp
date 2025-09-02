#include <rst/__behaviour/fsm/state_machine/finite_state_machine.h>

#include <../include/public/rst/framework/data_type.h>
#include <rst/__behaviour/fsm/transition/base_transition.h>


namespace rst
{
    FiniteStateMachine::FiniteStateMachine( Blackboard& blackboard )
        : blackboard_ref_{ blackboard } { }


    auto FiniteStateMachine::start( Uid const start_state_id ) -> void
    {
        assert( not started_ && "FiniteStateMachine::start: State machine already started!" );
        change_state( start_state_id );
        started_ = true;
    }


    auto FiniteStateMachine::force_transition( Uid const state_id ) -> void
    {
        change_state( state_id );
    }


    auto FiniteStateMachine::tick( ) -> void
    {
        if ( not started_ )
        {
            return;
        }

        do
        {
            evaluate_transitions( );

            // tick the (new or unchanged) state
            stacks_.at( current_state_id_ ).trigger_tick( blackboard_ref_ );

        } while ( is_intermediate( current_state_id_ ) );
    }


    auto FiniteStateMachine::get_current_state( ) const -> fsm::State*
    {
        if ( current_state_id_ != uid::NONE )
        {
            return stacks_.at( current_state_id_ ).state.get( );
        }
        return nullptr;
    }


    auto FiniteStateMachine::get_current_state_id( ) const -> Uid
    {
        return current_state_id_;
    }


    auto FiniteStateMachine::create_state_impl( Uid const state_id, std::unique_ptr<fsm::State>&& state ) -> fsm::State&
    {
        assert( not stacks_.contains( state_id ) && "FiniteStateMachine::create_state_impl: State already exists!" );
        return *( stacks_[state_id].state = std::move( state ) );
    }


    auto FiniteStateMachine::add_transition_impl( Uid const from, Uid to, std::unique_ptr<fsm::Condition>&& condition ) -> void
    {
        stacks_[from].transitions.push_back( std::make_pair( std::move( condition ), to ) );
    }


    auto FiniteStateMachine::evaluate_transitions( ) -> void
    {
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
    }


    auto FiniteStateMachine::change_state( Uid const uid ) -> void
    {
        assert( stacks_.contains( uid ) && "FiniteStateMachine::change_state: Invalid state!" );

        if ( current_state_id_ == uid )
        {
            return;
        }

        // 1. On exit the current state
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
