#ifndef RST_TRANSFORM_H
#define RST_TRANSFORM_H

#include <rst/pch.h>


namespace rst
{
    namespace detail
    {
        /**
         * 2D transformation matrix type.
         */
        using matrix_type = glm::mat3x3;


        enum class transform_space : uint8_t
        {
            local, world
        };

        namespace internal
        {
            template <transform_space space>
            concept valid_transform_space = space == transform_space::local || space == transform_space::world;
        }


        template <typename T, transform_space space> requires internal::valid_transform_space<space>
        class transform_operator final
        {
        public:
            explicit transform_operator( T& transform ) : transform_ref_{ transform } { }


            [[nodiscard]] auto matrix( ) const noexcept -> matrix_type const&
            {
                if constexpr ( space == transform_space::local )
                {
                    return transform_ref_.local_matrix_;
                }
                else
                {
                    transform_ref_.refresh_world_transform( );
                    return transform_ref_.world_matrix_;
                }
            }


            auto set_matrix( matrix_type const& mat ) noexcept -> void
            {
                if constexpr ( space == transform_space::local )
                {
                    transform_ref_.set_local_matrix( mat );
                }
                else
                {
                    transform_ref_.set_world_matrix( mat );
                }
            }


            [[nodiscard]] auto location( ) const noexcept -> glm::vec2
            {
                return { matrix( )[2][0], matrix( )[2][1] };
            }


            [[nodiscard]] auto rotation( ) const noexcept -> float
            {
                return std::atan2( matrix( )[0][1], matrix( )[0][0] );
            }


            [[nodiscard]] auto scale( ) const noexcept -> glm::vec2
            {
                return {
                    glm::length( glm::vec2{ matrix( )[0][0], matrix( )[0][1] } ),
                    glm::length( glm::vec2{ matrix( )[1][0], matrix( )[1][1] } )
                };
            }


            auto translate( glm::vec2 const delta ) noexcept -> void
            {
                set_matrix( glm::translate( matrix( ), delta ) );
            }


            auto rotate( float const angle ) noexcept -> void
            {
                set_matrix( glm::rotate( matrix( ), angle ) );
            }


            auto scale( glm::vec2 const factor ) noexcept -> void
            {
                set_matrix( glm::scale( matrix( ), factor ) );
            }

        private:
            T& transform_ref_;
        };
    }


    /**
     * @brief POD transform component for ECS.
     *
     * Stores local transformation matrix and parent relationship.
     * All behavior is handled through free functions to maintain POD status.
     */
    class transform
    {
        friend class detail::transform_operator<transform, detail::transform_space::local>;
        friend class detail::transform_operator<transform, detail::transform_space::world>;

    public:
        transform( ) noexcept = default;
        explicit transform( glm::vec2 location ) noexcept;
        explicit transform( glm::vec2 location, float rotation, glm::vec2 scale ) noexcept;

        ~transform( ) noexcept                               = default;
        transform( transform const& )                        = default;
        transform( transform&& ) noexcept                    = default;
        auto operator=( transform const& ) -> transform&     = default;
        auto operator=( transform&& ) noexcept -> transform& = default;

        auto set_parent( transform* parent, bool keep_world_position = true ) noexcept -> void;
        [[nodiscard]] auto parent( ) const noexcept -> transform*;

        [[nodiscard]] auto local( ) const noexcept -> detail::transform_operator<transform const, detail::transform_space::local>;
        [[nodiscard]] auto local( ) noexcept -> detail::transform_operator<transform, detail::transform_space::local>;
        [[nodiscard]] auto world( ) noexcept -> detail::transform_operator<transform, detail::transform_space::world>;

    private:
        transform* parent_ptr_{ nullptr };
        detail::matrix_type local_matrix_{ 1.f };
        detail::matrix_type world_matrix_{ 1.f };
        bool dirty_{ true };

        auto refresh_world_transform( ) noexcept -> void;
        auto mark_dirty( ) noexcept -> void;

        auto set_world_matrix( detail::matrix_type const& world_mat ) noexcept -> void;
        auto set_local_matrix( detail::matrix_type const& local_mat ) noexcept -> void;

        [[nodiscard]] auto is_child( transform const* maybe_child ) const noexcept -> bool;

        // +--------------------------------+
        // | STATIC METHODS                 |
        // +--------------------------------+
        static auto from_translation( glm::vec2 translation ) noexcept -> detail::matrix_type;
        static auto from_rotation( float rotation ) noexcept -> detail::matrix_type;
        static auto from_scale( glm::vec2 scale ) noexcept -> detail::matrix_type;
        static auto from_trs( glm::vec2 translation, float rotation, glm::vec2 scale ) noexcept -> detail::matrix_type;
    };
}


#endif //!RST_TRANSFORM_H
