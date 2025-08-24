#include <framework/component/physics/PhysicsComponent.h>

#include <framework/Transform.h>
#include <framework/component/physics/BoxColliderComponent.h>
#include <singleton/GameInstance.h>
#include <singleton/GameTime.h>


namespace engine
{
    PhysicsComponent::PhysicsComponent( owner_t& owner )
        : Component{ owner } { }


    void PhysicsComponent::fixed_tick( )
    {
        if ( simulate_physics_ )
        {
            add_force( { 0.f, GAME_INSTANCE.get_gravity_coefficient( ) }, true );
        }
        const glm::vec2 displacement = velocity_ * GAME_TIME.get_delta_time( );
        get_owner( ).get_transform_operator( ).translate( displacement );
    }


    void PhysicsComponent::set_simulate_physics( const bool simulate )
    {
        simulate_physics_ = simulate;
    }


    bool PhysicsComponent::is_simulating_physics( ) const
    {
        return simulate_physics_;
    }


    glm::vec2 PhysicsComponent::get_velocity( ) const
    {
        return velocity_;
    }


    void PhysicsComponent::add_force( const glm::vec2 force, const bool acceleration )
    {
        if ( acceleration )
        {
            velocity_ += force * GAME_TIME.get_delta_time( );
        }
        else
        {
            velocity_ += force;
        }
    }

}
