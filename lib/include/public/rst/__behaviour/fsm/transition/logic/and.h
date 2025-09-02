#ifndef RST_FSM_LOGIC_AND_H
#define RST_FSM_LOGIC_AND_H

#include <rst/pch.h>

#include <rst/framework/behaviour/fsm/__transition/logic/combine.h>


namespace rst::fsm::logic
{
    template <typename... TConditions>
    struct And final : Combine<TConditions...>
    {
        auto evaluate( Blackboard& blackboard ) const -> bool override
        {
            for ( auto& condition : Combine<TConditions...>::conditions_ )
            {
                if ( not condition->evaluate( blackboard ) )
                {
                    return false;
                }
            }
            return true;
        }
    };
}


#endif //!RST_FSM_LOGIC_AND_H
