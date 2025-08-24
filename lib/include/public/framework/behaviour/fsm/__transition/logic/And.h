#ifndef AND_H
#define AND_H

#include <framework/behaviour/fsm/__transition/logic/Combine.h>


namespace engine::fsm::logic
{
    template <typename... conditions_t>
    class And final : public Combine<conditions_t...>
    {
    public:
        bool evaluate( Blackboard& blackboard ) const override;

    };


    template <typename... conditions_t>
    bool And<conditions_t...>::evaluate( Blackboard& blackboard ) const
    {
        for ( auto& condition : Combine<conditions_t...>::conditions_ )
        {
            if ( not condition->evaluate( blackboard ) )
            {
                return false;
            }
        }
        return true;
    }
}


#endif //!AND_H
