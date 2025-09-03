#ifndef RST_FSM_LOGIC_NOR_H
#define RST_FSM_LOGIC_NOR_H

#include <rst/pch.h>

#include <rst/framework/behaviour/fsm/__transition/logic/combine.h>


namespace rst::fsm::logic
{
    template <typename... TConditions>
    struct Nor final : Combine<TConditions...>
    {
        auto evaluate( blackboard& blackboard ) const -> bool override
        {
            for ( auto const& condition : Combine<TConditions...>::conditions_ )
            {
                if ( condition->evaluate( blackboard ) )
                {
                    return false;
                }
            }
            return true;
        }
    };
}


#endif //!RST_FSM_LOGIC_NOR_H
