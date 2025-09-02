#include <rst/__service/sound/sdl/sdl_sound_system.h>

#include <rst/temp/singleton/resource_manager.h>
#include <rst/__type/sound_type/sdl_audio.h>

#include <SDL_mixer.h>


namespace rst
{
    // TODO: make assertion engine configurable
    // TODO: assess code
    SdlSoundSystem::SdlSoundSystem( uint8_t const channels, sound::SdlInitInfo info, sound::QueuePolicy const policy )
        : channels_{ channels }
        , policy_{ policy }
    {
        assert( channels_ <= MAX_CHANNELS_ && "Too many channels requested!" );

        Mix_OpenAudio(
            static_cast<int>( info.sample_rate ), MIX_DEFAULT_FORMAT, static_cast<int>( info.channel_type ), info.buffer_size );
        Mix_Init( MIX_INIT_WAVPACK | MIX_INIT_MP3 | MIX_INIT_FLAC );
        Mix_AllocateChannels( channels );
    }


    SdlSoundSystem::~SdlSoundSystem( )
    {
        Mix_CloseAudio( );
        Mix_Quit( );
    }


    auto SdlSoundSystem::get_service_type( ) -> ServiceType
    {
        return ServiceType::SDL;
    }


    auto SdlSoundSystem::load_sound(
        std::filesystem::path const& path, sound::SoundType const type, Uid const tag_id ) -> std::shared_ptr<Audio>
    {
        Uid const sound_id{ path.string( ) };

        // check if the sound is already registered. If not we can register it
        if ( not sound_resources_.contains( sound_id ) )
        {
            sound_resources_[sound_id] = sound::SoundInstance{
                std::make_shared<SdlAudio>( RESOURCE_MANAGER.get_data_path( ) / path, type, sound_id, tag_id )
            };
        }
        else
        {
            assert(
                sound_resources_.at( sound_id ).instance->get_tag_id( ) == tag_id &&
                "Sound already registered with a different tag!" );
        }

        // initialize the tag volume to 1.0f if it doesn't exist
        if ( not tag_volumes_.contains( tag_id ) )
        {
            tag_volumes_[tag_id] = 1.f;
        }

        return sound_resources_.at( sound_id ).instance;
    }


