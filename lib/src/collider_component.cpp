// #include <rst/temp/framework/component/physics/collider_component.h>
//
// #include <rst/temp/singleton/game_instance.h>
//
//
// namespace rst
// {
//     // +---------------------------+
//     // | COLLIDER COMPONENT        |
//     // +---------------------------+
//     collider_component::collider_component( owner_type& owner, glm::vec2 const position )
//         : component{ owner }
//     {
//         s_colliders_.emplace_back( this );
//         offset_to( position );
//     }
//
//
//     collider_component::~collider_component( )
//     {
//         std::erase( s_colliders_, this );
//     }
//
//
//     auto collider_component::late_tick( ) -> void
//     {
//         // TODO: This should be done in a better way
//         // check for overlaps with colliders that are not in the overlapping set or persisting overlaps
//         if ( s_colliders_.size( ) < 2U )
//         {
//             return;
//         }
//
//         for ( size_t i{}; i < s_colliders_.size( ) - 1U; ++i )
//         {
//             for ( size_t j{ i + 1U }; j < s_colliders_.size( ); ++j )
//             {
//                 auto& collider_a{ *s_colliders_[i] };
//                 auto& collider_b{ *s_colliders_[j] };
//
//                 if ( not collider_a.is_enabled( ) or not collider_b.is_enabled( ) )
//                 {
//                     continue;
//                 }
//
//                 if ( collider_a.overlapping_colliders_.contains( &collider_b ) )
//                 {
//                     collider_a.handle_persist_overlap( collider_b );
//                 }
//                 else
//                 {
//                     collider_a.hit_test( collider_b );
//                 }
//             }
//         }
//     }
//
//
//     auto collider_component::offset_by( glm::vec2 const offset ) -> void
//     {
//         offset_ += offset;
//     }
//
//
//     auto collider_component::offset_to( glm::vec2 const offset ) -> void
//     {
//         offset_ = offset;
//     }
//
//
//     auto collider_component::offset( ) const -> glm::vec2
//     {
//         return offset_;
//     }
//
//
//     auto collider_component::set_enabled( bool const enabled ) -> void
//     {
//         enabled_ = enabled;
//         if ( not enabled )
//         {
//             for ( auto* collider : overlapping_colliders_ )
//             {
//                 clear_overlap( *collider );
//             }
//         }
//     }
//
//
//     auto collider_component::is_enabled( ) const -> bool
//     {
//         return enabled_;
//     }
//
//
//     auto collider_component::position( ) const -> glm::vec2
//     {
//         return offset_ + owner( ).world_transform( ).position( );
//     }
//
//
//     auto collider_component::clear_overlap( collider_component& other ) -> void
//     {
//         handle_end_overlap( other );
//         other.handle_end_overlap( *this );
//     }
//
//
//     auto collider_component::hit_test( collider_component& other, bool const ignore_cache ) -> collision_info
//     {
//         if ( not ignore_cache && overlapping_colliders_.contains( &other ) )
//         {
//             return { .status = collision_status::cached };
//         }
//
//         if ( collision_info info = hit_test_impl( other ); info.has_collided( ) )
//         {
//             if ( not ignore_cache )
//             {
//                 handle_begin_overlap( other, info );
//
//                 // invert the normal to match the other collider's perspective
//                 info.normal = -info.normal;
//                 other.handle_begin_overlap( *this, info );
//             }
//             return info;
//         }
//
//         return { .status = collision_status::none };
//     }
//
//
//     auto collider_component::handle_persist_overlap( collider_component& other ) -> void
//     {
//         if ( not hit_test( other, true ).has_collided( ) )
//         {
//             clear_overlap( other );
//         }
//     }
//
//
//     auto collider_component::handle_begin_overlap( collider_component& other, collision_info const& info ) -> void
//     {
//         on_begin_overlap_dispatcher_.broadcast( *this, other, info );
//         overlapping_colliders_.emplace( &other );
//     }
//
//
//     auto collider_component::handle_end_overlap( collider_component& other ) -> void
//     {
//         on_end_overlap_dispatcher_.broadcast( *this, other );
//         overlapping_colliders_.remove( &other );
//     }
//
// }
