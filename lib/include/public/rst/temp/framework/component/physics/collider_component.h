#ifndef RST_COLLIDER_COMPONENT_H
#define RST_COLLIDER_COMPONENT_H

#include <rst/pch.h>

#include <rst/framework/data_type.h>
#include <rst/framework/multicast_delegate.h>
#include <rst/__core/component.h>


namespace rst
{
    // TODO: cleanup structures and add documentation
    enum class CollisionStatus
    {
        NONE, CACHED, FULL
    };


    struct CollisionInfo
    {
        CollisionStatus status{ CollisionStatus::NONE };
        glm::vec2 contact_point{};
        glm::vec2 normal{};
        float depth{};


        auto has_collided( ) const -> bool
        {
            return status != CollisionStatus::NONE;
        }
    };


    // TODO: free overlap on object deletion
    // TODO: store collision cache
    // TODO: Add support for different shapes
    class ColliderComponent : public Component
    {
        Dispatcher<ColliderComponent&, ColliderComponent&, CollisionInfo const&> on_begin_overlap_dispatcher_{};
        Dispatcher<ColliderComponent&, ColliderComponent&> on_end_overlap_dispatcher_{};

    public:
        MulticastDelegate<ColliderComponent&, ColliderComponent&, CollisionInfo const&> on_begin_overlap{
            on_begin_overlap_dispatcher_
        };
        MulticastDelegate<ColliderComponent&, ColliderComponent&> on_end_overlap{ on_end_overlap_dispatcher_ };

        explicit ColliderComponent( owner_t& owner, glm::vec2 position = {} );
        ~ColliderComponent( ) override;

        ColliderComponent( ColliderComponent const& )                        = delete;
        ColliderComponent( ColliderComponent&& ) noexcept                    = delete;
        auto operator=( ColliderComponent const& ) -> ColliderComponent&     = delete;
        auto operator=( ColliderComponent&& ) noexcept -> ColliderComponent& = delete;

        static auto late_tick( ) -> void;

        /**
         * Moves the collider by the given offset.
         * @param offset
         */
        auto offset_by( glm::vec2 offset ) -> void;

        /**
         * Sets the offset of the collider to the given value.
         * @param offset
         */
        auto offset_to( glm::vec2 offset ) -> void;

        [[nodiscard]] auto get_offset( ) const -> glm::vec2;

        auto set_enabled( bool enabled ) -> void;
        [[nodiscard]] auto is_enabled( ) const -> bool;

        /**
         * Returns the position of the collider in world space.
         * @return
         */
        [[nodiscard]] auto get_position( ) const -> glm::vec2;

        auto clear_overlap( ColliderComponent& other ) -> void;

        auto hit_test( ColliderComponent& other, bool ignore_cache = false ) -> CollisionInfo;
        [[nodiscard]] virtual auto get_pivots( ) const -> std::span<glm::vec2 const> = 0;

    protected:
        [[nodiscard]] virtual auto hit_test_impl( ColliderComponent& other ) const -> CollisionInfo = 0;

    private:
        inline static std::vector<ColliderComponent*> s_colliders_{};

        SparseSet<ColliderComponent*> overlapping_colliders_{};
        glm::vec2 offset_{};

        bool enabled_{ true };

        auto handle_persist_overlap( ColliderComponent& other ) -> void;
        auto handle_begin_overlap( ColliderComponent& other, CollisionInfo const& info ) -> void;
        auto handle_end_overlap( ColliderComponent& other ) -> void;
    };
}


#endif //!RST_COLLIDER_COMPONENT_H
