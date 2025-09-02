#ifndef RST_PARALLEL_SOUND_SYSTEM_H
#define RST_PARALLEL_SOUND_SYSTEM_H

#include <rst/pch.h>

#include <rst/framework/data_type.h>
#include <rst/framework/event_queue.h>
#include <rst/__service/sound/sound_system.h>


namespace rst
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
            Audio const* audio{ nullptr };
            float volume{ 0.f };
            int loops{ 0 };
        };
    }


    class ParallelSoundSystem final : public SoundSystem
    {
    public:
        explicit ParallelSoundSystem( std::unique_ptr<SoundSystem>&& ss );
        ~ParallelSoundSystem( ) override;

        ParallelSoundSystem( ParallelSoundSystem const& )                        = delete;
        ParallelSoundSystem( ParallelSoundSystem&& ) noexcept                    = delete;
        auto operator=( ParallelSoundSystem const& ) -> ParallelSoundSystem&     = delete;
        auto operator=( ParallelSoundSystem&& ) noexcept -> ParallelSoundSystem& = delete;

        [[nodiscard]] auto get_service_type( ) -> ServiceType override;

        [[nodiscard]] auto load_sound(
            std::filesystem::path const& path, sound::SoundType type, Uid tag_id ) -> std::shared_ptr<Audio> override;

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

        auto set_volume_by_tag( Uid tag_id, float volume ) -> void override;
        [[nodiscard]] auto get_volume_by_tag( Uid tag_id ) const -> float override;

    private:
        bool running_{ true };

        std::thread worker_thread_;
        std::condition_variable loop_cv_;
        thread::SafeResource<EventQueue<sound::SoundPlaybackOptions>> playback_queue_;

        std::unique_ptr<SoundSystem> impl_ptr_{};

        auto create_worker_thread( ) -> void;
    };
}


#endif //!RST_PARALLEL_SOUND_SYSTEM_H
