#include <rst/__core/__service/sound/sdl_sound_service.h>

#include <rst/temp/singleton/resource_manager.h>
#include <rst/__internal/resource/sdl_audio.h>

#include <SDL_audio.h>
#include <SDL_mixer.h>


namespace rst::service
{
    // TODO: make assertion engine configurable
    // TODO: assess code
    sdl_sound_service::sdl_sound_service( uint8_t const channels, sound::sdl_init_info info, sound::queue_policy const policy )
        : channels_{ channels }
        , policy_{ policy }
    {
        assert( channels_ <= max_channels_ && "Too many channels requested!" );

        Mix_OpenAudio(
            static_cast<int>( info.sample_rate ), MIX_DEFAULT_FORMAT, static_cast<int>( info.channel_type ), info.buffer_size );
        Mix_Init( MIX_INIT_WAVPACK | MIX_INIT_MP3 | MIX_INIT_FLAC );
        Mix_AllocateChannels( channels );
    }


    sdl_sound_service::~sdl_sound_service( )
    {
        Mix_CloseAudio( );
        Mix_Quit( );
    }


    auto sdl_sound_service::service_type( ) -> service::service_type
    {
        return service_type::sdl;
    }


    auto sdl_sound_service::load_sound(
        std::filesystem::path const& path, sound::sound_type const type, earmark const tag_mark ) -> std::shared_ptr<audio>
    {
        earmark const sound_mark{ path.string( ) };

        // check if the sound is already registered. If not we can register it
        if ( not sound_resources_.contains( sound_mark ) )
        {
            sound_resources_[sound_mark] = sound::sound_instance{
                std::make_shared<sdl_audio>( RESOURCE_MANAGER.data_path( ) / path, type, sound_mark, tag_mark )
            };
        }
        else
        {
            assert(
                sound_resources_.at( sound_mark ).instance->tag_mark( ) == tag_mark &&
                "Sound already registered with a different tag!" );
        }

        // initialize the tag volume to 1.0f if it doesn't exist
        if ( not tag_volumes_.contains( tag_mark ) )
        {
            tag_volumes_[tag_mark] = 1.f;
        }

        return sound_resources_.at( sound_mark ).instance;
    }


    auto sdl_sound_service::play( audio const& audio, float const volume, int const loops ) -> int
    {
        assert_on_missing_sound( audio );

        auto& sound          = sound_resources_.at( audio.sound_mark( ) );
        auto const* instance = static_cast<sdl_audio*>( sound.instance.get( ) );

        int const mix_volume{ static_cast<int>( MIX_MAX_VOLUME * volume * tag_volumes_.at( audio.tag_mark( ) ) ) };

        switch ( audio.type( ) )
        {
            case sound::sound_type::sound_effect:
            {
                auto const effect = std::get<Mix_Chunk*>( instance->resource( ) );
                Mix_VolumeChunk( effect, mix_volume );

                // play the sound effect and store the channel
                auto const channel = Mix_PlayChannel( find_channel( ), effect, loops );
                if ( channel != -1 )
                {
                    sound.channel      = static_cast<uint8_t>( channel );
                    last_used_channel_ = channel;
                }
                return channel;
            }

            case sound::sound_type::sound_track:
            {
                auto const track = std::get<Mix_Music*>( instance->resource( ) );
                Mix_VolumeMusic( mix_volume );
                Mix_PlayMusic( track, loops );
                current_track_ptr_ = &sound;
                return 0;
            }
        }
        return -1;
    }


    auto sdl_sound_service::stop( audio const& audio ) -> bool
    {
        assert_on_missing_sound( audio );

        auto& [instance, channel] = sound_resources_.at( audio.sound_mark( ) );

        switch ( audio.type( ) )
        {
            case sound::sound_type::sound_effect:
            {
                auto const result = Mix_HaltChannel( channel.value( ) );
                handle_mixer_result( result );
                channel = std::nullopt;
                return true;
            }

            case sound::sound_type::sound_track: if ( current_track_ptr_ )
                {
                    Mix_HaltMusic( );
                    current_track_ptr_ = nullptr;
                    return true;
                }
        }
        return false;
    }


    auto sdl_sound_service::stop_all( ) -> void
    {
        current_track_ptr_ = nullptr;
        Mix_HaltMusic( );

        Mix_HaltChannel( -1 );
    }


    auto sdl_sound_service::pause( audio const& audio ) -> bool
    {
        assert_on_missing_sound( audio );

        switch ( audio.type( ) )
        {
            case sound::sound_type::sound_effect: if ( auto& [instance, channel] = sound_resources_.at( audio.sound_mark( ) );
                    channel.has_value( ) )
                {
                    Mix_Pause( channel.value( ) );
                    return true;
                }

            case sound::sound_type::sound_track: if ( current_track_ptr_ )
                {
                    Mix_PauseMusic( );
                    return true;
                }
        }
        return false;
    }


