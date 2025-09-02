#ifndef RST_TRANSFORM_H
#define RST_TRANSFORM_H

#include <rst/pch.h>


namespace rst
{
    class GameObject;
}

namespace rst
{
    /**
     * This class is a matrix wrapper for 2D transformations.
     */
    class Transform final
    {
        using matrix_t = glm::mat3x3;

    public:
        Transform( ) = default;
        // ReSharper disable once CppNonExplicitConvertingConstructor
        Transform( matrix_t const& matrix );

        [[nodiscard]] auto get_position( ) const -> glm::vec2;
        [[nodiscard]] auto get_matrix( ) const -> matrix_t const&;

        [[nodiscard]] auto translate( glm::vec2 translation ) const -> Transform;
        [[nodiscard]] auto rotate( float rotation ) const -> Transform;
        [[nodiscard]] auto scale( glm::vec2 scale ) const -> Transform;

        auto combine( Transform const& other ) -> void;
        [[nodiscard]] auto operator*( Transform const& other ) const -> Transform;

        [[nodiscard]] static auto from_translation( glm::vec2 translation ) -> Transform;
        [[nodiscard]] static auto from_rotation( float rotation ) -> Transform;
        [[nodiscard]] static auto from_scale( glm::vec2 scale ) -> Transform;

    private:
        matrix_t matrix_{ 1.f };
    };

    /**
     * This class streamlines the transform operations on a game object.
     */
    class TransformOperator final
    {
    public:
        explicit TransformOperator( GameObject& gameobject );

        auto translate( glm::vec2 translation ) const -> void;
        auto rotate( float rotation ) const -> void;

    private:
        GameObject& gameobject_ref_;
    };
}


#endif //!RST_TRANSFORM_H
