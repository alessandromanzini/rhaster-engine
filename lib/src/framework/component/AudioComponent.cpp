#include <framework/component/AudioComponent.h>

#include <singleton/ResourceManager.h>
#include <singleton/ServiceLocator.h>


namespace engine
{
    AudioComponent::AudioComponent( owner_t& owner, const std::filesystem::path& path,
                                    const sound::SoundType type, const UID tagId )
        : Component( owner )
        , audio_ptr_{ SERVICE_LOCATOR.get_sound_system( ).load_sound( path, type, tagId ) } { }


    AudioComponent::~AudioComponent( ) noexcept
    {
        RESOURCE_MANAGER.signal_lifetime_event( event::LifetimeEvent::UNLOAD_AUDIO, audio_ptr_->get_sound_id(  ) );
    }


    void AudioComponent::set_volume( const float volume )
    {
        volume_ = std::clamp( volume, 0.f, 1.f );
    }


    float AudioComponent::get_volume( ) const
    {
        return volume_;
    }


    void AudioComponent::increase_volume( const float delta )
    {
        set_volume( get_volume(  ) + delta );
    }


    void AudioComponent::play( const int loops ) const
    {
        SERVICE_LOCATOR.get_sound_system( ).play( *audio_ptr_, volume_, loops );
    }


    void AudioComponent::set_playback_on_deletion( )
    {
        playback_on_deletion_ = true;
    }


    void AudioComponent::begin_owner_deletion( )
    {
        if ( playback_on_deletion_)
        {
            play( );
        }
    }

}
