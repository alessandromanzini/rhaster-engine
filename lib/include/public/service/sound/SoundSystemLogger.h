#ifndef SOUNDSYSTEMLOGGER_H
#define SOUNDSYSTEMLOGGER_H

#include <framework/event/Observer.h>
#include <service/sound/SoundSystem.h>

#include <sstream>


namespace engine
{
    class SoundSystemLogger final : public SoundSystem
    {
    public:
        explicit SoundSystemLogger( std::unique_ptr<SoundSystem> soundSystem, std::ostream& logStream,
                                          std::ostream& errorStream );

        [[nodiscard]] ServiceType get_service_type( ) override;

        [[nodiscard]] std::shared_ptr<Audio>
        load_sound( const std::filesystem::path& path, sound::SoundType type, UID tagId ) override;

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
        static constexpr std::string_view LOGGER_SIG_{ "[SoundSystemLogger] :> " };

        std::unique_ptr<SoundSystem> sound_system_ptr_{ nullptr };
        std::ostream& log_stream_;
        std::ostream& error_stream_;

        static std::string get_sound_info( const Audio& audio );

    };

}


#endif //!SOUNDSYSTEMLOGGER_H
