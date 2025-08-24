#ifndef FINITESTATEMACHINE_H
#define FINITESTATEMACHINE_H

#include <framework/behaviour/fsm/__state_machine/BaseFiniteStateMachine.h>
#include <framework/behaviour/fsm/__state_machine/StateStack.h>

#include <map>


namespace engine
{
    class FiniteStateMachine final : public BaseFiniteStateMachine
    {
    public:
        explicit FiniteStateMachine( Blackboard& blackboard );

        void start( UID startStateId ) override;
        void force_transition( UID stateId ) override;
        void tick( ) override;

        [[nodiscard]] fsm::State* get_current_state( ) const;
        [[nodiscard]] UID get_current_state_id( ) const;

    protected:
        fsm::State& create_state_impl( UID stateId, std::unique_ptr<fsm::State>&& state ) override;
        void add_transition_impl( UID from, UID to, std::unique_ptr<fsm::Condition>&& condition ) override;

    private:
        Blackboard& blackboard_ref_;
        std::map<UID, fsm::SingleStateStack> stacks_{};

        bool started_{ false };
        UID current_state_id_{ NULL_UID };

        void evaluate_transitions( );
        void change_state( UID uid );


    };

}


#endif //!FINITESTATEMACHINE_H
