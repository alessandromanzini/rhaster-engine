// #ifndef RST_COLLIDER_COMPONENT_H
// #define RST_COLLIDER_COMPONENT_H
//
// #include <rst/pch.h>
//
// #include <rst/framework/multicast_delegate.h>
// #include <rst/type/sparse_set.h>
// #include <rst/__core/component.h>
//
//
// namespace rst
// {
//     // TODO: cleanup structures and add documentation
//     enum class collision_status
//     {
//         none, cached, full
//     };
//
//
//     struct collision_info
//     {
//         collision_status status{ collision_status::none };
//         glm::vec2 contact_point{};
//         glm::vec2 normal{};
//         float depth{};
//
//         auto has_collided( ) const -> bool { return status != collision_status::none; }
//     };
//
//
//     // TODO: free overlap on object deletion
//     // TODO: store collision cache
//     // TODO: Add support for different shapes
//     class collider_component : public component
//     {
//         dispatcher<collider_component&, collider_component&, collision_info const&> on_begin_overlap_dispatcher_{};
//         dispatcher<collider_component&, collider_component&> on_end_overlap_dispatcher_{};
//
//     public:
//         multicast_delegate<collider_component&, collider_component&, collision_info const&> on_begin_overlap{
//             on_begin_overlap_dispatcher_
//         };
//         multicast_delegate<collider_component&, collider_component&> on_end_overlap{ on_end_overlap_dispatcher_ };
//
//         explicit collider_component( owner_type& owner, glm::vec2 position = {} );
//         ~collider_component( ) override;
//
//         collider_component( collider_component const& )                        = delete;
//         collider_component( collider_component&& ) noexcept                    = delete;
//         auto operator=( collider_component const& ) -> collider_component&     = delete;
//         auto operator=( collider_component&& ) noexcept -> collider_component& = delete;
//
//         static auto late_tick( ) -> void;
//
//         /**
//          * Moves the collider by the given offset.
//          * @param offset
//          */
//         auto offset_by( glm::vec2 offset ) -> void;
//
//         /**
//          * Sets the offset of the collider to the given value.
//          * @param offset
//          */
//         auto offset_to( glm::vec2 offset ) -> void;
//
//         [[nodiscard]] auto offset( ) const -> glm::vec2;
//
//         auto set_enabled( bool enabled ) -> void;
//         [[nodiscard]] auto is_enabled( ) const -> bool;
//
//         /**
//          * Returns the position of the collider in world space.
//          * @return
//          */
//         [[nodiscard]] auto position( ) const -> glm::vec2;
//
//         auto clear_overlap( collider_component& other ) -> void;
//
//         auto hit_test( collider_component& other, bool ignore_cache = false ) -> collision_info;
//         [[nodiscard]] virtual auto pivots( ) const -> std::span<glm::vec2 const> = 0;
//
//     protected:
//         [[nodiscard]] virtual auto hit_test_impl( collider_component& other ) const -> collision_info = 0;
//
//     private:
//         inline static std::vector<collider_component*> s_colliders_{};
//
//         sparse_set<collider_component*> overlapping_colliders_{};
//         glm::vec2 offset_{};
//
//         bool enabled_{ true };
//
//         auto handle_persist_overlap( collider_component& other ) -> void;
//         auto handle_begin_overlap( collider_component& other, collision_info const& info ) -> void;
//         auto handle_end_overlap( collider_component& other ) -> void;
//     };
// }
//
//
// #endif //!RST_COLLIDER_COMPONENT_H
