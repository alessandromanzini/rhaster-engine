#ifndef RST_TRANSFORM_H
#define RST_TRANSFORM_H

#include <rst/pch.h>


namespace rst
{
    namespace detail
    {
        /**
         * @brief 2D transformation matrix type.
         *
         * Uses a 3x3 matrix to represent 2D transformations with translation,
         * rotation, and scale operations. The third row/column enables
         * homogeneous coordinates for translation operations.
         */
        using matrix_type = glm::mat3x3;


        /**
         * @brief Enumeration defining transformation coordinate spaces.
         *
         * Used to distinguish between local (relative to parent) and world
         * (absolute) coordinate systems for transformation operations.
         */
        enum class transform_space : uint8_t
        {
            local, ///< Local coordinate space relative to parent transform
            world  ///< World coordinate space (absolute positioning)
        };


        namespace internal
        {
            /**
             * @brief Concept constraining valid transformation spaces.
             *
             * @tparam space The transformation space to validate
             *
             * Ensures that only local and world transformation spaces are accepted
             * by template functions requiring a specific coordinate space.
             */
            template <transform_space space>
            concept valid_transform_space = space == transform_space::local || space == transform_space::world;

            /**
             * @brief Creates a translation matrix from a 2D vector.
             *
             * @param translation 2D translation vector
             * @return matrix_type 3x3 transformation matrix representing translation
             *
             * @complexity O(1)
             * @note Generated matrix will have translation in the third column
             */
            [[nodiscard]] auto from_translation( glm::vec2 translation ) noexcept -> matrix_type;

            /**
             * @brief Creates a rotation matrix from an angle in radians.
             *
             * @param rotation Rotation angle in radians
             * @return matrix_type 3x3 transformation matrix representing rotation
             *
             * @complexity O(1)
             * @note Rotation is applied around the origin (0,0)
             */
            [[nodiscard]] auto from_rotation( float rotation ) noexcept -> matrix_type;

            /**
             * @brief Creates a scaling matrix from a 2D scale factor.
             *
             * @param scale 2D scale factor (x, y components)
             * @return matrix_type 3x3 transformation matrix representing scale
             *
             * @complexity O(1)
             * @note Scale is applied around the origin (0,0)
             */
            [[nodiscard]] auto from_scale( glm::vec2 scale ) noexcept -> matrix_type;

            /**
             * @brief Creates a composite transformation matrix from rotation, translation, and scale.
             *
             * @param translation 2D translation vector
             * @param rotation Rotation angle in radians
             * @param scale 2D scale factor
             * @return matrix_type 3x3 transformation matrix combining all transformations
             *
             * @complexity O(1)
             * @note Transformations are applied in TRS order: translation, rotation, then scale
             */
            [[nodiscard]] auto from_rts( glm::vec2 translation, float rotation, glm::vec2 scale ) noexcept -> matrix_type;

            /**
             * @brief Extracts translation vector from transformation matrix.
             *
             * @param mat Transformation matrix to extract from
             * @return glm::vec2 Translation vector (x, y components)
             *
             * @complexity O(1)
             */
            [[nodiscard]] auto extract_translation( matrix_type const& mat ) noexcept -> glm::vec2;

            /**
             * @brief Extracts rotation angle from transformation matrix.
             *
             * @param mat Transformation matrix to extract from
             * @return float Rotation angle in radians
             *
             * @complexity O(1)
             */
            [[nodiscard]] auto extract_rotation( matrix_type const& mat ) noexcept -> float;

            /**
             * @brief Extracts scale factors from transformation matrix.
             *
             * @param mat Transformation matrix to extract from
             * @return glm::vec2 Scale factors (x, y components)
             *
             * @complexity O(1)
             */
            [[nodiscard]] auto extract_scale( matrix_type const& mat ) noexcept -> glm::vec2;
        }


