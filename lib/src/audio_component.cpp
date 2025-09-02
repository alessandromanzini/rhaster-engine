#include <rst/temp/framework/component/audio_component.h>

#include <rst/temp/singleton/resource_manager.h>
#include <rst/temp/singleton/service_locator.h>


namespace rst
{
    AudioComponent::AudioComponent( owner_t& owner, std::filesystem::path const& path, sound::SoundType const type, UID const tag_id )
        : Component( owner )
        , audio_ptr_{ SERVICE_LOCATOR.get_sound_system( ).load_sound( path, type, tag_id ) } { }


    AudioComponent::~AudioComponent( ) noexcept
    {
        RESOURCE_MANAGER.signal_lifetime_event( event::LifetimeEvent::UNLOAD_AUDIO, audio_ptr_->get_sound_id(  ) );
    }


    auto AudioComponent::set_volume( float const volume ) -> void
    {
        volume_ = std::clamp( volume, 0.f, 1.f );
    }


    auto AudioComponent::get_volume( ) const -> float
    {
        return volume_;
    }


    auto AudioComponent::increase_volume( float const delta ) -> void
    {
        set_volume( get_volume(  ) + delta );
    }


    auto AudioComponent::play( int const loops ) const -> void
    {
        SERVICE_LOCATOR.get_sound_system( ).play( *audio_ptr_, volume_, loops );
    }


    auto AudioComponent::set_playback_on_deletion( ) -> void
    {
        playback_on_deletion_ = true;
    }


    auto AudioComponent::begin_owner_deletion( ) -> void
    {
        if ( playback_on_deletion_)
        {
            play( );
        }
    }

}
