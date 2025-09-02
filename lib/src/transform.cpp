#include <rst/__core/transform.h>

#include <rst/__core/gameobject.h>


namespace rst
{
    // +---------------------------+
    // | TRANSFORM                 |
    // +---------------------------+
    Transform::Transform( glm::mat3x3 const& matrix )
        : matrix_{ matrix } { }


    auto Transform::get_position( ) const -> glm::vec2
    {
        return matrix_[2];
    }


    auto Transform::get_matrix( ) const -> glm::mat3x3 const&
    {
        return matrix_;
    }


    auto Transform::translate( glm::vec2 const translation ) const -> Transform
    {
        return glm::translate( matrix_, translation );
    }


    auto Transform::rotate( float const rotation ) const -> Transform
    {
        return glm::rotate( matrix_, rotation );
    }


    auto Transform::scale( glm::vec2 const scale ) const -> Transform
    {
        return glm::scale( matrix_, scale );
    }


    auto Transform::combine( Transform const& other ) -> void
    {
        matrix_ = matrix_ * other.matrix_;
    }


    auto Transform::operator*( Transform const& other ) const -> Transform
    {
        Transform result{ *this };
        result.combine( other );
        return result;
    }


    auto Transform::from_translation( glm::vec2 const translation ) -> Transform
    {
        return glm::translate( matrix_t{ 1.0 }, translation );
    }


    auto Transform::from_rotation( float const rotation ) -> Transform
    {
        return glm::rotate( matrix_t{ 1.0 }, rotation );
    }


    auto Transform::from_scale( glm::vec2 const scale ) -> Transform
    {
        return glm::scale( matrix_t{ 1.0 }, scale );
    }


    // +---------------------------+
    // | TRANSFORM OPERATOR        |
    // +---------------------------+
    TransformOperator::TransformOperator( GameObject& gameobject )
        : gameobject_ref_{ gameobject }
    {
    }


    auto TransformOperator::translate( glm::vec2 const translation ) const -> void
    {
        auto const translated = gameobject_ref_.get_local_transform( ) * Transform::from_translation( translation );
        gameobject_ref_.set_local_transform( translated );
    }


    auto TransformOperator::rotate( float const rotation ) const -> void
    {
        auto const rotated = gameobject_ref_.get_local_transform( ) * Transform::from_rotation( rotation );
        gameobject_ref_.set_local_transform( rotated );
    }

}
