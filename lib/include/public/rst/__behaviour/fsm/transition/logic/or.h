#ifndef RST_FSM_LOGIC_OR_H
#define RST_FSM_LOGIC_OR_H

#include <framework/behaviour/fsm/__transition/logic/Combine.h>


namespace rst::fsm::logic
{
    template <typename... conditions_t>
    struct Or final : Combine<conditions_t...>
    {
        auto evaluate( blackboard& blackboard ) const -> bool override
        {
            for ( auto const& condition : Combine<conditions_t...>::conditions_ )
            {
                if ( condition->evaluate( blackboard ) )
                {
                    return true;
                }
            }
            return false;
        }
    };
}


#endif //!RST_FSM_LOGIC_OR_H
