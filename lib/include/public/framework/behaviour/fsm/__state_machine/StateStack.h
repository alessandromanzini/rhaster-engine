#ifndef STATESTACK_H
#define STATESTACK_H

#include <core/UID.h>

#include <memory>
#include <utility>


namespace engine
{
    class Blackboard;
}

namespace engine::fsm
{
    class Condition;
    class State;

    using transition_pair_t = std::pair<std::unique_ptr<Condition>, UID>;


    struct SingleStateStack final
    {
        std::unique_ptr<State> state{ nullptr };
        std::vector<transition_pair_t> transitions{};

        void trigger_on_enter( Blackboard& blackboard ) const;
        void trigger_on_exit( Blackboard& blackboard ) const;
        void trigger_tick( Blackboard& blackboard ) const;
        void trigger_render( ) const;
    };


    struct MultiStateStack final
    {
        std::vector<std::unique_ptr<State>> states{};
        std::vector<transition_pair_t> transitions{};

        void trigger_on_enter( Blackboard& blackboard ) const;
        void trigger_on_exit( Blackboard& blackboard ) const;
        void trigger_tick( Blackboard& blackboard ) const;
        void trigger_render( ) const;
    };

}


#endif //!STATESTACK_H
