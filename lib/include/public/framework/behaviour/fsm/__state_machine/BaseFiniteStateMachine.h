#ifndef BASEFINITESTATEMACHINE_H
#define BASEFINITESTATEMACHINE_H

#include <core/UID.h>
#include <framework/behaviour/fsm/transitions.h>

#include <concepts>
#include <set>


namespace engine
{
    class BaseFiniteStateMachine
    {
    public:
        BaseFiniteStateMachine( )          = default;
        virtual ~BaseFiniteStateMachine( ) = default;

        BaseFiniteStateMachine( const BaseFiniteStateMachine& )                = delete;
        BaseFiniteStateMachine( BaseFiniteStateMachine&& ) noexcept            = delete;
        BaseFiniteStateMachine& operator=( const BaseFiniteStateMachine& )     = delete;
        BaseFiniteStateMachine& operator=( BaseFiniteStateMachine&& ) noexcept = delete;

        /**
         * Create a state in the state machine linked to the UID. Multiple states can exist with the same UID, and they will all take action.
         * @tparam state_t Type of the state
         * @tparam state_args_t Arguments to be passed to the state constructor
         * @param stateId
         * @param args
         */
        template <typename state_t, typename... state_args_t>
            requires std::derived_from<state_t, fsm::State>
                     && std::constructible_from<state_t, state_args_t...>
        state_t& create_state( UID stateId, state_args_t&&... args );

        /**
         * Mark a state as an intermediate state. Intermediate states will immediately transition to the next state when the condition is met instead of waiting for next tick.
         * @param stateId
         */
        void mark_intermediate_state( UID stateId );
        bool is_intermediate( UID stateId ) const;

        /**
         * Add a transition between two states. The transition will be triggered when the condition is met.
         * @tparam condition_t
         * @param from
         * @param to
         */
        template <typename condition_t> requires std::derived_from<condition_t, fsm::Condition>
        void add_transition( UID from, UID to );

        /**
         * Initialize the state machine with a start state. The state machine will start in this state.
         * @param startStateId
         */
        virtual void start( UID startStateId ) = 0;

        /**
         * Force a transition to a specific state. This will immediately change the current state to the specified state, regardless of the current state or conditions.
         * @param stateId
         */
        virtual void force_transition( UID stateId ) = 0;

        /**
         * Updates the current state. The state machine will check if the current state has any transitions to other states.
         */
        virtual void tick( ) = 0;

    protected:
        virtual fsm::State& create_state_impl( UID stateId, std::unique_ptr<fsm::State>&& state ) = 0;
        virtual void add_transition_impl( UID from, UID to, std::unique_ptr<fsm::Condition>&& condition ) = 0;

    private:
        std::set<UID> intermediate_states_{};

    };


    inline void BaseFiniteStateMachine::mark_intermediate_state( const UID stateId )
    {
        intermediate_states_.insert( stateId );
    }


    inline bool BaseFiniteStateMachine::is_intermediate( const UID stateId ) const
    {
        return intermediate_states_.contains( stateId );
    }


    template <typename state_t, typename... state_args_t> requires std::derived_from<state_t, fsm::State> && std::
                                                                   constructible_from<state_t, state_args_t...>
    state_t& BaseFiniteStateMachine::create_state( const UID stateId, state_args_t&&... args )
    {
        return static_cast<state_t&>( create_state_impl(
            stateId, std::make_unique<state_t>( std::forward<state_args_t>( args )... ) ) );
    }


    template <typename condition_t> requires std::derived_from<condition_t, fsm::Condition>
    void BaseFiniteStateMachine::add_transition( const UID from, const UID to )
    {
        add_transition_impl( from, to, std::make_unique<condition_t>( ) );
    }

}


#endif //!BASEFINITESTATEMACHINE_H
