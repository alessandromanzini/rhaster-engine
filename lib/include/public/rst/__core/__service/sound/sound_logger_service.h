#ifndef RST_SERVICE_SOUND_SYSTEM_LOGGER_H
#define RST_SERVICE_SOUND_SYSTEM_LOGGER_H

#include <rst/pch.h>

#include <rst/__core/__service/sound_service.h>


namespace rst
{
    namespace internal
    {
        inline constexpr std::string_view default_logger_identifier{ "[SoundSystemLogger] :> " };
    }


    class sound_logger_service final : public sound_service
    {
    public:
        explicit sound_logger_service(
            std::unique_ptr<sound_service> sound_system, std::ostream& log, std::ostream& err,
            std::string identifier = internal::default_logger_identifier.data( ) );

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
        std::string const logger_identifier_;

        std::unique_ptr<sound_service> sound_system_ptr_{ nullptr };
        std::ostream& log_stream_;
        std::ostream& error_stream_;

        static auto sound_info( audio const& audio ) -> std::string;
    };
}


#endif //!RST_SERVICE_SOUND_SYSTEM_LOGGER_H
