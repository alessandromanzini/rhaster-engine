#include <rst/temp/framework/component/physics/physics_component.h>

#include <rst/temp/framework/component/physics/box_collider_component.h>
#include <rst/temp/singleton/game_instance.h>
#include <rst/temp/singleton/game_time.h>
#include <rst/__core/transform.h>


namespace rst
{
    PhysicsComponent::PhysicsComponent( owner_t& owner )
        : Component{ owner } { }


    auto PhysicsComponent::fixed_tick( ) -> void
    {
        if ( simulate_physics_ )
        {
            add_force( { 0.f, GAME_INSTANCE.get_gravity_coefficient( ) }, true );
        }
        glm::vec2 const displacement = velocity_ * GAME_TIME.get_delta_time( );
        get_owner( ).get_transform_operator( ).translate( displacement );
    }


    auto PhysicsComponent::set_simulate_physics( bool const simulate ) -> void
    {
        simulate_physics_ = simulate;
    }


    auto PhysicsComponent::is_simulating_physics( ) const -> bool
    {
        return simulate_physics_;
    }


    auto PhysicsComponent::get_velocity( ) const -> glm::vec2
    {
        return velocity_;
    }


    auto PhysicsComponent::add_force( glm::vec2 const force, bool const acceleration ) -> void
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