        /**
         * @brief Provides space-specific access to transform operations.
         *
         * @tparam T Transform type (const or non-const)
         * @tparam space Coordinate space (local or world)
         *
         * This class acts as a proxy to access transform operations in either
         * local or world coordinate space. It provides const correct access to 
         * the attached transform.
         *
         * Usage:
         * @code
         * transform t{};
         * auto local_op = t.local();
         * local_op.translate({10.0f, 5.0f});
         *
         * auto world_op = t.world();
         * glm::vec2 world_pos = world_op.location();
         * 
         * t.world().translate_to({});
         * @endcode
         */
        template <typename T, transform_space space> requires internal::valid_transform_space<space>
        class transform_operator final
        {
        public:
            /**
             * @brief Constructs operator with reference to transform.
             *
             * @param transform Reference to the transform to operate on
             */
            explicit transform_operator( T& transform ) : transform_ref_{ transform } { }


            /**
             * @brief Gets the transformation matrix for the specified space.
             *
             * @return matrix_type const& Reference to transformation matrix
             *
             * @complexity O(1) for local space, O(depth) for world space
             * @note World space access may trigger hierarchy traversal for dirty transforms
             */
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


            /**
             * @brief Gets the translation/position in the specified coordinate space.
             *
             * @return glm::vec2 Position vector (x, y components)
             *
             * @complexity O(1) for local space, O(depth) for world space
             */
            [[nodiscard]] auto location( ) const noexcept -> glm::vec2
            {
                return internal::extract_translation( matrix( ) );
            }


            /**
             * @brief Gets the rotation angle in the specified coordinate space.
             *
             * @return float Rotation angle in radians
             *
             * @complexity O(1) for local space, O(depth) for world space
             */
            [[nodiscard]] auto rotation( ) const noexcept -> float
            {
                return internal::extract_rotation( matrix( ) );
            }


            /**
             * @brief Gets the scale factors in the specified coordinate space.
             *
             * @return glm::vec2 Scale factors (x, y components)
             *
             * @complexity O(1) for local space, O(depth) for world space
             */
            [[nodiscard]] auto scale( ) const noexcept -> glm::vec2
            {
                return internal::extract_scale( matrix( ) );
            }


            /**
             * @brief Sets the transformation matrix directly.
             *
             * @param mat New transformation matrix
             *
             * @complexity O(1) for local space, O(children) for world space
             * @note Only available for non-const transforms
             * @note Setting world matrix will mark children as dirty
             */
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


            /**
             * @brief Sets absolute position, preserving rotation and scale.
             *
             * @param delta New position vector
             *
             * @complexity O(1) for local space, O(children) for world space
             * @note Only available for non-const transforms
             */
            auto translate_to( glm::vec2 const delta ) noexcept -> void requires ( not std::is_const_v<T> )
            {
                set_matrix( internal::from_rts( delta, rotation( ), scale( ) ) );
            }


            /**
             * @brief Applies relative translation to current position.
             *
             * @param delta Translation offset to apply
             *
             * @complexity O(1) for local space, O(children) for world space
             * @note Only available for non-const transforms
             */
            auto translate( glm::vec2 const delta ) noexcept -> void requires ( not std::is_const_v<T> )
            {
                set_matrix( glm::translate( matrix( ), delta ) );
            }


            /**
             * @brief Sets absolute rotation, preserving position and scale.
             *
             * @param angle New rotation angle in radians
             *
             * @complexity O(1) for local space, O(children) for world space
             * @note Only available for non-const transforms
             */
            auto rotate_to( float const angle ) noexcept -> void requires ( not std::is_const_v<T> )
            {
                set_matrix( internal::from_rts( location( ), angle, scale( ) ) );
            }


            /**
             * @brief Applies relative rotation to current orientation.
             *
             * @param angle Rotation angle in radians to apply
             *
             * @complexity O(1) for local space, O(children) for world space
             * @note Only available for non-const transforms
             */
            auto rotate( float const angle ) noexcept -> void requires ( not std::is_const_v<T> )
            {
                set_matrix( internal::from_rotation( angle ) * matrix( ) );
            }


            /**
             * @brief Sets absolute scale, preserving position and rotation.
             *
             * @param factor New scale factors (x, y components)
             *
             * @complexity O(1) for local space, O(children) for world space
             * @note Only available for non-const transforms
             */
            auto scale_to( glm::vec2 const factor ) noexcept -> void requires ( not std::is_const_v<T> )
            {
                set_matrix( internal::from_rts( location( ), rotation( ), factor ) );
            }


