#include <service/sound/SdlSoundSystem.h>

#include <singleton/ResourceManager.h>

#include <framework/resource/sound/SdlAudio.h>

#include <SDL_mixer.h>

#include <cassert>
#include <iostream>
#include <thread>


namespace engine
{
    SdlSoundSystem::SdlSoundSystem( const uint8_t channels, sound::SdlInitInfo info, sound::QueuePolicy policy )
        : channels_{ channels }
        , policy_{ policy }
    {
        assert( channels_ <= MAX_CHANNELS_ && "Too many channels requested!" );

        Mix_OpenAudio( static_cast<int>( info.sampleRate ),
                       MIX_DEFAULT_FORMAT,
                       static_cast<int>( info.channelType ),
                       info.bufferSize );
        Mix_Init( MIX_INIT_WAVPACK | MIX_INIT_MP3 | MIX_INIT_FLAC );
        Mix_AllocateChannels( channels );
    }


    SdlSoundSystem::~SdlSoundSystem( )
    {
        Mix_CloseAudio( );
        Mix_Quit( );
    }


    ServiceType SdlSoundSystem::get_service_type( )
    {
        return ServiceType::SDL;
    }


    std::shared_ptr<Audio> SdlSoundSystem::load_sound( const std::filesystem::path& path,
                                                       const sound::SoundType type,
                                                       const UID tagId )
    {
        const UID soundId{ path.string( ) };

        // Check if the sound is already registered. If not we can register it
        if ( not sound_resources_.contains( soundId ) )
        {
            sound_resources_[soundId] = sound::SoundInstance{
                std::make_shared<SdlAudio>( RESOURCE_MANAGER.get_data_path( ) / path,
                                            type, soundId, tagId )
            };
        }
        else
        {
            assert( sound_resources_.at( soundId ).instance->get_tag_id( ) == tagId &&
                "Sound already registered with a different tag!" );
        }

        // Initialize the tag volume to 1.0f if it doesn't exist
        if ( not tag_volumes_.contains( tagId ) )
        {
            tag_volumes_[tagId] = 1.f;
        }

        return sound_resources_.at( soundId ).instance;
    }


    int SdlSoundSystem::play( const Audio& audio, const float volume, const int loops )
    {
        assert_on_missing_sound( audio );

        auto& sound          = sound_resources_.at( audio.get_sound_id( ) );
        const auto* instance = static_cast<SdlAudio*>( sound.instance.get( ) );

        const int mixVolume{ static_cast<int>( MIX_MAX_VOLUME * volume * tag_volumes_.at( audio.get_tag_id( ) ) ) };

        switch ( audio.get_type( ) )
        {
            case sound::SoundType::SOUND_EFFECT:
            {
                const auto effect = std::get<Mix_Chunk*>( instance->get_resource( ) );
                Mix_VolumeChunk( effect, mixVolume );

                // Play the sound effect and store the channel
                const auto channel = Mix_PlayChannel( find_channel( ), effect, loops );
                if ( channel != -1 )
                {
                    sound.channel      = static_cast<uint8_t>( channel );
                    last_used_channel_ = channel;
                }
                return channel;
            }

            case sound::SoundType::SOUND_TRACK:
            {
                const auto track = std::get<Mix_Music*>( instance->get_resource( ) );
                Mix_VolumeMusic( mixVolume );
                Mix_PlayMusic( track, loops );
                current_track_ptr_ = &sound;
                return 0;
            }
        }
        return -1;
    }


    bool SdlSoundSystem::stop( const Audio& audio )
    {
        assert_on_missing_sound( audio );

        auto& [instance, channel] = sound_resources_.at( audio.get_sound_id( ) );

        switch ( audio.get_type( ) )
        {
            case sound::SoundType::SOUND_EFFECT:
            {
                const auto result = Mix_HaltChannel( channel.value( ) );
                handle_mixer_result( result );
                channel = std::nullopt;
                return true;
            }

            case sound::SoundType::SOUND_TRACK:
                if ( current_track_ptr_ )
                {
                    Mix_HaltMusic( );
                    current_track_ptr_ = nullptr;
                    return true;
                }
        }
        return false;
    }


    void SdlSoundSystem::stop_all( )
    {
        current_track_ptr_ = nullptr;
        Mix_HaltMusic( );

        Mix_HaltChannel( -1 );
    }


    bool SdlSoundSystem::pause( const Audio& audio )
    {
        assert_on_missing_sound( audio );

        switch ( audio.get_type( ) )
        {
            case sound::SoundType::SOUND_EFFECT:
                if ( auto& [instance, channel] = sound_resources_.at( audio.get_sound_id( ) ); channel.has_value( ) )
                {
                    Mix_Pause( channel.value( ) );
                    return true;
                }

            case sound::SoundType::SOUND_TRACK:
                if ( current_track_ptr_ )
                {
                    Mix_PauseMusic( );
                    return true;
                }
        }
        return false;
    }


