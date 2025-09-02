#ifndef RST_STATE_STACK_H
#define RST_STATE_STACK_H

#include <rst/pch.h>


namespace rst
{
    class Blackboard;
}

namespace rst::fsm
{
    class Condition;
    class State;
}

// todo: Review code
namespace rst::fsm
{
    using TTransitionPair = std::pair<std::unique_ptr<Condition>, Uid>;


    struct SingleStateStack final
    {
        std::unique_ptr<State> state{ nullptr };
        std::vector<TTransitionPair> transitions;

        auto trigger_on_enter( Blackboard& blackboard ) const -> void;
        auto trigger_on_exit( Blackboard& blackboard ) const -> void;
        auto trigger_tick( Blackboard& blackboard ) const -> void;
    };


    struct MultiStateStack final
    {
        std::vector<std::unique_ptr<State>> states{};
        std::vector<TTransitionPair> transitions{};

        auto trigger_on_enter( Blackboard& blackboard ) const -> void;
        auto trigger_on_exit( Blackboard& blackboard ) const -> void;
        auto trigger_tick( Blackboard& blackboard ) const -> void;
    };
}


#endif //!RST_STATE_STACK_H
