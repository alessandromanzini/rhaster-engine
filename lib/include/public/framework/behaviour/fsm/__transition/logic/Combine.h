#ifndef COMBINE_H
#define COMBINE_H

#include <framework/behaviour/fsm/__transition/base_transitions.h>


namespace engine::fsm::logic
{
    // TODO: OPTIMIZE FOR COMPILE-TIME EVALUATION
    template <typename... conditions_t> requires ( std::derived_from<conditions_t, Condition> && ... )
    class Combine : public Condition
    {
    public:
        Combine( );

    protected:
        std::vector<std::unique_ptr<Condition>> conditions_{};

    };

    template <typename ... conditions_t> requires (std::derived_from<conditions_t, Condition> && ...)
    Combine<conditions_t...>::Combine( )
    {
        ( conditions_.push_back( std::make_unique<conditions_t>( ) ), ... );
    }

}


#endif //!COMBINE_H
