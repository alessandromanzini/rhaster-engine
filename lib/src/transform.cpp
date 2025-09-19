#include <rst/__core/component/transform.h>


namespace rst
{
    transform::transform( glm::vec2 const location ) noexcept : local_matrix_{ from_translation( location ) } { }


    transform::transform( glm::vec2 const location, float const rotation, glm::vec2 const scale ) noexcept
        : local_matrix_{ from_trs( location, rotation, scale ) } { }


    auto transform::set_parent( transform* const parent, bool const keep_world_position ) noexcept -> void
    {
        // parent validation
        if ( is_child( parent ) || parent == this || parent == parent_ptr_ )
        {
            return;
        }

        if ( parent == nullptr )
        {
            // if becoming root object, local transform becomes world space transform
            set_local_matrix( world_matrix_ );
        }
        else
        {
            // if parent is provided, we need to adjust the local transform to keep the world position and set it dirty to
            // recalculate the world transform.
            if ( keep_world_position )
            {
                // TODO: test if rotation is canceled
                glm::vec2 const translation = world( ).location( ) - parent->world( ).location( );
                set_local_matrix( from_translation( translation ) );
            }
            mark_dirty( );
        }

        // parenting logic
        // if ( parent_ptr_ != nullptr ) { parent_ptr_->remove_child( this ); }
        // if ( parent != nullptr ) { parent->add_child( this ); }
        parent_ptr_ = parent;
    }


    auto transform::parent( ) const noexcept -> transform*
    {
        return parent_ptr_;
    }


    auto transform::local( ) const noexcept -> detail::transform_operator<transform const, detail::transform_space::local>
    {
        return detail::transform_operator<transform const, detail::transform_space::local>{ *this };
    }


    auto transform::local( ) noexcept -> detail::transform_operator<transform, detail::transform_space::local>
    {
        return detail::transform_operator<transform, detail::transform_space::local>{ *this };
    }


    auto transform::world( ) noexcept -> detail::transform_operator<transform, detail::transform_space::world>
    {
        return detail::transform_operator<transform, detail::transform_space::world>{ *this };
    }


    auto transform::refresh_world_transform( ) noexcept -> void
    {
        // 1. update only if dirty
        if ( not dirty_ ) { return; }

        // 2. update parent first
        if ( parent_ptr_ )
        {
            parent_ptr_->refresh_world_transform( );
            world_matrix_ = parent_ptr_->world_matrix_ * local_matrix_;
        }
        else
        {
            world_matrix_ = local_matrix_;
        }

        // 3. reset dirty flag
        dirty_ = false;
    }


    auto transform::mark_dirty( ) noexcept -> void
    {
        dirty_ = true;
        // TODO: Propagate to children when hierarchy system is implemented
    }


    auto transform::set_world_matrix( detail::matrix_type const& world_mat ) noexcept -> void
    {
        // 1. update world matrix directly
        world_matrix_ = world_mat;

        // 2. if not root node, update local transform to match new world transform ...
        if ( parent_ptr_ )
        {
            parent_ptr_->refresh_world_transform( );
            local_matrix_ = glm::inverse( parent_ptr_->world_matrix_ ) * world_mat;
        }
        else
        {
            // ... else just set local to world
            local_matrix_ = world_mat;
        }

        // 3. reset dirty flag
        dirty_ = false;
    }


    auto transform::set_local_matrix( detail::matrix_type const& local_mat ) noexcept -> void
    {
        local_matrix_ = local_mat;
        mark_dirty( );
    }


    auto transform::is_child( transform const* /* maybe_child */ ) const noexcept -> bool
    {
        // todo: implement
        return false;
    }


    auto transform::from_translation( glm::vec2 const translation ) noexcept -> detail::matrix_type
    {
        return glm::translate( detail::matrix_type{ 1.0 }, translation );
    }


    auto transform::from_rotation( float const rotation ) noexcept -> detail::matrix_type
    {
        return glm::rotate( detail::matrix_type{ 1.0 }, rotation );
    }


    auto transform::from_scale( glm::vec2 const scale ) noexcept -> detail::matrix_type
    {
        return glm::scale( detail::matrix_type{ 1.0 }, scale );
    }


    auto transform::from_trs(
        glm::vec2 const translation, float const rotation, glm::vec2 const scale ) noexcept -> detail::matrix_type
    {
        float const c = std::cosf( rotation );
        float const s = std::sinf( rotation );
        return detail::matrix_type{
            scale.x * c, scale.x * s, 0.0f,
            -scale.y * s, scale.y * c, 0.0f,
            translation.x, translation.y, 1.0f
        };
    }
}
