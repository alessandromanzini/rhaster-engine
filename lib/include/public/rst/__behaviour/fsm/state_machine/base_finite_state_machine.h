#ifndef RST_BASE_FINITE_STATE_MACHINE_H
#define RST_BASE_FINITE_STATE_MACHINE_H

#include <rst/pch.h>

#include <rst/__behaviour/fsm/transition/base_transition.h>


namespace rst
{
    class BaseFiniteStateMachine
    {
    public:
        BaseFiniteStateMachine( )          = default;
        virtual ~BaseFiniteStateMachine( ) = default;

        BaseFiniteStateMachine( BaseFiniteStateMachine const& )                        = delete;
        BaseFiniteStateMachine( BaseFiniteStateMachine&& ) noexcept                    = delete;
        auto operator=( BaseFiniteStateMachine const& ) -> BaseFiniteStateMachine&     = delete;
        auto operator=( BaseFiniteStateMachine&& ) noexcept -> BaseFiniteStateMachine& = delete;

        /**
         * Create a state in the state machine linked to the UID. Multiple states can exist with the same UID, and they will all take action.
         * @tparam state_t Type of the state
         * @tparam state_args_t Arguments to be passed to the state constructor
         * @param state_id
         * @param args
         */
        template <typename state_t, typename... state_args_t> requires
            std::derived_from<state_t, fsm::State> && std::constructible_from<state_t, state_args_t...>
        auto create_state( Uid state_id, state_args_t&&... args ) -> state_t&;

        /**
         * Mark a state as an intermediate state. Intermediate states will immediately transition to the next state when the condition is met instead of waiting for next tick.
         * @param state_id
         */
        auto mark_intermediate_state( Uid state_id ) -> void;
        [[nodiscard]] auto is_intermediate( Uid state_id ) const -> bool;

        /**
         * Add a transition between two states. The transition will be triggered when the condition is met.
         * @tparam condition_t
         * @param from
         * @param to
         */
        template <typename condition_t> requires std::derived_from<condition_t, fsm::Condition>
        auto add_transition( Uid from, Uid to ) -> void;

        /**
         * Initialize the state machine with a start state. The state machine will start in this state.
         * @param start_state_id
         */
        virtual auto start( Uid start_state_id ) -> void = 0;

        /**
         * Force a transition to a specific state. This will immediately change the current state to the specified state, regardless of the current state or conditions.
         * @param state_id
         */
        virtual auto force_transition( Uid state_id ) -> void = 0;

        /**
         * Updates the current state. The state machine will check if the current state has any transitions to other states.
         */
        virtual auto tick( ) -> void = 0;

    protected:
        virtual auto create_state_impl( Uid state_id, std::unique_ptr<fsm::State>&& state ) -> fsm::State& = 0;
        virtual auto add_transition_impl( Uid from, Uid to, std::unique_ptr<fsm::Condition>&& condition ) -> void = 0;

    private:
        std::set<Uid> intermediate_states_{};
    };


    template <typename state_t, typename... state_args_t> requires
        std::derived_from<state_t, fsm::State> && std::constructible_from<state_t, state_args_t...>
    auto BaseFiniteStateMachine::create_state( Uid const state_id, state_args_t&&... args ) -> state_t&
    {
        return static_cast<state_t&>( create_state_impl(
            state_id, std::make_unique<state_t>( std::forward<state_args_t>( args )... ) ) );
    }


    inline auto BaseFiniteStateMachine::mark_intermediate_state( Uid const state_id ) -> void
    {
        intermediate_states_.insert( state_id );
    }


    inline auto BaseFiniteStateMachine::is_intermediate( Uid const state_id ) const -> bool
    {
        return intermediate_states_.contains( state_id );
    }


    template <typename condition_t> requires std::derived_from<condition_t, fsm::Condition>
    auto BaseFiniteStateMachine::add_transition( Uid const from, Uid const to ) -> void
    {
        add_transition_impl( from, to, std::make_unique<condition_t>( ) );
    }
}


#endif //!RST_BASE_FINITE_STATE_MACHINE_H