            /**
             * @brief Applies relative scaling to current scale.
             *
             * @param factor Scale factors to multiply with current scale
             *
             * @complexity O(1) for local space, O(children) for world space
             * @note Only available for non-const transforms
             */
            auto scale( glm::vec2 const factor ) noexcept -> void requires ( not std::is_const_v<T> )
            {
                set_matrix( internal::from_scale( factor ) * matrix( ) );
            }

        private:
            T& transform_ref_; ///< Reference to the transform being operated on
        };
    }


    /**
     * @brief Represents a 2D transform with support for hierarchical parent-child
     * relationships. Uses lazy evaluation with dirty flagging for world space
     * calculations to optimize performance in transform hierarchies.
     *
     * Features:
     * - Local and world coordinate space operations via proxy objects.
     * - Hierarchical parent-child relationships with intrusive linked list.
     * - Lazy world transform computation with dirty flagging.
     * - Cache-friendly POD layout suitable for ECS sparse set storage.
     * - Const-correct access patterns for both read and write operations.
     *
     * Usage:
     * @code
     * // create and manipulate transforms
     * transform parent{};
     * parent.local().translate_to({100.0f, 50.0f});
     *
     * transform child{{10.0f, 20.0f}};
     * child.set_parent(&parent);
     *
     * // access world coordinates (computed from hierarchy)
     * glm::vec2 world_pos = child.world().location(); // {110.0f, 70.0f}
     * @endcode
     */
    class transform
    {
        friend class detail::transform_operator<transform, detail::transform_space::local>;
        friend class detail::transform_operator<transform, detail::transform_space::world>;

    public:
        /**
         * @brief Default constructor creating identity transform.
         *
         * Creates transform at origin with no rotation, unit scale, and no parent.
         */
        transform( ) noexcept = default;

        /**
         * @brief Constructor with initial position.
         *
         * @param location Initial position in local space
         */
        explicit transform( glm::vec2 location ) noexcept;

        /**
         * @brief Constructor with full transformation parameters.
         *
         * @param location Initial position in local space
         * @param rotation Initial rotation angle in radians
         * @param scale Initial scale factors (x, y components)
         */
        explicit transform( glm::vec2 location, float rotation, glm::vec2 scale ) noexcept;

        ~transform( ) noexcept                               = default;
        transform( transform const& )                        = default;
        transform( transform&& ) noexcept                    = default;
        auto operator=( transform const& ) -> transform&     = default;
        auto operator=( transform&& ) noexcept -> transform& = default;

        /**
         * @brief Sets parent transform in the hierarchy.
         *
         * @param parent Pointer to parent transform (nullptr to unparent)
         * @param keep_world_position If true, adjusts local transform to maintain world position
         *
         * @complexity O(c), where c is children count for marking dirty flags
         * @note Creates parent-child relationship using intrusive linked list
         * @note Setting parent marks this transform and all children as dirty
         */
        auto set_parent( transform* parent, bool keep_world_position = true ) noexcept -> void;

        /**
         * @brief Gets current parent transform.
         *
         * @return transform* Pointer to parent transform, or nullptr if no parent
         *
         * @complexity O(1)
         */
        [[nodiscard]] auto parent( ) const noexcept -> transform*;

        /**
         * @brief Gets local space operation proxy (const version).
         *
         * @return transform_operator Proxy for local space operations (read-only)
         *
         * @complexity O(1)
         */
        [[nodiscard]] auto local( ) const noexcept -> detail::transform_operator<transform const, detail::transform_space::local>;

        /**
         * @brief Gets local space operation proxy (mutable version).
         *
         * @return transform_operator Proxy for local space operations (read-write)
         *
         * @complexity O(1)
         */
        [[nodiscard]] auto local( ) noexcept -> detail::transform_operator<transform, detail::transform_space::local>;

        /**
         * @brief Gets world space operation proxy.
         *
         * @return transform_operator Proxy for world space operations (read-write)
         *
         * @complexity O(depth) for dirty transforms, O(1) for clean
         * @note May trigger hierarchy traversal to compute world transform
         */
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
