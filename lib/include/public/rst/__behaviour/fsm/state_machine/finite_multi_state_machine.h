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

        auto start( Uid start_state_id ) -> void override;
        auto force_transition( Uid state_id ) -> void override;
        auto tick( ) -> void override;

    protected:
        auto create_state_impl( Uid state_id, std::unique_ptr<fsm::State>&& state ) -> fsm::State& override;
        auto add_transition_impl( Uid from, Uid to, std::unique_ptr<fsm::Condition>&& cnd ) -> void override;

    private:
        Blackboard& blackboard_ref_;
        std::map<Uid, fsm::MultiStateStack> stacks_{};

        bool started_{ false };
        Uid current_state_id_{ uid::NONE };

        auto change_state( Uid uid ) -> void;
    };
}


#endif //!RST_FINITE_MULTI_STATE_MACHINE_H
