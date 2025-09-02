#ifndef RST_SDL_SOUND_SYSTEM_H
#define RST_SDL_SOUND_SYSTEM_H

#include <rst/pch.h>

#include <rst/__service/sound/sound_system.h>


namespace rst
{
    namespace sound
    {
        struct SdlInitInfo final
        {
            SampleRate sample_rate{ SampleRate::HZ_44100 };
            ChannelType channel_type{ ChannelType::STEREO };
            uint16_t buffer_size{ 2048U };
        };

        struct SoundInstance final
        {
            std::shared_ptr<Audio> instance{ nullptr };
            std::optional<uint8_t> channel{};
        };
    }

    // TODO: Garbage collection of sound resources
    class SdlSoundSystem final : public SoundSystem
    {
    public:
        explicit SdlSoundSystem(
            uint8_t channels, sound::SdlInitInfo info, sound::QueuePolicy policy = sound::QueuePolicy::REPLACE_OLDEST );
        ~SdlSoundSystem( ) override;

        SdlSoundSystem( SdlSoundSystem const& )                        = delete;
        SdlSoundSystem( SdlSoundSystem&& ) noexcept                    = delete;
        auto operator=( SdlSoundSystem const& ) -> SdlSoundSystem&     = delete;
        auto operator=( SdlSoundSystem&& ) noexcept -> SdlSoundSystem& = delete;

        [[nodiscard]] auto get_service_type( ) -> ServiceType override;

        [[nodiscard]] auto load_sound(
            std::filesystem::path const& path, sound::SoundType type, UID tag_id ) -> std::shared_ptr<Audio> override;

        auto play( Audio const& audio, float volume, int loops ) -> int override;

        auto stop( Audio const& audio ) -> bool override;
        auto stop_all( ) -> void override;

        auto pause( Audio const& audio ) -> bool override;
        auto resume( Audio const& audio ) -> bool override;

        [[nodiscard]] auto is_playing( Audio const& audio ) const -> bool override;
        [[nodiscard]] auto is_paused( Audio const& audio ) const -> bool override;

        [[nodiscard]] auto get_current_track( ) const -> Audio const* override;

        auto set_master_volume( float volume ) -> void override;
        [[nodiscard]] auto get_master_volume( ) const -> float override;

        auto set_volume_by_tag( UID tag_id, float volume ) -> void override;
        [[nodiscard]] auto get_volume_by_tag( UID tag_id ) const -> float override;

    private:
        static constexpr uint8_t MAX_CHANNELS_{ 16U };

        uint8_t const channels_{};
        sound::QueuePolicy const policy_{};

        std::unordered_map<UID, sound::SoundInstance> sound_resources_{};

        int last_used_channel_{ -1 };

        float master_volume_{ 1.f };
        std::unordered_map<UID, float> tag_volumes_{};

        sound::SoundInstance* current_track_ptr_{};

        auto assert_on_missing_sound( Audio const& audio ) const -> void;
        auto assert_on_missing_tag( UID tag_id ) const -> void;

        auto unload_unused_resources( ) -> void;

        [[nodiscard]] auto find_channel( ) const -> int;

        static auto handle_mixer_result( int result ) -> void;
    };
}


#endif //!RST_SDL_SOUND_SYSTEM_H
