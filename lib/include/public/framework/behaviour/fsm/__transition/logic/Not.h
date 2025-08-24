#ifndef NOT_H
#define NOT_H


namespace engine::fsm::logic
{
    template <typename condition_t>
    class Not final : public Condition
    {
    public:
        Not();
        bool evaluate( Blackboard& blackboard ) const override;

    private:
        std::unique_ptr<condition_t> condition_ptr_{ nullptr };

    };


    template <typename condition_t>
    Not<condition_t>::Not( )
        : condition_ptr_{ std::make_unique<condition_t>( ) }
    {
    }


    template <typename condition_t>
    bool Not<condition_t>::evaluate( Blackboard& blackboard ) const
    {
        return not condition_ptr_->evaluate( blackboard );
    }

}


#endif //!NOT_H
