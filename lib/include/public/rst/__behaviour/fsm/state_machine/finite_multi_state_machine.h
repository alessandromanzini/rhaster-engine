#ifndef RST_FINITE_MULTI_STATE_MACHINE_H
#define RST_FINITE_MULTI_STATE_MACHINE_H

#include <rst/pch.h>

#include <rst/__behaviour/fsm/state_machine/base_finite_state_machine.h>
#include <rst/__behaviour/fsm/state_machine/state_stack.h>


// TODO: Implement intermediate states
// TODO: Review code
namespace rst
{
    class Blackboard;
}

namespace rst
{
    class FiniteMultiStateMachine final : public BaseFiniteStateMachine
    {
    public:
        explicit FiniteMultiStateMachine( Blackboard& blackboard );

        auto start( UID start_state_id ) -> void override;
        auto force_transition( UID state_id ) -> void override;
        auto tick( ) -> void override;

    protected:
        auto create_state_impl( UID state_id, std::unique_ptr<fsm::State>&& state ) -> fsm::State& override;
        auto add_transition_impl( UID from, UID to, std::unique_ptr<fsm::Condition>&& cnd ) -> void override;

    private:
        Blackboard& blackboard_ref_;
        std::map<UID, fsm::MultiStateStack> stacks_{};

        bool started_{ false };
        UID current_state_id_{ NULL_UID };

        auto change_state( UID uid ) -> void;
    };
}


#endif //!RST_FINITE_MULTI_STATE_MACHINE_H
