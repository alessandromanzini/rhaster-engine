#include <framework/Transform.h>

// +--------------------------------+
// | PROJECT HEADERS				|
// +--------------------------------+
#include <framework/GameObject.h>


namespace engine
{
    // +---------------------------+
    // | TRANSFORM                 |
    // +---------------------------+
    Transform::Transform( const glm::mat3x3& matrix )
        : matrix_{ matrix } { }


    glm::vec2 Transform::get_position( ) const
    {
        return matrix_[2];
    }


    const glm::mat3x3& Transform::get_matrix( ) const
    {
        return matrix_;
    }


    Transform Transform::translate( const glm::vec2 translation ) const
    {
        return glm::translate( matrix_, translation );
    }


    Transform Transform::rotate( const float rotation ) const
    {
        return glm::rotate( matrix_, rotation );
    }


    Transform Transform::scale( const glm::vec2 scale ) const
    {
        return glm::scale( matrix_, scale );
    }


    void Transform::combine( const Transform& other )
    {
        matrix_ = matrix_ * other.matrix_;
    }


    Transform Transform::operator*( const Transform& other ) const
    {
        Transform result{ *this };
        result.combine( other );
        return result;
    }


    Transform Transform::from_translation( const glm::vec2 translation )
    {
        return glm::translate( matrix_t{ 1.0 }, translation );
    }


    Transform Transform::from_rotation( const float rotation )
    {
        return glm::rotate( matrix_t{ 1.0 }, rotation );
    }


    Transform Transform::from_scale( const glm::vec2 scale )
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


    void TransformOperator::translate( const glm::vec2 translation ) const
    {
        const auto translated = gameobject_ref_.get_local_transform( ) * Transform::from_translation( translation );
        gameobject_ref_.set_local_transform( translated );
    }


    void TransformOperator::rotate( const float rotation ) const
    {
        const auto rotated = gameobject_ref_.get_local_transform( ) * Transform::from_rotation( rotation );
        gameobject_ref_.set_local_transform( rotated );
    }

}
