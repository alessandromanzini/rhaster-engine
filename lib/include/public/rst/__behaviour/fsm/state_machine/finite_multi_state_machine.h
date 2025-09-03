#ifndef RST_FINITE_MULTI_STATE_MACHINE_H
#define RST_FINITE_MULTI_STATE_MACHINE_H

#include <rst/pch.h>

#include <rst/__behaviour/fsm/state_machine/base_finite_state_machine.h>
#include <rst/__behaviour/fsm/state_machine/state_stack.h>


// TODO: Implement intermediate states
// TODO: Review code
namespace rst
{
    class blackboard;
}

namespace rst
{
    class finite_multi_state_machine final : public base_finite_state_machine
    {
    public:
        explicit finite_multi_state_machine( blackboard& blackboard );

        auto start( earmark start_state_mark ) -> void override;
        auto force_transition( earmark state_mark ) -> void override;
        auto tick( ) -> void override;

    protected:
        auto create_state_impl( earmark state_mark, std::unique_ptr<fsm::state>&& state ) -> fsm::state& override;
        auto add_transition_impl( earmark from, earmark to, std::unique_ptr<fsm::condition>&& cnd ) -> void override;

    private:
        blackboard& blackboard_ref_;
        std::map<earmark, fsm::multi_state_stack> stacks_{};

        bool started_{ false };
        earmark current_state_mark_{ earmark::null };

        auto change_state( earmark mark ) -> void;
    };
}


#endif //!RST_FINITE_MULTI_STATE_MACHINE_H
