#ifndef RST_TRANSFORM_H
#define RST_TRANSFORM_H

#include <rst/pch.h>


namespace rst
{
    class gameobject;
}

namespace rst
{
    /**
     * This class is a matrix wrapper for 2D transformations.
     */
    class transform final
    {
        using matrix_type = glm::mat3x3;

    public:
        transform( ) = default;
        // ReSharper disable once CppNonExplicitConvertingConstructor
        transform( matrix_type const& matrix );

        [[nodiscard]] auto position( ) const -> glm::vec2;
        [[nodiscard]] auto matrix( ) const -> matrix_type const&;

        [[nodiscard]] auto translate( glm::vec2 translation ) const -> transform;
        [[nodiscard]] auto rotate( float rotation ) const -> transform;
        [[nodiscard]] auto scale( glm::vec2 scale ) const -> transform;

        auto combine( transform const& other ) -> void;
        [[nodiscard]] auto operator*( transform const& other ) const -> transform;

        [[nodiscard]] static auto from_translation( glm::vec2 translation ) -> transform;
        [[nodiscard]] static auto from_rotation( float rotation ) -> transform;
        [[nodiscard]] static auto from_scale( glm::vec2 scale ) -> transform;

    private:
        matrix_type matrix_{ 1.f };
    };

    /**
     * This class streamlines the transform operations on a game object.
     */
    class transform_operator final
    {
    public:
        explicit transform_operator( gameobject& gameobject );

        auto translate( glm::vec2 translation ) const -> void;
        auto rotate( float rotation ) const -> void;

    private:
        gameobject& gameobject_ref_;
    };
}


#endif //!RST_TRANSFORM_H