    bool SdlSoundSystem::resume( const Audio& audio )
    {
        assert_on_missing_sound( audio );

        switch ( audio.get_type( ) )
        {
            case sound::SoundType::SOUND_EFFECT:
                if ( auto& [instance, channel] = sound_resources_.at( audio.get_sound_id( ) ); channel.has_value( ) )
                {
                    Mix_Resume( channel.value( ) );
                    return true;
                }

            case sound::SoundType::SOUND_TRACK:
                if ( current_track_ptr_ )
                {
                    Mix_ResumeMusic( );
                    return true;
                }
        }
        return false;
    }


    bool SdlSoundSystem::is_playing( const Audio& audio ) const
    {
        assert_on_missing_sound( audio );

        switch ( audio.get_type( ) )
        {
            case sound::SoundType::SOUND_EFFECT:
                if ( const auto& [instance, channel] = sound_resources_.at( audio.get_sound_id( ) ); channel.has_value( ) )
                {
                    return Mix_Playing( channel.value( ) );
                }

            case sound::SoundType::SOUND_TRACK:
                if ( not current_track_ptr_ ) { return false; }
                return audio.get_sound_id( ) == current_track_ptr_->instance->get_sound_id( ) && Mix_PlayingMusic( );
        }
        return false;
    }


    bool SdlSoundSystem::is_paused( const Audio& audio ) const
    {
        assert_on_missing_sound( audio );

        switch ( audio.get_type( ) )
        {
            case sound::SoundType::SOUND_EFFECT:
                if ( const auto& [instance, channel] = sound_resources_.at( audio.get_sound_id( ) ); channel.has_value( ) )
                {
                    return Mix_Paused( channel.value( ) );
                }

            case sound::SoundType::SOUND_TRACK:
                if ( not current_track_ptr_ ) { return false; }
                return audio.get_sound_id( ) == current_track_ptr_->instance->get_sound_id( ) && Mix_PausedMusic( );
        }
        return false;
    }


    const Audio* SdlSoundSystem::get_current_track( ) const
    {
        return current_track_ptr_ ? current_track_ptr_->instance.get( ) : nullptr;
    }


    void SdlSoundSystem::set_master_volume( const float volume )
    {
        master_volume_ = std::clamp( volume, 0.f, 1.f );
        Mix_Volume( -1, static_cast<int>( master_volume_ * MIX_MAX_VOLUME ) );
        Mix_VolumeMusic( static_cast<int>( master_volume_ * MIX_MAX_VOLUME ) );
    }


    float SdlSoundSystem::get_master_volume( ) const
    {
        return master_volume_;
    }


    void SdlSoundSystem::set_volume_by_tag( const UID tagId, const float volume )
    {
        tag_volumes_.at( tagId ) = std::clamp( volume, 0.f, 1.f );
    }


    float SdlSoundSystem::get_volume_by_tag( const UID tagId ) const
    {
        assert_on_missing_tag( tagId );
        return tag_volumes_.at( tagId );
    }


    void SdlSoundSystem::assert_on_missing_sound( [[maybe_unused]] const Audio& audio ) const
    {
        assert( sound_resources_.contains( audio.get_sound_id( ) ) && "Sound not registered!" );
    }


    void SdlSoundSystem::assert_on_missing_tag( [[maybe_unused]] const UID tagId ) const
    {
        assert( tag_volumes_.contains( tagId ) && "Tag not registered!" );
    }


    void SdlSoundSystem::handle_mixer_result( const int result ) const
    {
        if ( result == -1 )
        {
            throw std::runtime_error( SDL_GetError( ) );
        }
    }


    void SdlSoundSystem::unload_unused_resources( )
    {
        // for ( auto it = sound_resources_.begin( ); it != sound_resources_.end( ); )
        // {
        //     if ( auto& instance = it->second.instance; instance.use_count( ) == 1 )
        //     {
        //         if ( is_playing( *instance ) )
        //         {
        //             // If the sound is still being played, we defer until playback is finished
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


    int SdlSoundSystem::find_channel( ) const
    {
        // TODO: Correctly implement channel replacement policies
        switch ( policy_ )
        {
            case sound::QueuePolicy::REPLACE_OLDEST:
                return last_used_channel_;

            case sound::QueuePolicy::REPLACE_NEWEST:
                return last_used_channel_;

            default:
            case sound::QueuePolicy::DISCARD:
                return -1;
        }
    }

}
