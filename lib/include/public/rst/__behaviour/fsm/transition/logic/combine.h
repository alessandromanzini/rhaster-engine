#ifndef RST_FSM_LOGIC_COMBINE_H
#define RST_FSM_LOGIC_COMBINE_H

#include <rst/pch.h>

#include <rst/framework/behaviour/fsm/__transition/base_transition.h>


namespace rst::fsm::logic
{
    // TODO: OPTIMIZE FOR COMPILE-TIME EVALUATION (tuple concat)
    template <typename... TConditions> requires ( std::derived_from<TConditions, Condition> && ... )
    class Combine : public Condition
    {
    public:
        Combine( )
        {
            ( conditions_.emplace_back( std::make_unique<TConditions>( ) ), ... );
        }

    protected:
        std::vector<std::unique_ptr<Condition>> conditions_{};
    };
}


#endif //!RST_FSM_LOGIC_COMBINE_H
