#ifndef PARALLELSOUNDSYSTEM_H
#define PARALLELSOUNDSYSTEM_H

#include <service/sound/SoundSystem.h>

#include <framework/resource/data/SafeResource.h>
#include <framework/event/EventQueue.h>
#include <framework/event/Observer.h>

#include <cassert>
#include <thread>


namespace engine
{
    namespace sound
    {
        enum class PlaybackMode : uint8_t
        {
            PLAY,
            STOP,
            PAUSE,
            RESUME
        };

        struct SoundPlaybackOptions final
        {
            PlaybackMode mode{ PlaybackMode::PLAY };
            const Audio* audio{ nullptr };
            float volume{ 0.f };
            int loops{ 0 };
        };
    }


    class ParallelSoundSystem final : public SoundSystem
    {
    public:
        explicit ParallelSoundSystem( std::unique_ptr<SoundSystem>&& ss );
        ~ParallelSoundSystem( ) override;

        ParallelSoundSystem( const ParallelSoundSystem& )                = delete;
        ParallelSoundSystem( ParallelSoundSystem&& ) noexcept            = delete;
        ParallelSoundSystem& operator=( const ParallelSoundSystem& )     = delete;
        ParallelSoundSystem& operator=( ParallelSoundSystem&& ) noexcept = delete;

        [[nodiscard]] ServiceType get_service_type( ) override;

        [[nodiscard]] std::shared_ptr<Audio> load_sound( const std::filesystem::path& path,
                                                         sound::SoundType type,
                                                         UID tagId ) override;

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
        bool running_{ true };

        std::thread worker_thread_;
        std::condition_variable loop_cv_;
        threading::SafeResource<EventQueue<sound::SoundPlaybackOptions>> playback_queue_;

        std::unique_ptr<SoundSystem> impl_ptr_{};

        void create_worker_thread( );

    };

}


#endif //!PARALLELSOUNDSYSTEM_H