    auto SdlSoundSystem::play( Audio const& audio, float const volume, int const loops ) -> int
    {
        assert_on_missing_sound( audio );

        auto& sound          = sound_resources_.at( audio.get_sound_id( ) );
        auto const* instance = static_cast<SdlAudio*>( sound.instance.get( ) );

        int const mix_volume{ static_cast<int>( MIX_MAX_VOLUME * volume * tag_volumes_.at( audio.get_tag_id( ) ) ) };

        switch ( audio.get_type( ) )
        {
            case sound::SoundType::SOUND_EFFECT:
            {
                auto const effect = std::get<Mix_Chunk*>( instance->get_resource( ) );
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

            case sound::SoundType::SOUND_TRACK:
            {
                auto const track = std::get<Mix_Music*>( instance->get_resource( ) );
                Mix_VolumeMusic( mix_volume );
                Mix_PlayMusic( track, loops );
                current_track_ptr_ = &sound;
                return 0;
            }
        }
        return -1;
    }


    auto SdlSoundSystem::stop( Audio const& audio ) -> bool
    {
        assert_on_missing_sound( audio );

        auto& [instance, channel] = sound_resources_.at( audio.get_sound_id( ) );

        switch ( audio.get_type( ) )
        {
            case sound::SoundType::SOUND_EFFECT:
            {
                auto const result = Mix_HaltChannel( channel.value( ) );
                handle_mixer_result( result );
                channel = std::nullopt;
                return true;
            }

            case sound::SoundType::SOUND_TRACK: if ( current_track_ptr_ )
                {
                    Mix_HaltMusic( );
                    current_track_ptr_ = nullptr;
                    return true;
                }
        }
        return false;
    }


    auto SdlSoundSystem::stop_all( ) -> void
    {
        current_track_ptr_ = nullptr;
        Mix_HaltMusic( );

        Mix_HaltChannel( -1 );
    }


    auto SdlSoundSystem::pause( Audio const& audio ) -> bool
    {
        assert_on_missing_sound( audio );

        switch ( audio.get_type( ) )
        {
            case sound::SoundType::SOUND_EFFECT: if ( auto& [instance, channel] = sound_resources_.at( audio.get_sound_id( ) );
                    channel.has_value( ) )
                {
                    Mix_Pause( channel.value( ) );
                    return true;
                }

            case sound::SoundType::SOUND_TRACK: if ( current_track_ptr_ )
                {
                    Mix_PauseMusic( );
                    return true;
                }
        }
        return false;
    }


    auto SdlSoundSystem::resume( Audio const& audio ) -> bool
    {
        assert_on_missing_sound( audio );

        switch ( audio.get_type( ) )
        {
            case sound::SoundType::SOUND_EFFECT: if ( auto& [instance, channel] = sound_resources_.at( audio.get_sound_id( ) );
                    channel.has_value( ) )
                {
                    Mix_Resume( channel.value( ) );
                    return true;
                }

            case sound::SoundType::SOUND_TRACK: if ( current_track_ptr_ )
                {
                    Mix_ResumeMusic( );
                    return true;
                }
        }
        return false;
    }


    auto SdlSoundSystem::is_playing( Audio const& audio ) const -> bool
    {
        assert_on_missing_sound( audio );

        switch ( audio.get_type( ) )
        {
            case sound::SoundType::SOUND_EFFECT: if ( auto const& [instance, channel] = sound_resources_.at(
                    audio.get_sound_id( ) ); channel.has_value( ) )
                {
                    return Mix_Playing( channel.value( ) );
                }

            case sound::SoundType::SOUND_TRACK: if ( not current_track_ptr_ ) { return false; }
                return audio.get_sound_id( ) == current_track_ptr_->instance->get_sound_id( ) && Mix_PlayingMusic( );
        }
        return false;
    }


    auto SdlSoundSystem::is_paused( Audio const& audio ) const -> bool
    {
        assert_on_missing_sound( audio );

        switch ( audio.get_type( ) )
        {
            case sound::SoundType::SOUND_EFFECT: if ( auto const& [instance, channel] = sound_resources_.at(
                    audio.get_sound_id( ) ); channel.has_value( ) )
                {
                    return Mix_Paused( channel.value( ) );
                }

            case sound::SoundType::SOUND_TRACK: if ( not current_track_ptr_ ) { return false; }
                return audio.get_sound_id( ) == current_track_ptr_->instance->get_sound_id( ) && Mix_PausedMusic( );
        }
        return false;
    }


    auto SdlSoundSystem::get_current_track( ) const -> Audio const*
    {
        return current_track_ptr_ ? current_track_ptr_->instance.get( ) : nullptr;
    }


    auto SdlSoundSystem::set_master_volume( float const volume ) -> void
    {
        master_volume_ = std::clamp( volume, 0.f, 1.f );
        Mix_Volume( -1, static_cast<int>( master_volume_ * MIX_MAX_VOLUME ) );
        Mix_VolumeMusic( static_cast<int>( master_volume_ * MIX_MAX_VOLUME ) );
    }


    auto SdlSoundSystem::get_master_volume( ) const -> float
    {
        return master_volume_;
    }


    auto SdlSoundSystem::set_volume_by_tag( Uid const tag_id, float const volume ) -> void
    {
        tag_volumes_.at( tag_id ) = std::clamp( volume, 0.f, 1.f );
    }


    auto SdlSoundSystem::get_volume_by_tag( Uid const tag_id ) const -> float
    {
        assert_on_missing_tag( tag_id );
        return tag_volumes_.at( tag_id );
    }


    auto SdlSoundSystem::assert_on_missing_sound( [[maybe_unused]] Audio const& audio ) const -> void
    {
        assert( sound_resources_.contains( audio.get_sound_id( ) ) && "Sound not registered!" );
    }


    auto SdlSoundSystem::assert_on_missing_tag( [[maybe_unused]] Uid const tag_id ) const -> void
    {
        assert( tag_volumes_.contains( tag_id ) && "Tag not registered!" );
    }


    auto SdlSoundSystem::unload_unused_resources( ) -> void
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


    auto SdlSoundSystem::find_channel( ) const -> int
    {
        // TODO: Correctly implement channel replacement policies
        switch ( policy_ )
        {
            case sound::QueuePolicy::REPLACE_OLDEST: return last_used_channel_;

            case sound::QueuePolicy::REPLACE_NEWEST: return last_used_channel_;

            default: case sound::QueuePolicy::DISCARD: return -1;
        }
    }


    auto SdlSoundSystem::handle_mixer_result( int const result ) -> void
    {
        if ( result == -1 )
        {
            throw std::runtime_error( SDL_GetError( ) );
        }
    }
}
