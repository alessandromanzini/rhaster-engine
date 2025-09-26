#ifndef RST_SERVICE_SDL_SOUND_SYSTEM_H
#define RST_SERVICE_SDL_SOUND_SYSTEM_H

#include <rst/pch.h>

#include <rst/__core/__service/base/sound_service.h>


namespace rst
{
    namespace sound
    {
        struct sdl_init_info final
        {
            sample_rate sample_rate{ sample_rate::hz_44100 };
            channel_type channel_type{ channel_type::stereo };
            uint16_t buffer_size{ 2048U };
        };

        struct sound_instance final
        {
            std::shared_ptr<audio> instance{ nullptr };
            std::optional<uint8_t> channel{};
        };
    }

    namespace service
    {
        // TODO: Garbage collection of sound resources
        class sdl_sound_service final : public sound_service
        {
        public:
            explicit sdl_sound_service(
                uint8_t channels, sound::sdl_init_info info, sound::queue_policy policy = sound::queue_policy::replace_oldest );
            ~sdl_sound_service( ) override;

            sdl_sound_service( sdl_sound_service const& )                        = delete;
            sdl_sound_service( sdl_sound_service&& ) noexcept                    = delete;
            auto operator=( sdl_sound_service const& ) -> sdl_sound_service&     = delete;
            auto operator=( sdl_sound_service&& ) noexcept -> sdl_sound_service& = delete;

            [[nodiscard]] auto service_type( ) -> service::service_type override;

            [[nodiscard]] auto load_sound(
                std::filesystem::path const& path, sound::sound_type type, earmark tag_mark ) -> std::shared_ptr<audio> override;

            auto play( audio const& audio, float volume, int loops ) -> int override;

            auto stop( audio const& audio ) -> bool override;
            auto stop_all( ) -> void override;

            auto pause( audio const& audio ) -> bool override;
            auto resume( audio const& audio ) -> bool override;

            [[nodiscard]] auto is_playing( audio const& audio ) const -> bool override;
            [[nodiscard]] auto is_paused( audio const& audio ) const -> bool override;

            [[nodiscard]] auto current_track( ) const -> audio const* override;

            auto set_master_volume( float volume ) -> void override;
            [[nodiscard]] auto master_volume( ) const -> float override;

            auto set_volume_by_tag( earmark tag_mark, float volume ) -> void override;
            [[nodiscard]] auto volume_by_tag( earmark tag_mark ) const -> float override;

        private:
            static constexpr uint8_t max_channels_{ 16U };

            uint8_t const channels_{};
            sound::queue_policy const policy_{};

            std::unordered_map<earmark, sound::sound_instance> sound_resources_{};

            int last_used_channel_{ -1 };

            float master_volume_{ 1.f };
            std::unordered_map<earmark, float> tag_volumes_{};

            sound::sound_instance* current_track_ptr_{};

            auto assert_on_missing_sound( audio const& audio ) const -> void;
            auto assert_on_missing_tag( earmark tag_mark ) const -> void;

            auto unload_unused_resources( ) -> void;

            [[nodiscard]] auto find_channel( ) const -> int;

            static auto handle_mixer_result( int result ) -> void;
        };
    }
}


#endif //!RST_SERVICE_SDL_SOUND_SYSTEM_H
