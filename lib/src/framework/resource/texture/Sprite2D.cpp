#include <framework/resource/texture/Sprite2D.h>

#include <framework/GameObject.h>
#include <singleton/GameTime.h>
#include <singleton/Renderer.h>
#include <singleton/ResourceManager.h>


namespace engine
{
    Sprite2D::Sprite2D( const std::string& filename, const uint8_t rows, const uint8_t cols,
                        const float frameDelay, const float scale, const glm::vec2 offset,
                        const bool loop )
        : rows_{ rows }
        , cols_{ cols }
        , total_frames_{ static_cast<uint16_t>( rows * cols ) }
        , frame_delay_{ frameDelay }
        , scale_{ scale }
        , offset_{ offset }
        , loop_{ loop }
        , texture_ptr_{ RESOURCE_MANAGER.load_texture( filename ) }
    {
        assert( frame_delay_ > 0.f && "Frame delay must be greater than 0" );
        assert( total_frames_ > 0 && "Total frames must be greater than 0" );

        frame_width_  = static_cast<float>( texture_ptr_->get_size( ).x ) / cols_;
        frame_height_ = static_cast<float>( texture_ptr_->get_size( ).y ) / rows_;
    }


    void Sprite2D::render( const glm::vec2 position ) const
    {
        const glm::vec4 dstRect{
            position + offset_,
            frame_width_ * scale_,
            frame_height_ * scale_
        };
        const glm::vec4 srcRect{
            static_cast<float>( current_frame_ % cols_ ) * frame_width_,
            static_cast<float>( current_frame_ / cols_ ) * frame_height_,
            frame_width_ * flip_.first,
            frame_height_ * flip_.second
        };
        RENDERER.render_partial_texture( *texture_ptr_, dstRect, srcRect );
    }


    void Sprite2D::tick( )
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


    void Sprite2D::flip( const bool horizontal, const bool vertical ) {
        flip_.first  = horizontal ? -1 : 1;
        flip_.second = vertical ? -1 : 1;
    }


    bool Sprite2D::is_animation_completed( ) const
    {
        return completed_;
    }


    bool Sprite2D::is_looping( ) const
    {
        return loop_;
    }


    uint16_t Sprite2D::get_frame_index( ) const
    {
        return static_cast<uint16_t>( current_frame_ );
    }


    void Sprite2D::reset( )
    {
        accumulated_time_ = 0.f;
        current_frame_    = 0;
        completed_        = false;
        flip_             = std::make_pair( static_cast<int8_t>( 1 ), static_cast<int8_t>( 1 ) );
    }


    void Sprite2D::wrap_frame( )
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
