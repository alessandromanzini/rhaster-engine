#include <rst/__behaviour/fsm/state_machine/finite_state_machine.h>

#include <rst/diagnostic.h>
#include <rst/__behaviour/fsm/transition/base_transition.h>


namespace rst
{
    finite_state_machine::finite_state_machine( blackboard& blackboard )
        : blackboard_ref_{ blackboard } { }


    auto finite_state_machine::start( earmark const start_state_mark ) -> void
    {
        ensure( not started_, "state machine already started!" );
        change_state( start_state_mark );
        started_ = true;
    }


    auto finite_state_machine::force_transition( earmark const state_mark ) -> void
    {
        change_state( state_mark );
    }


    auto finite_state_machine::tick( ) -> void
    {
        if ( not started_ )
        {
            return;
        }

        do
        {
            evaluate_transitions( );

            // tick the (new or unchanged) state
            stacks_.at( current_state_mark_ ).trigger_tick( blackboard_ref_ );

        } while ( is_intermediate( current_state_mark_ ) );
    }


    auto finite_state_machine::current_state( ) const -> fsm::state*
    {
        if ( current_state_mark_ != earmark::null )
        {
            return stacks_.at( current_state_mark_ ).state.get( );
        }
        return nullptr;
    }


    auto finite_state_machine::current_state_mark( ) const -> earmark
    {
        return current_state_mark_;
    }


    auto finite_state_machine::create_state_impl( earmark const state_mark, std::unique_ptr<fsm::state>&& state ) -> fsm::state&
    {
        ensure( not stacks_.contains( state_mark ), "state already exists!" );
        return *( stacks_[state_mark].state = std::move( state ) );
    }


    auto finite_state_machine::add_transition_impl( earmark const from, earmark to, std::unique_ptr<fsm::condition>&& cnd ) -> void
    {
        stacks_[from].transitions.push_back( std::make_pair( std::move( cnd ), to ) );
    }


    auto finite_state_machine::evaluate_transitions( ) -> void
    {
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
    }


    auto finite_state_machine::change_state( earmark const mark ) -> void
    {
        ensure( stacks_.contains( mark ), "invalid state!" );

        if ( current_state_mark_ == mark )
        {
            return;
        }

        // 1. On exit the current state
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
