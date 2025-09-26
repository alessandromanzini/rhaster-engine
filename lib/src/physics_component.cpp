// #include <rst/temp/framework/component/physics/physics_component.h>
//
// #include <rst/temp/framework/component/physics/box_collider_component.h>
// #include <rst/temp/singleton/game_instance.h>
// #include <rst/temp/singleton/game_time.h>
// #include <rst/__core/component/transform.h>
//
//
// namespace rst
// {
//     physics_component::physics_component( owner_type& owner )
//         : component{ owner } { }
//
//
//     auto physics_component::fixed_tick( ) -> void
//     {
//         if ( simulate_physics_ )
//         {
//             add_force( { 0.f, GAME_INSTANCE.gravity_coefficient( ) }, true );
//         }
//         glm::vec2 const displacement = velocity_ * GAME_TIME.delta_time( );
//         owner( ).transform_operator( ).translate( displacement );
//     }
//
//
//     auto physics_component::set_simulate_physics( bool const simulate ) -> void
//     {
//         simulate_physics_ = simulate;
//     }
//
//
//     auto physics_component::is_simulating_physics( ) const -> bool
//     {
//         return simulate_physics_;
//     }
//
//
//     auto physics_component::velocity( ) const -> glm::vec2
//     {
//         return velocity_;
//     }
//
//
//     auto physics_component::add_force( glm::vec2 const force, bool const acceleration ) -> void
//     {
//         if ( acceleration )
//         {
//             velocity_ += force * GAME_TIME.delta_time( );
//         }
//         else
//         {
//             velocity_ += force;
//         }
//     }
//
// }
