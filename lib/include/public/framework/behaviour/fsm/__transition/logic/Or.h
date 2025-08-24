#ifndef OR_H
#define OR_H

#include <framework/behaviour/fsm/__transition/logic/Combine.h>


namespace engine::fsm::logic
{
    template <typename... conditions_t>
    class Or final : public Combine<conditions_t...>
    {
    public:
        bool evaluate( Blackboard& blackboard ) const override;

    };


    template <typename ... conditions_t>
    bool Or<conditions_t...>::evaluate( Blackboard& blackboard ) const
    {
        for ( const auto& condition : Combine<conditions_t...>::conditions_ )
        {
            if ( condition->evaluate( blackboard ) )
            {
                return true;
            }
        }
        return false;
    }

}



#endif //!OR_H
