#ifndef COLLIDERCOMPONENT_H
#define COLLIDERCOMPONENT_H

#include <framework/component/Component.h>
#include <framework/event/MulticastDelegate.h>
#include <framework/resource/data/SparseSet.h>

#include <vec2.hpp>

#include <span>


namespace engine
{
    enum class CollisionStatus
    {
        NONE,
        CACHED,
        FULL
    };


    struct CollisionInfo
    {
        CollisionStatus status{ CollisionStatus::NONE };
        glm::vec2 contact_point{};
        glm::vec2 normal{};
        float depth{};


        bool has_collided( ) const
        {
            return status != CollisionStatus::NONE;
        }
    };


    // TODO: free overlap on object deletion
    // TODO: store collision cache
    // TODO: Add support for different shapes
    class ColliderComponent : public Component
    {
        Dispatcher<ColliderComponent&, ColliderComponent&, const CollisionInfo&> on_begin_overlap_dispatcher_{};
        Dispatcher<ColliderComponent&, ColliderComponent&> on_end_overlap_dispatcher_{};

    public:
        MulticastDelegate<ColliderComponent&, ColliderComponent&, const CollisionInfo&> on_begin_overlap{
            on_begin_overlap_dispatcher_
        };
        MulticastDelegate<ColliderComponent&, ColliderComponent&> on_end_overlap{ on_end_overlap_dispatcher_ };

        explicit ColliderComponent( owner_t& owner, glm::vec2 position = {} );
        ~ColliderComponent( ) override;

        ColliderComponent( const ColliderComponent& )                = delete;
        ColliderComponent( ColliderComponent&& ) noexcept            = delete;
        ColliderComponent& operator=( const ColliderComponent& )     = delete;
        ColliderComponent& operator=( ColliderComponent&& ) noexcept = delete;

        static void late_tick( );

        /**
         * Moves the collider by the given offset.
         * @param offset
         */
        void offset_by( glm::vec2 offset );

        /**
         * Sets the offset of the collider to the given value.
         * @param offset
         */
        void offset_to( glm::vec2 offset );

        [[nodiscard]] glm::vec2 get_offset( ) const;

        void set_enabled( bool enabled );
        [[nodiscard]] bool is_enabled( ) const;

        /**
         * Returns the position of the collider in world space.
         * @return
         */
        [[nodiscard]] glm::vec2 get_position( ) const;

        void clear_overlap( ColliderComponent& other );

        CollisionInfo hit_test( ColliderComponent& other, bool ignoreCache = false );
        [[nodiscard]] virtual std::span<const glm::vec2> get_pivots( ) const = 0;

    protected:
        [[nodiscard]] virtual CollisionInfo hit_test_impl( ColliderComponent& other ) const = 0;

    private:
        inline static std::vector<ColliderComponent*> s_colliders_{};

        SparseSet<ColliderComponent*> overlapping_colliders_{};
        glm::vec2 offset_{};

        bool enabled_{ true };

        void handle_persist_overlap( ColliderComponent& other );
        void handle_begin_overlap( ColliderComponent& other, const CollisionInfo& info );
        void handle_end_overlap( ColliderComponent& other );

    };

}


#endif //!COLLIDERCOMPONENT_H
