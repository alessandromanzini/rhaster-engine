#ifndef RST_FSM_LOGIC_NAND_H
#define RST_FSM_LOGIC_NAND_H

#include <rst/pch.h>

#include <rst/framework/behaviour/fsm/__transition/logic/combine.h>


namespace rst::fsm::logic
{
    template <typename... conditions_t>
    struct Nand final : Combine<conditions_t...>
    {
        auto evaluate( Blackboard& blackboard ) const -> bool override
        {
            for ( auto& condition : Combine<conditions_t...>::conditions_ )
            {
                if ( not condition->evaluate( blackboard ) )
                {
                    return true;
                }
            }
            return false;
        }
    };
}


#endif //!RST_FSM_LOGIC_NAND_H
