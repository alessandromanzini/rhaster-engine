#ifndef RST_STATE_STACK_H
#define RST_STATE_STACK_H

#include <rst/pch.h>


namespace rst
{
    class blackboard;
}

namespace rst::fsm
{
    class condition;
    class state;
}

// todo: Review code
namespace rst::fsm
{
    using transition_pair_t = std::pair<std::unique_ptr<condition>, earmark>;


    struct single_state_stack final
    {
        std::unique_ptr<state> state{ nullptr };
        std::vector<transition_pair_t> transitions;

        auto trigger_on_enter( blackboard& blackboard ) const -> void;
        auto trigger_on_exit( blackboard& blackboard ) const -> void;
        auto trigger_tick( blackboard& blackboard ) const -> void;
    };


    struct multi_state_stack final
    {
        std::vector<std::unique_ptr<state>> states{};
        std::vector<transition_pair_t> transitions{};

        auto trigger_on_enter( blackboard& blackboard ) const -> void;
        auto trigger_on_exit( blackboard& blackboard ) const -> void;
        auto trigger_tick( blackboard& blackboard ) const -> void;
    };
}


#endif //!RST_STATE_STACK_H
