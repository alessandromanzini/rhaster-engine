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

            [[nodiscard]] auto from_translation( glm::vec2 translation ) noexcept -> matrix_type;
            [[nodiscard]] auto from_rotation( float rotation ) noexcept -> matrix_type;
            [[nodiscard]] auto from_scale( glm::vec2 scale ) noexcept -> matrix_type;
            [[nodiscard]] auto from_rts( glm::vec2 translation, float rotation, glm::vec2 scale ) noexcept -> matrix_type;

            [[nodiscard]] auto extract_translation( matrix_type const& mat ) noexcept -> glm::vec2;
            [[nodiscard]] auto extract_rotation( matrix_type const& mat ) noexcept -> float;
            [[nodiscard]] auto extract_scale( matrix_type const& mat ) noexcept -> glm::vec2;
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


            [[nodiscard]] auto location( ) const noexcept -> glm::vec2
            {
                return internal::extract_translation( matrix( ) );
            }


            [[nodiscard]] auto rotation( ) const noexcept -> float
            {
                return internal::extract_rotation( matrix( ) );
            }


            [[nodiscard]] auto scale( ) const noexcept -> glm::vec2
            {
                return internal::extract_scale( matrix( ) );
            }


            auto set_matrix( matrix_type const& mat ) noexcept -> void requires ( not std::is_const_v<T> )
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


            auto translate_to( glm::vec2 const delta ) noexcept -> void requires ( not std::is_const_v<T> )
            {
                set_matrix( internal::from_rts( delta, rotation( ), scale( ) ) );
            }


            auto translate( glm::vec2 const delta ) noexcept -> void requires ( not std::is_const_v<T> )
            {
                set_matrix( glm::translate( matrix( ), delta ) );
            }


            auto rotate_to( float const angle ) noexcept -> void requires ( not std::is_const_v<T> )
            {
                set_matrix( internal::from_rts( location( ), angle, scale( ) ) );
            }


            auto rotate( float const angle ) noexcept -> void requires ( not std::is_const_v<T> )
            {
                set_matrix( internal::from_rotation( angle ) * matrix( ) );
            }


            auto scale_to( glm::vec2 const factor ) noexcept -> void requires ( not std::is_const_v<T> )
            {
                set_matrix( internal::from_rts( location( ), rotation( ), factor ) );
            }


            auto scale( glm::vec2 const factor ) noexcept -> void requires ( not std::is_const_v<T> )
            {
                set_matrix( internal::from_scale( factor ) * matrix( ) );
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
        transform* first_child_ptr_{ nullptr };
        transform* next_sibling_ptr_{ nullptr };

        detail::matrix_type local_matrix_{ 1.f };
        detail::matrix_type world_matrix_{ 1.f };
        bool dirty_{ true };

        // +--------------------------------+
        // | DIRTY FLAGGING                 |
        // +--------------------------------+
        auto refresh_world_transform( ) noexcept -> void;
        auto mark_dirty( ) noexcept -> void;

        // +--------------------------------+
        // | MATRIX SETTERS                 |
        // +--------------------------------+
        auto set_world_matrix( detail::matrix_type const& world_mat ) noexcept -> void;
        auto set_local_matrix( detail::matrix_type const& local_mat ) noexcept -> void;

        // +--------------------------------+
        // | PARENTING                      |
        // +--------------------------------+
        [[nodiscard]] auto is_child( transform const* maybe_child ) const noexcept -> bool;
        auto add_child( transform* child ) noexcept -> void;
        auto remove_child( transform const* child ) noexcept -> void;
    };
}


#endif //!RST_TRANSFORM_H
