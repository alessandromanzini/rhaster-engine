#ifndef FINITEMULTISTATEMACHINE_H
#define FINITEMULTISTATEMACHINE_H

#include <framework/behaviour/fsm/__state_machine/BaseFiniteStateMachine.h>
#include <framework/behaviour/fsm/__state_machine/StateStack.h>

#include <map>


// TODO: Implement intermediate states
namespace engine
{
    class Blackboard;
    class FiniteMultiStateMachine final : public BaseFiniteStateMachine
    {
    public:
        explicit FiniteMultiStateMachine( Blackboard& blackboard );

        void start( UID startStateId ) override;
        void force_transition( UID stateId ) override;
        void tick( ) override;

    protected:
        fsm::State& create_state_impl( UID stateId, std::unique_ptr<fsm::State>&& state ) override;
        void add_transition_impl( UID from, UID to, std::unique_ptr<fsm::Condition>&& condition ) override;

    private:
        Blackboard& blackboard_ref_;
        std::map<UID, fsm::MultiStateStack> stacks_{};

        bool started_{ false };
        UID current_state_id_{ NULL_UID };

        void change_state( UID uid );

    };

}


#endif //!FINITEMULTISTATEMACHINE_H
