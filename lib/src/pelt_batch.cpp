#include <rst/__core/resource/pelt_batch.h>

#include <rst/temp/singleton/game_time.h>
#include <rst/temp/singleton/resource_manager.h>


namespace rst
{
    pelt_batch::pelt_batch(
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

        frame_width_  = texture_ptr_->dimensions( ).x / static_cast<float>( cols_ );
        frame_height_ = texture_ptr_->dimensions( ).y / static_cast<float>( rows_ );
    }


    auto pelt_batch::render( glm::vec2 const position ) const -> void
    {
        glm::vec4 const dst_rect{ position + offset_, frame_width_ * scale_, frame_height_ * scale_ };
        glm::vec4 const src_rect{
            static_cast<float>( current_frame_ % cols_ ) * frame_width_,
            static_cast<float>( current_frame_ / cols_ ) * frame_height_, frame_width_ * flip_.first, frame_height_ * flip_.second
        };
        // RENDERER.render_partial_texture( *texture_ptr_, dst_rect, src_rect );
    }


    auto pelt_batch::tick( ) -> void
    {
        accumulated_time_ += GAME_TIME.delta_time( );
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


    auto pelt_batch::flip( bool const horizontal, bool const vertical ) -> void
    {
        flip_.first  = horizontal ? -1 : 1;
        flip_.second = vertical ? -1 : 1;
    }


    auto pelt_batch::is_animation_completed( ) const -> bool
    {
        return completed_;
    }


    auto pelt_batch::is_looping( ) const -> bool
    {
        return loop_;
    }


    auto pelt_batch::frame_index( ) const -> uint16_t
    {
        return static_cast<uint16_t>( current_frame_ );
    }


    auto pelt_batch::reset( ) -> void
    {
        accumulated_time_ = 0.f;
        current_frame_    = 0;
        completed_        = false;
        flip_             = std::make_pair( static_cast<int8_t>( 1 ), static_cast<int8_t>( 1 ) );
    }


    auto pelt_batch::wrap_frame( ) -> void
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
