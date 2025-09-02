#ifndef RST_SOUND_SYSTEM_LOGGER_H
#define RST_SOUND_SYSTEM_LOGGER_H

#include <rst/pch.h>

#include <rst/__service/sound/sound_system.h>


namespace rst
{
    class SoundSystemLogger final : public SoundSystem
    {
    public:
        explicit SoundSystemLogger(
            std::unique_ptr<SoundSystem> sound_system, std::ostream& log, std::ostream& err,
            std::string const& identifier = "[SoundSystemLogger] :> " );

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
        std::string const logger_identifier_;

        std::unique_ptr<SoundSystem> sound_system_ptr_{ nullptr };
        std::ostream& log_stream_;
        std::ostream& error_stream_;

        static auto get_sound_info( Audio const& audio ) -> std::string;
    };
}


#endif //!RST_SOUND_SYSTEM_LOGGER_H
