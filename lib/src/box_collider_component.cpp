// #include <rst/temp/framework/component/physics/box_collider_component.h>
//
// #include <rst/__core/gameobject.h>
//
//
// namespace rst
// {
//     box_collider_component::box_collider_component( owner_type& owner, glm::vec2 const size, glm::vec2 const position )
//         : collider_component{ owner, position }
//         , pivots_{
//             glm::vec2{ 0.f, 0.f },
//             glm::vec2{ size.x, 0.f },
//             glm::vec2{ size.x, size.y },
//             glm::vec2{ 0.f, size.y }
//         } { }
//
//
//     auto box_collider_component::pivots( ) const -> std::span<glm::vec2 const>
//     {
//         return std::span<glm::vec2 const>{ pivots_ };
//     }
//
//
//     auto box_collider_component::hit_test_impl( collider_component& other ) const -> collision_info
//     {
//         glm::vec2 const self_position  = position( );
//         glm::vec2 const other_position = other.position( );
//
//         glm::vec2 const a_min = self_position + pivots_[0];
//         glm::vec2 const a_max = self_position + pivots_[2];
//
//         auto const other_pivots = other.pivots( );
//         glm::vec2 const b_min   = other_position + other_pivots[0];
//         glm::vec2 const b_max   = other_position + other_pivots[2];
//
//         if ( a_max.x < b_min.x || a_min.x > b_max.x || a_max.y < b_min.y || a_min.y > b_max.y )
//         {
//             return collision_info{ .status = collision_status::none };
//         }
//
//         // calculate overlap in each direction
//         float const overlap_x = std::min( a_max.x, b_max.x ) - std::max( a_min.x, b_min.x );
//         float const overlap_y = std::min( a_max.y, b_max.y ) - std::max( a_min.y, b_min.y );
//
//         glm::vec2 normal{};
//         float depth{};
//
//         if ( overlap_x < overlap_y )
//         {
//             normal = self_position.x < other_position.x ? glm::vec2{ -1.f, 0.f } : glm::vec2{ 1.f, 0.f };
//             depth  = overlap_x;
//         }
//         else
//         {
//             normal = self_position.y < other_position.y ? glm::vec2{ -0.f, -1.f } : glm::vec2{ 0.f, 1.f };
//             depth  = overlap_y;
//         }
//
//         glm::vec2 const contact_min   = max( a_min, b_min );
//         glm::vec2 const contact_max   = min( a_max, b_max );
//         glm::vec2 const contact_point = ( contact_min + contact_max ) * 0.5f;
//
//         return collision_info{
//             .status = collision_status::full,
//             .contact_point = contact_point,
//             .normal = normal,
//             .depth = depth
//         };
//     }
//
// }
