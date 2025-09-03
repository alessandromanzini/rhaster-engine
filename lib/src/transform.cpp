#include <rst/__core/transform.h>

#include <rst/__core/gameobject.h>


namespace rst
{
    // +---------------------------+
    // | TRANSFORM                 |
    // +---------------------------+
    transform::transform( glm::mat3x3 const& matrix )
        : matrix_{ matrix } { }


    auto transform::position( ) const -> glm::vec2
    {
        return matrix_[2];
    }


    auto transform::matrix( ) const -> glm::mat3x3 const&
    {
        return matrix_;
    }


    auto transform::translate( glm::vec2 const translation ) const -> transform
    {
        return glm::translate( matrix_, translation );
    }


    auto transform::rotate( float const rotation ) const -> transform
    {
        return glm::rotate( matrix_, rotation );
    }


    auto transform::scale( glm::vec2 const scale ) const -> transform
    {
        return glm::scale( matrix_, scale );
    }


    auto transform::combine( transform const& other ) -> void
    {
        matrix_ = matrix_ * other.matrix_;
    }


    auto transform::operator*( transform const& other ) const -> transform
    {
        transform result{ *this };
        result.combine( other );
        return result;
    }


    auto transform::from_translation( glm::vec2 const translation ) -> transform
    {
        return glm::translate( matrix_type{ 1.0 }, translation );
    }


    auto transform::from_rotation( float const rotation ) -> transform
    {
        return glm::rotate( matrix_type{ 1.0 }, rotation );
    }


    auto transform::from_scale( glm::vec2 const scale ) -> transform
    {
        return glm::scale( matrix_type{ 1.0 }, scale );
    }


    // +---------------------------+
    // | TRANSFORM OPERATOR        |
    // +---------------------------+
    transform_operator::transform_operator( gameobject& gameobject )
        : gameobject_ref_{ gameobject }
    {
    }


    auto transform_operator::translate( glm::vec2 const translation ) const -> void
    {
        auto const translated = gameobject_ref_.local_transform( ) * transform::from_translation( translation );
        gameobject_ref_.set_local_transform( translated );
    }


    auto transform_operator::rotate( float const rotation ) const -> void
    {
        auto const rotated = gameobject_ref_.local_transform( ) * transform::from_rotation( rotation );
        gameobject_ref_.set_local_transform( rotated );
    }

}
