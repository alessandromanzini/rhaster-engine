#ifndef RST_BASE_FINITE_STATE_MACHINE_H
#define RST_BASE_FINITE_STATE_MACHINE_H

#include <rst/pch.h>

#include <rst/__behaviour/fsm/transition/base_transition.h>


namespace rst
{
    class base_finite_state_machine
    {
    public:
        base_finite_state_machine( )          = default;
        virtual ~base_finite_state_machine( ) = default;

        base_finite_state_machine( base_finite_state_machine const& )                        = delete;
        base_finite_state_machine( base_finite_state_machine&& ) noexcept                    = delete;
        auto operator=( base_finite_state_machine const& ) -> base_finite_state_machine&     = delete;
        auto operator=( base_finite_state_machine&& ) noexcept -> base_finite_state_machine& = delete;

        /**
         * Create a state in the state machine linked to the UID. Multiple states can exist with the same UID, and they will all take action.
         * @tparam TState Type of the state
         * @tparam TStateArgs Arguments to be passed to the state constructor
         * @param state_mark
         * @param args
         */
        template <typename TState, typename... TStateArgs> requires
            std::derived_from<TState, fsm::state> && std::constructible_from<TState, TStateArgs...>
        auto create_state( earmark state_mark, TStateArgs&&... args ) -> TState&;

        /**
         * Mark a state as an intermediate state. Intermediate states will immediately transition to the next state when the condition is met instead of waiting for next tick.
         * @param state_mark
         */
        auto mark_intermediate_state( earmark state_mark ) -> void;
        [[nodiscard]] auto is_intermediate( earmark state_mark ) const -> bool;

        /**
         * Add a transition between two states. The transition will be triggered when the condition is met.
         * @tparam TCondition
         * @param from
         * @param to
         */
        template <typename TCondition> requires std::derived_from<TCondition, fsm::condition>
        auto add_transition( earmark from, earmark to ) -> void;

        /**
         * Initialize the state machine with a start state. The state machine will start in this state.
         * @param start_state_mark
         */
        virtual auto start( earmark start_state_mark ) -> void = 0;

        /**
         * Force a transition to a specific state. This will immediately change the current state to the specified state, regardless of the current state or conditions.
         * @param state_mark
         */
        virtual auto force_transition( earmark state_mark ) -> void = 0;

        /**
         * Updates the current state. The state machine will check if the current state has any transitions to other states.
         */
        virtual auto tick( ) -> void = 0;

    protected:
        virtual auto create_state_impl( earmark state_mark, std::unique_ptr<fsm::state>&& state ) -> fsm::state& = 0;
        virtual auto add_transition_impl( earmark from, earmark to, std::unique_ptr<fsm::condition>&& condition ) -> void = 0;

    private:
        std::set<earmark> intermediate_states_{};
    };


    template <typename TState, typename... TStateArgs> requires
        std::derived_from<TState, fsm::state> && std::constructible_from<TState, TStateArgs...>
    auto base_finite_state_machine::create_state( earmark const state_mark, TStateArgs&&... args ) -> TState&
    {
        return static_cast<TState&>( create_state_impl(
            state_mark, std::make_unique<TState>( std::forward<TStateArgs>( args )... ) ) );
    }


    inline auto base_finite_state_machine::mark_intermediate_state( earmark const state_mark ) -> void
    {
        intermediate_states_.insert( state_mark );
    }


    inline auto base_finite_state_machine::is_intermediate( earmark const state_mark ) const -> bool
    {
        return intermediate_states_.contains( state_mark );
    }


    template <typename TCondition> requires std::derived_from<TCondition, fsm::condition>
    auto base_finite_state_machine::add_transition( earmark const from, earmark const to ) -> void
    {
        add_transition_impl( from, to, std::make_unique<TCondition>( ) );
    }
}


#endif //!RST_BASE_FINITE_STATE_MACHINE_H
