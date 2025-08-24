#ifndef NOR_H
#define NOR_H


namespace engine::fsm::logic
{
    template <typename ... conditions_t>
    class Nor final : public Combine<conditions_t...>
    {
    public:
        bool evaluate( Blackboard& blackboard ) const override;

    };


    template <typename ... conditions_t>
    bool Nor<conditions_t...>::evaluate( Blackboard& blackboard ) const
    {
        for ( const auto& condition : Combine<conditions_t...>::conditions_ )
        {
            if ( condition->evaluate( blackboard ) )
            {
                return false;
            }
        }
        return true;
    }

}


#endif //!NOR_H
