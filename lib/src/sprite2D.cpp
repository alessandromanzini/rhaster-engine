#include <rst/__type/texture_type/sprite2D.h>

#include <rst/temp/singleton/game_time.h>
#include <rst/temp/singleton/renderer.h>
#include <rst/temp/singleton/resource_manager.h>
#include <rst/__core/gameobject.h>


namespace rst
{
    Sprite2D::Sprite2D(
        std::string const& filename, uint8_t const rows, uint8_t const cols, float const frame_delay, float const scale,
        glm::vec2 const offset, bool const loop )
        : rows_{ rows }
        , cols_{ cols }
        , total_frames_{ static_cast<uint16_t>( rows * cols ) }
        , frame_delay_{ frame_delay }
        , scale_{ scale }
        , offset_{ offset }
        , loop_{ loop }
        , texture_ptr_{ RESOURCE_MANAGER.load_texture( filename ) }
    {
        assert( frame_delay_ > 0.f && "Frame delay must be greater than 0" );
        assert( total_frames_ > 0 && "Total frames must be greater than 0" );

        frame_width_  = texture_ptr_->get_size( ).x / static_cast<float>( cols_ );
        frame_height_ = texture_ptr_->get_size( ).y / static_cast<float>( rows_ );
    }


    auto Sprite2D::render( glm::vec2 const position ) const -> void
    {
        glm::vec4 const dst_rect{ position + offset_, frame_width_ * scale_, frame_height_ * scale_ };
        glm::vec4 const src_rect{
            static_cast<float>( current_frame_ % cols_ ) * frame_width_,
            static_cast<float>( current_frame_ / cols_ ) * frame_height_, frame_width_ * flip_.first, frame_height_ * flip_.second
        };
        RENDERER.render_partial_texture( *texture_ptr_, dst_rect, src_rect );
    }


    auto Sprite2D::tick( ) -> void
    {
        accumulated_time_ += GAME_TIME.get_delta_time( );
        if ( accumulated_time_ >= frame_delay_ )
        {
            accumulated_time_ -= frame_delay_;
            ++current_frame_;

            if ( current_frame_ >= total_frames_ )
            {
                completed_ = true;
                wrap_frame( );
            }
        }
    }


    auto Sprite2D::flip( bool const horizontal, bool const vertical ) -> void
    {
        flip_.first  = horizontal ? -1 : 1;
        flip_.second = vertical ? -1 : 1;
    }


    auto Sprite2D::is_animation_completed( ) const -> bool
    {
        return completed_;
    }


    auto Sprite2D::is_looping( ) const -> bool
    {
        return loop_;
    }


    auto Sprite2D::get_frame_index( ) const -> uint16_t
    {
        return static_cast<uint16_t>( current_frame_ );
    }


    auto Sprite2D::reset( ) -> void
    {
        accumulated_time_ = 0.f;
        current_frame_    = 0;
        completed_        = false;
        flip_             = std::make_pair( static_cast<int8_t>( 1 ), static_cast<int8_t>( 1 ) );
    }


    auto Sprite2D::wrap_frame( ) -> void
    {
        if ( loop_ )
        {
            current_frame_ %= total_frames_;
        }
        else
        {
            current_frame_ = std::min( current_frame_, total_frames_ - 1 );
        }
    }
}
