#ifndef RST_FINITE_STATE_MACHINE_H
#define RST_FINITE_STATE_MACHINE_H

#include <rst/pch.h>

#include <rst/__behaviour/fsm/state_machine/base_finite_state_machine.h>
#include <rst/__behaviour/fsm/state_machine/state_stack.h>


namespace rst
{
    // todo: Review code
    class FiniteStateMachine final : public BaseFiniteStateMachine
    {
    public:
        explicit FiniteStateMachine( Blackboard& blackboard );

        auto start( UID start_state_id ) -> void override;
        auto force_transition( UID state_id ) -> void override;
        auto tick( ) -> void override;

        [[nodiscard]] auto get_current_state( ) const -> fsm::State*;
        [[nodiscard]] auto get_current_state_id( ) const -> UID;

    protected:
        auto create_state_impl( UID state_id, std::unique_ptr<fsm::State>&& state ) -> fsm::State& override;
        auto add_transition_impl( UID from, UID to, std::unique_ptr<fsm::Condition>&& cnd ) -> void override;

    private:
        Blackboard& blackboard_ref_;
        std::map<UID, fsm::SingleStateStack> stacks_{};

        bool started_{ false };
        UID current_state_id_{ NULL_UID };

        auto evaluate_transitions( ) -> void;
        auto change_state( UID uid ) -> void;
    };
}


#endif //!RST_FINITE_STATE_MACHINE_H
