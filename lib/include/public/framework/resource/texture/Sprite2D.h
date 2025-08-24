#ifndef SPRITE2D_H
#define SPRITE2D_H

#include <framework/resource/texture/Texture2D.h>

#include <memory>


namespace engine
{
    class Sprite2D final
    {
    public:
        explicit Sprite2D( const std::string& filename, uint8_t rows, uint8_t cols, float frameDelay,
                           float scale      = 1.0f,
                           glm::vec2 offset = {},
                           bool loop        = true );

        void render( glm::vec2 position ) const;
        void tick( );

        void flip( bool horizontal, bool vertical );

        [[nodiscard]] bool is_animation_completed( ) const;
        [[nodiscard]] bool is_looping( ) const;

        [[nodiscard]] uint16_t get_frame_index( ) const;

        void reset( );

    private:
        const uint8_t rows_{};
        const uint8_t cols_{};
        const uint16_t total_frames_{};
        const float frame_delay_{};

        const float scale_{};
        const glm::vec2 offset_{};
        const bool loop_{};

        std::shared_ptr<Texture2D> texture_ptr_{ nullptr };

        float frame_width_{};
        float frame_height_{};

        float accumulated_time_{ 0.f };
        int current_frame_{ 0 };
        bool completed_{};

        std::pair<int8_t, int8_t> flip_{ static_cast<int8_t>( 1 ), static_cast<int8_t>( 1 ) };

        void wrap_frame( );

    };

}


#endif //!SPRITE2D_H
