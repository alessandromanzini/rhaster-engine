#ifndef NEGATE_H
#define NEGATE_H

#include <framework/behaviour/fsm/__transition/base_transitions.h>


namespace engine::fsm::condition
{
    template <typename condition_t>
    class Negate final : public Condition
    {
    public:
        Negate( );
        bool evaluate( Blackboard& blackboard ) const override;

    private:
        std::unique_ptr<condition_t> condition_ptr_{ nullptr };
    };


    template <typename condition_t>
        Negate<condition_t>::Negate( )
        : condition_ptr_{ std::make_unique<condition_t>(  ) }
    {
    }


    template <typename condition_t>
    bool Negate<condition_t>::evaluate( Blackboard& blackboard ) const
    {
        return not condition_ptr_->evaluate( blackboard );
    }

}


#endif //!NEGATE_H
