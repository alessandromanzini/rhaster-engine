#ifndef NAND_H
#define NAND_H


namespace engine::fsm::logic
{
    template <typename... conditions_t>
    class Nand final : public Combine<conditions_t...>
    {
    public:
        bool evaluate( Blackboard& blackboard ) const override;

    };


    template <typename ... conditions_t>
    bool Nand<conditions_t...>::evaluate( Blackboard& blackboard ) const
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

}


#endif //!NAND_H