    auto sdl_sound_service::resume( audio const& audio ) -> bool
    {
        assert_on_missing_sound( audio );

        switch ( audio.type( ) )
        {
            case sound::sound_type::sound_effect: if ( auto& [instance, channel] = sound_resources_.at( audio.sound_mark( ) );
                    channel.has_value( ) )
                {
                    Mix_Resume( channel.value( ) );
                    return true;
                }

            case sound::sound_type::sound_track: if ( current_track_ptr_ )
                {
                    Mix_ResumeMusic( );
                    return true;
                }
        }
        return false;
    }


    auto sdl_sound_service::is_playing( audio const& audio ) const -> bool
    {
        assert_on_missing_sound( audio );

        switch ( audio.type( ) )
        {
            case sound::sound_type::sound_effect: if ( auto const& [instance, channel] = sound_resources_.at(
                    audio.sound_mark( ) ); channel.has_value( ) )
                {
                    return Mix_Playing( channel.value( ) );
                }

            case sound::sound_type::sound_track: if ( not current_track_ptr_ ) { return false; }
                return audio.sound_mark( ) == current_track_ptr_->instance->sound_mark( ) && Mix_PlayingMusic( );
        }
        return false;
    }


    auto sdl_sound_service::is_paused( audio const& audio ) const -> bool
    {
        assert_on_missing_sound( audio );

        switch ( audio.type( ) )
        {
            case sound::sound_type::sound_effect: if ( auto const& [instance, channel] = sound_resources_.at(
                    audio.sound_mark( ) ); channel.has_value( ) )
                {
                    return Mix_Paused( channel.value( ) );
                }

            case sound::sound_type::sound_track: if ( not current_track_ptr_ ) { return false; }
                return audio.sound_mark( ) == current_track_ptr_->instance->sound_mark( ) && Mix_PausedMusic( );
        }
        return false;
    }


    auto sdl_sound_service::current_track( ) const -> audio const*
    {
        return current_track_ptr_ ? current_track_ptr_->instance.get( ) : nullptr;
    }


    auto sdl_sound_service::set_master_volume( float const volume ) -> void
    {
        master_volume_ = std::clamp( volume, 0.f, 1.f );
        Mix_Volume( -1, static_cast<int>( master_volume_ * MIX_MAX_VOLUME ) );
        Mix_VolumeMusic( static_cast<int>( master_volume_ * MIX_MAX_VOLUME ) );
    }


    auto sdl_sound_service::master_volume( ) const -> float
    {
        return master_volume_;
    }


    auto sdl_sound_service::set_volume_by_tag( earmark const tag_mark, float const volume ) -> void
    {
        tag_volumes_.at( tag_mark ) = std::clamp( volume, 0.f, 1.f );
    }


    auto sdl_sound_service::volume_by_tag( earmark const tag_mark ) const -> float
    {
        assert_on_missing_tag( tag_mark );
        return tag_volumes_.at( tag_mark );
    }


    auto sdl_sound_service::assert_on_missing_sound( [[maybe_unused]] audio const& audio ) const -> void
    {
        assert( sound_resources_.contains( audio.sound_mark( ) ) && "Sound not registered!" );
    }


    auto sdl_sound_service::assert_on_missing_tag( [[maybe_unused]] earmark const tag_mark ) const -> void
    {
        assert( tag_volumes_.contains( tag_mark ) && "Tag not registered!" );
    }


    auto sdl_sound_service::unload_unused_resources( ) -> void
    {
        // for ( auto it = sound_resources_.begin( ); it != sound_resources_.end( ); )
        // {
        //     if ( auto& instance = it->second.instance; instance.use_count( ) == 1 )
        //     {
        //         if ( is_playing( *instance ) )
        //         {
        //             // if the sound is still being played, we defer until playback is finished
        //             std::thread{
        //                 [&, this]( const Audio& audio )
        //                     {
        //                         while ( is_playing( audio ) )
        //                         {
        //                             std::this_thread::sleep_for( std::chrono::milliseconds( 100 ) );
        //                         }
        //                         RESOURCE_MANAGER.signal_lifetime_event( event::LifetimeEvent::UNLOAD_AUDIO );
        //                     },
        //                 std::ref( *instance )
        //             }.detach( );
        //         }
        //         else
        //         {
        //             it = sound_resources_.erase( it );
        //             continue;
        //         }
        //     }
        //     ++it;
        // }
    }


    auto sdl_sound_service::find_channel( ) const -> int
    {
        // TODO: Correctly implement channel replacement policies
        switch ( policy_ )
        {
            case sound::queue_policy::replace_oldest: return last_used_channel_;

            case sound::queue_policy::replace_newest: return last_used_channel_;

            default: case sound::queue_policy::discard: return -1;
        }
    }


    auto sdl_sound_service::handle_mixer_result( int const result ) -> void
    {
        if ( result == -1 )
        {
            throw std::runtime_error( SDL_GetError( ) );
        }
    }
}
