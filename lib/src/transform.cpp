#include <rst/__core/component/transform.h>


namespace rst
{
    // +--------------------------------+
    // | ITERNAL HELPERS                |
    // +--------------------------------+
    auto detail::internal::from_translation( glm::vec2 const translation ) noexcept -> matrix_type
    {
        return glm::translate( matrix_type{ 1.0 }, translation );
    }


    auto detail::internal::from_rotation( float const rotation ) noexcept -> matrix_type
    {
        return glm::rotate( matrix_type{ 1.0 }, rotation );
    }


    auto detail::internal::from_scale( glm::vec2 const scale ) noexcept -> matrix_type
    {
        return glm::scale( matrix_type{ 1.0 }, scale );
    }


    auto detail::internal::from_rts(
        glm::vec2 const translation, float const rotation, glm::vec2 const scale ) noexcept -> matrix_type
    {
        return from_rotation( rotation ) * from_translation( translation ) * from_scale( scale );
    }


    auto detail::internal::extract_translation( matrix_type const& mat ) noexcept -> glm::vec2
    {
        return { mat[2][0], mat[2][1] };
    }


    auto detail::internal::extract_rotation( matrix_type const& mat ) noexcept -> float
    {
        return std::atan2( mat[0][1], mat[0][0] );
    }


    auto detail::internal::extract_scale( matrix_type const& mat ) noexcept -> glm::vec2
    {
        return { glm::length( glm::vec2{ mat[0][0], mat[0][1] } ), glm::length( glm::vec2{ mat[1][0], mat[1][1] } ) };
    }


    // +--------------------------------+
    // | TRANSFORM                      |
    // +--------------------------------+
    transform::transform( glm::vec2 const location ) noexcept
        : local_matrix_{ detail::internal::from_translation( location ) } { }


    transform::transform( glm::vec2 const location, float const rotation, glm::vec2 const scale ) noexcept
        : local_matrix_{ detail::internal::from_rts( location, rotation, scale ) } { }


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
                set_local_matrix( detail::internal::from_translation( translation ) );
            }
            else
            {
                mark_dirty( );
            }
        }

        // parenting logic
        if ( parent_ptr_ != nullptr ) { parent_ptr_->remove_child( this ); }
        if ( parent != nullptr ) { parent->add_child( this ); }
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

        transform* current = first_child_ptr_;
        while ( current != nullptr )
        {
            current->mark_dirty( );
            current = current->next_sibling_ptr_;
        }
    }


    auto transform::set_world_matrix( detail::matrix_type const& world_mat ) noexcept -> void
    {
        //todo: check for local matrix if need to use set_local_matrix
        // 1. update world matrix directly
        world_matrix_ = world_mat;

        // 2. if not root node, update local transform to match new world transform ...
        if ( parent_ptr_ )
        {
            parent_ptr_->refresh_world_transform( );
            set_local_matrix( glm::inverse( parent_ptr_->world_matrix_ ) * world_mat );
        }
        else
        {
            // ... else just set local to world
            set_local_matrix( world_mat );
        }

        // 3. reset dirty flag
        dirty_ = false;
    }


    auto transform::set_local_matrix( detail::matrix_type const& local_mat ) noexcept -> void
    {
        local_matrix_ = local_mat;
        mark_dirty( );
    }


    auto transform::is_child( transform const* maybe_child ) const noexcept -> bool
    {
        // bad argument check
        if ( maybe_child == nullptr )
        {
            return false;
        }

        // 0. base case
        if ( maybe_child == this )
        {
            return true;
        }

        // 1. check siblings, if any
        if ( next_sibling_ptr_ != nullptr && next_sibling_ptr_->is_child( maybe_child ) )
        {
            return true;
        }

        // 2. check children, if any
        if ( first_child_ptr_ != nullptr && first_child_ptr_->is_child( maybe_child ) )
        {
            return true;
        }

        // 3. not found in this branch
        return false;
    }


    auto transform::add_child( transform* child ) noexcept -> void
    {
        if ( child == nullptr ) { return; }

        // if no children, set as first child ...
        if ( first_child_ptr_ == nullptr )
        {
            first_child_ptr_ = child;
        }
        else
        {
            // ... else append to sibling list
            transform* current = first_child_ptr_;
            while ( current->next_sibling_ptr_ != nullptr )
            {
                current = current->next_sibling_ptr_;
            }
            current->next_sibling_ptr_ = child;
        }
    }


    auto transform::remove_child( transform const* child ) noexcept -> void
    {
        // if no children or bad argument, return
        if ( child == nullptr || first_child_ptr_ == nullptr ) { return; }

        // ... else find child and bypass
        if ( first_child_ptr_ == child )
        {
            first_child_ptr_ = child->next_sibling_ptr_;
        }
        else
        {
            transform* current = first_child_ptr_;
            while ( current->next_sibling_ptr_ != nullptr )
            {
                if ( current->next_sibling_ptr_ == child )
                {
                    current->next_sibling_ptr_ = current->next_sibling_ptr_->next_sibling_ptr_;
                    return;
                }
            }
        }
    }
}
