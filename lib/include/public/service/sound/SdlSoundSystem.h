#ifndef SDLSOUNDSYSTEM_H
#define SDLSOUNDSYSTEM_H

#include <framework/event/Observer.h>
#include <service/sound/SoundSystem.h>

#include <cstdint>


namespace engine
{
    namespace sound
    {
        struct SdlInitInfo final
        {
            SampleRate sampleRate{ SampleRate::HZ_44100 };
            ChannelType channelType{ ChannelType::STEREO };
            uint16_t bufferSize{ 2048 };
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
        explicit SdlSoundSystem( uint8_t channels, sound::SdlInitInfo info, sound::QueuePolicy policy = sound::QueuePolicy::DISCARD );
        ~SdlSoundSystem( ) override;

        SdlSoundSystem( const SdlSoundSystem& )                = delete;
        SdlSoundSystem( SdlSoundSystem&& ) noexcept            = delete;
        SdlSoundSystem& operator=( const SdlSoundSystem& )     = delete;
        SdlSoundSystem& operator=( SdlSoundSystem&& ) noexcept = delete;

        static void request_channels( uint8_t channels );

        [[nodiscard]] ServiceType get_service_type( ) override;

        [[nodiscard]] std::shared_ptr<Audio> load_sound( const std::filesystem::path& path, sound::SoundType type, UID tagId ) override;

        int play( const Audio& audio, float volume, int loops ) override;

        bool stop( const Audio& audio ) override;
        void stop_all( ) override;

        bool pause( const Audio& audio ) override;
        bool resume( const Audio& audio ) override;

        [[nodiscard]] bool is_playing( const Audio& audio ) const override;
        [[nodiscard]] bool is_paused( const Audio& audio ) const override;

        [[nodiscard]] const Audio* get_current_track( ) const override;

        void set_master_volume( float volume ) override;
        [[nodiscard]] float get_master_volume( ) const override;

        void set_volume_by_tag( UID tagId, float volume ) override;
        [[nodiscard]] float get_volume_by_tag( UID tagId ) const override;

    private:
        static constexpr uint8_t MAX_CHANNELS_{ 16 };

        const uint8_t channels_{};
        const sound::QueuePolicy policy_{};

        std::unordered_map<UID, sound::SoundInstance> sound_resources_{};

        int last_used_channel_{ -1 };

        float master_volume_{ 1.f };
        std::unordered_map<UID, float> tag_volumes_{};

        sound::SoundInstance* current_track_ptr_{};

        void assert_on_missing_sound( const Audio& audio ) const;
        void assert_on_missing_tag( UID tagId ) const;
        void handle_mixer_result( int result ) const;

        void unload_unused_resources( );

        [[nodiscard]] int find_channel( ) const;

    };

}


#endif //!SDLSOUNDSYSTEM_H
