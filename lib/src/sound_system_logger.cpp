#include <rst/__core/__service/sound/sound_logger_service.h>

#include <rst/temp/singleton/resource_manager.h>
#include <rst/__core/resource/audio.h>


// todo: assess code
// todo: substitute logging with engine logging
namespace rst
{
    std::map<sound::sound_type, std::string> sound_type_to_string{
        { sound::sound_type::sound_effect, "SOUND_EFFECT" }, { sound::sound_type::sound_track, "SOUND_TRACK" }
    };


    sound_logger_service::sound_logger_service(
        std::unique_ptr<sound_service> sound_system, std::ostream& log, std::ostream& err, std::string identifier )
        : logger_identifier_{ std::move( identifier ) }
        , sound_system_ptr_{ std::move( sound_system ) }
        , log_stream_{ log }
        , error_stream_{ err } { }


    auto sound_logger_service::service_type( ) -> service::service_type
    {
        return sound_system_ptr_->service_type( );
    }


    auto sound_logger_service::load_sound(
        std::filesystem::path const& path, sound::sound_type const type, earmark const tag_mark ) -> std::shared_ptr<audio>
    {
        log_stream_ << logger_identifier_ << "Loading " << sound_type_to_string[type] << " from " << path << '\n';
        std::shared_ptr sound{ sound_system_ptr_->load_sound( path, type, tag_mark ) };

        log_stream_ << logger_identifier_ << "Assigned " << sound_info( *sound ) << '\n';

        return sound;
    }


    auto sound_logger_service::play( audio const& audio, float const volume, int const loops ) -> int
    {
        log_stream_ << logger_identifier_ << "Requested to playback of " << sound_type_to_string[audio.type( )] << ": " <<
                sound_info( audio ) << '\n';

        return sound_system_ptr_->play( audio, volume, loops );
    }


    auto sound_logger_service::stop( audio const& audio ) -> bool
    {
        bool const success{ sound_system_ptr_->stop( audio ) };
        if ( success )
        {
            log_stream_ << logger_identifier_ << "Stopped to playback of " << sound_type_to_string[audio.type( )] << ": " <<
                    sound_info( audio ) << '\n';
        }
        else
        {
            error_stream_ << logger_identifier_ << "Playback stop requested failed " << sound_type_to_string[audio.type( )] <<
                    ": " << sound_info( audio ) << '\n';
        }
        return success;
    }


    auto sound_logger_service::stop_all( ) -> void
    {
        log_stream_ << logger_identifier_ << "Stopped all playbacks" << '\n';
        sound_system_ptr_->stop_all( );
    }


    auto sound_logger_service::pause( audio const& audio ) -> bool
    {
        bool const success{ sound_system_ptr_->pause( audio ) };
        if ( success )
        {
            log_stream_ << logger_identifier_ << "Paused playback of " << sound_type_to_string[audio.type( )] << ": " <<
                    sound_info( audio ) << '\n';
        }
        else
        {
            error_stream_ << logger_identifier_ << "Playback pause requested failed " << sound_type_to_string[audio.type( )]
                    << ": " << sound_info( audio ) << '\n';
        }
        return success;
    }


    auto sound_logger_service::resume( audio const& audio ) -> bool
    {
        bool const success{ sound_system_ptr_->resume( audio ) };
        if ( success )
        {
            log_stream_ << logger_identifier_ << "Resumed playback of " << sound_type_to_string[audio.type( )] << ": " <<
                    sound_info( audio ) << '\n';
        }
        else
        {
            error_stream_ << logger_identifier_ << "Playback resume requested failed " << sound_type_to_string[audio.type( )]
                    << ": " << sound_info( audio ) << '\n';
        }
        return success;
    }


    auto sound_logger_service::is_playing( audio const& audio ) const -> bool
    {
        return sound_system_ptr_->is_playing( audio );
    }


    auto sound_logger_service::is_paused( audio const& audio ) const -> bool
    {
        return sound_system_ptr_->is_paused( audio );
    }


    auto sound_logger_service::current_track( ) const -> audio const*
    {
        return sound_system_ptr_->current_track( );
    }


    auto sound_logger_service::set_master_volume( float const volume ) -> void
    {
        sound_system_ptr_->set_master_volume( volume );
        log_stream_ << logger_identifier_ << "Set master volume to " << sound_system_ptr_->master_volume( ) << '\n';
    }


    auto sound_logger_service::master_volume( ) const -> float
    {
        return sound_system_ptr_->master_volume( );
    }


    auto sound_logger_service::set_volume_by_tag( earmark const tag_mark, float const volume ) -> void
    {
        sound_system_ptr_->set_volume_by_tag( tag_mark, volume );
        log_stream_ << logger_identifier_ << "Set volume for tag " << tag_mark << " to " << sound_system_ptr_->
                volume_by_tag( tag_mark ) << '\n';
    }


    auto sound_logger_service::volume_by_tag( earmark const tag_mark ) const -> float
    {
        return sound_system_ptr_->volume_by_tag( tag_mark );
    }


    auto sound_logger_service::sound_info( audio const& audio ) -> std::string
    {
        std::stringstream ss{};
        ss << "[TAG: " << audio.tag_mark( ) << ", UID: " << audio.sound_mark( ) << "]";
        return ss.str( );
    }
}
