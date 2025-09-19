#ifndef RST_SERVICE_PARALLEL_SOUND_SYSTEM_H
#define RST_SERVICE_PARALLEL_SOUND_SYSTEM_H

#include <rst/pch.h>

#include <rst/data_type/safe_resource.h>
#include <rst/data_type/event/event_queue.h>
#include <rst/__core/__service/sound_service.h>


namespace rst
{
    namespace sound
    {
        enum class playback_mode : uint8_t
        {
            play, stop, pause, resume
        };

        struct sound_playback_options final
        {
            playback_mode mode{ playback_mode::play };
            audio const* audio{ nullptr };
            float volume{ 0.f };
            int loops{ 0 };
        };
    }


    namespace service
    {
        class parallel_sound_service final : public sound_service
        {
        public:
            explicit parallel_sound_service( std::unique_ptr<sound_service>&& ss );
            ~parallel_sound_service( ) override;

            parallel_sound_service( parallel_sound_service const& )                        = delete;
            parallel_sound_service( parallel_sound_service&& ) noexcept                    = delete;
            auto operator=( parallel_sound_service const& ) -> parallel_sound_service&     = delete;
            auto operator=( parallel_sound_service&& ) noexcept -> parallel_sound_service& = delete;

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
            [[nodiscard]] auto volume_by_tag( earmark tag_id ) const -> float override;

        private:
            bool running_{ true };

            std::thread worker_thread_;
            std::condition_variable loop_cv_;
            thread::safe_resource<event_queue<sound::sound_playback_options>> playback_queue_;

            std::unique_ptr<sound_service> impl_ptr_{};

            auto create_worker_thread( ) -> void;
        };
    }
}


#endif //!RST_SERVICE_PARALLEL_SOUND_SYSTEM_H
