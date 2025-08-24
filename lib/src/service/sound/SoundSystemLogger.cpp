#include <map>
#include <service/sound/SoundSystemLogger.h>
#include <singleton/ResourceManager.h>


namespace engine
{
    std::map<sound::SoundType, std::string> g_sound_type_to_string{
        { sound::SoundType::SOUND_EFFECT, "SOUND_EFFECT" },
        { sound::SoundType::SOUND_TRACK, "SOUND_TRACK" }
    };

    std::string_view g_logger_sig{ "[SoundSystemLogger] :> " };


    SoundSystemLogger::SoundSystemLogger( std::unique_ptr<SoundSystem> soundSystem, std::ostream& logStream,
                                          std::ostream& errorStream )
        : sound_system_ptr_{ std::move( soundSystem ) }
        , log_stream_{ logStream }
        , error_stream_{ errorStream }
    {
    }


    ServiceType SoundSystemLogger::get_service_type( )
    {
        return sound_system_ptr_->get_service_type( );
    }


    std::shared_ptr<Audio> SoundSystemLogger::load_sound( const std::filesystem::path& path,
                                                          const sound::SoundType type,
                                                          const UID tagId )
    {
        log_stream_ << LOGGER_SIG_ << "Loading " << g_sound_type_to_string[type] << " from " << path << '\n';
        std::shared_ptr sound{ sound_system_ptr_->load_sound( path, type, tagId ) };

        log_stream_ << LOGGER_SIG_ << "Assigned " << get_sound_info( *sound ) << '\n';

        return sound;
    }


    int SoundSystemLogger::play( const Audio& audio, const float volume, const int loops )
    {
        log_stream_ << LOGGER_SIG_ << "Requested to playback of " << g_sound_type_to_string[audio.get_type( )] << ": "
                << get_sound_info( audio ) << '\n';

        return sound_system_ptr_->play( audio, volume, loops );
    }


    bool SoundSystemLogger::stop( const Audio& audio )
    {
        const bool success{ sound_system_ptr_->stop( audio ) };

        if ( success )
        {
            log_stream_ << LOGGER_SIG_ << "Stopped to playback of " << g_sound_type_to_string[audio.get_type( )] << ": "
                    << get_sound_info( audio ) << '\n';
        }
        else
        {
            error_stream_ << LOGGER_SIG_ << "Playback stop requested failed " << g_sound_type_to_string[audio.get_type( )] << ": "
                    << get_sound_info( audio ) << '\n';
        }

        return success;
    }


    void SoundSystemLogger::stop_all( )
    {
        log_stream_ << LOGGER_SIG_ << "Stopped all playbacks" << '\n';

        sound_system_ptr_->stop_all( );
    }


    bool SoundSystemLogger::pause( const Audio& audio )
    {
        const bool success{ sound_system_ptr_->pause( audio ) };

        if ( success )
        {
            log_stream_ << LOGGER_SIG_ << "Paused playback of " << g_sound_type_to_string[audio.get_type( )] << ": "
                    << get_sound_info( audio ) << '\n';
        }
        else
        {
            error_stream_ << LOGGER_SIG_ << "Playback pause requested failed " << g_sound_type_to_string[audio.get_type( )] <<
                    ": "
                    << get_sound_info( audio ) << '\n';
        }

        return success;
    }


    bool SoundSystemLogger::resume( const Audio& audio )
    {
        const bool success{ sound_system_ptr_->resume( audio ) };

        if ( success )
        {
            log_stream_ << LOGGER_SIG_ << "Resumed playback of " << g_sound_type_to_string[audio.get_type( )] << ": "
                    << get_sound_info( audio ) << '\n';
        }
        else
        {
            error_stream_ << LOGGER_SIG_ << "Playback resume requested failed " << g_sound_type_to_string[audio.get_type( )] <<
                    ": " << get_sound_info( audio ) << '\n';
        }

        return success;
    }


    bool SoundSystemLogger::is_playing( const Audio& audio ) const
    {
        return sound_system_ptr_->is_playing( audio );
    }


    bool SoundSystemLogger::is_paused( const Audio& audio ) const
    {
        return sound_system_ptr_->is_paused( audio );
    }


    const Audio* SoundSystemLogger::get_current_track( ) const
    {
        return sound_system_ptr_->get_current_track( );
    }


    void SoundSystemLogger::set_master_volume( const float volume )
    {
        sound_system_ptr_->set_master_volume( volume );
        log_stream_ << LOGGER_SIG_ << "Set master volume to " << sound_system_ptr_->get_master_volume( ) << '\n';
    }


    float SoundSystemLogger::get_master_volume( ) const
    {
        return sound_system_ptr_->get_master_volume( );
    }


    void SoundSystemLogger::set_volume_by_tag( const UID tagId, const float volume )
    {
        sound_system_ptr_->set_volume_by_tag( tagId, volume );
        log_stream_ << LOGGER_SIG_ << "Set volume for tag " << tagId.uid << " to " << sound_system_ptr_->
                get_volume_by_tag( tagId ) << '\n';
    }


    float SoundSystemLogger::get_volume_by_tag( const UID tagId ) const
    {
        return sound_system_ptr_->get_volume_by_tag( tagId );
    }


    std::string SoundSystemLogger::get_sound_info( const Audio& audio )
    {
        std::stringstream ss{};
        ss << "[TAG: " << audio.get_tag_id( ).uid << ", UID: " << audio.get_sound_id( ).uid << "]";
        return ss.str( );
    }

}
