// ReSharper disable CppNonExplicitConvertingConstructor
#ifndef TRANSFORM_H
#define TRANSFORM_H

// +--------------------------------+
// | GLM HEADERS					|
// +--------------------------------+
#include <glm.hpp>
#include <gtx/matrix_transform_2d.inl>


namespace engine
{
    class GameObject;
}

namespace engine
{
    /**
     * This class is a matrix wrapper for 2D transformations.
     */
    class Transform final
    {
        using matrix_t = glm::mat3x3;
    public:
        Transform( ) = default;
        Transform( const matrix_t& matrix ); // NOLINT(*-explicit-constructor)

        [[nodiscard]] glm::vec2 get_position( ) const;
        [[nodiscard]] const matrix_t& get_matrix( ) const;

        [[nodiscard]] Transform translate( glm::vec2 translation ) const;
        [[nodiscard]] Transform rotate( float rotation ) const;
        [[nodiscard]] Transform scale( glm::vec2 scale ) const;

        void combine( const Transform& other );
        [[nodiscard]] Transform operator*( const Transform& other ) const;

        [[nodiscard]] static Transform from_translation( glm::vec2 translation );
        [[nodiscard]] static Transform from_rotation( float rotation );
        [[nodiscard]] static Transform from_scale( glm::vec2 scale );

    private:
        matrix_t matrix_{ 1.0 };

    };

    /**
     * This class streamlines the transform operations on a game object.
     */
    class TransformOperator final
    {
    public:
        explicit TransformOperator( GameObject& gameobject );

        void translate( glm::vec2 translation ) const;
        void rotate( float rotation ) const;

    private:
        GameObject& gameobject_ref_;

    };

}

#endif //!TRANSFORM_H
