#ifndef RST_SPRITE_2D_H
#define RST_SPRITE_2D_H

#include <rst/pch.h>

#include <rst/__resource_type/texture/texture2D.h>


// TODO: add documentation
namespace rst
{
    class sprite_2d final
    {
    public:
        explicit sprite_2d(
            std::string const& filename, uint8_t rows, uint8_t cols, float frame_delay,
            float scale = 1.0f, glm::vec2 offset = {}, bool loop = true );

        auto render( glm::vec2 position ) const -> void;
        auto tick( ) -> void;

        auto flip( bool horizontal, bool vertical ) -> void;

        [[nodiscard]] auto is_animation_completed( ) const -> bool;
        [[nodiscard]] auto is_looping( ) const -> bool;

        [[nodiscard]] auto frame_index( ) const -> uint16_t;

        auto reset( ) -> void;

    private:
        uint8_t const rows_{};
        uint8_t const cols_{};
        uint16_t const total_frames_{};
        float const frame_delay_{};

        float const scale_{};
        glm::vec2 const offset_{};
        bool const loop_{};

        std::shared_ptr<texture_2d> texture_ptr_{ nullptr };

        float frame_width_{};
        float frame_height_{};

        float accumulated_time_{ 0.f };
        int current_frame_{ 0 };
        bool completed_{};

        std::pair<int8_t, int8_t> flip_{ static_cast<int8_t>( 1 ), static_cast<int8_t>( 1 ) };

        auto wrap_frame( ) -> void;
    };
}


#endif //!RST_SPRITE_2D_H
