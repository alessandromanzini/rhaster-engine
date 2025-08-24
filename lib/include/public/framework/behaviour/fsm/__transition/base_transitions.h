#ifndef BASE_TRANSITIONS_H
#define BASE_TRANSITIONS_H

#include <framework/resource/data/Blackboard.h>


namespace engine::fsm
{
    class State
    {
    public:
        State( ) = default;
        virtual ~State( ) = default;

        State( const State& )                = delete;
        State( State&& ) noexcept            = delete;
        State& operator=( const State& )     = delete;
        State& operator=( State&& ) noexcept = delete;

        virtual void on_enter( Blackboard& ) { }
        virtual void on_exit( Blackboard& ) { }

        virtual void tick( Blackboard& ) { }

    };

    class Condition
    {
    public:
        Condition( )          = default;
        virtual ~Condition( ) = default;

        Condition( const Condition& )                = delete;
        Condition( Condition&& ) noexcept            = delete;
        Condition& operator=( const Condition& )     = delete;
        Condition& operator=( Condition&& ) noexcept = delete;

        virtual bool evaluate( Blackboard& blackboard ) const = 0;
    };

}


#endif //!BASE_TRANSITIONS_H
