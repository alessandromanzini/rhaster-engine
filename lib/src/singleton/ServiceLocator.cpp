#include <singleton/ServiceLocator.h>

#include <cassert>


namespace engine
{
    ServiceLocator& SERVICE_LOCATOR = ServiceLocator::get_instance( );


    SoundSystem& ServiceLocator::register_sound_system( std::unique_ptr<SoundSystem>&& ss )
    {
        sound_system_instance_ptr_ = std::move( ss );
        return get_sound_system( );
    }


    SoundSystem& ServiceLocator::get_sound_system( ) const
    {
        assert( is_sound_system_registered( ) && "SoundSystem instance not registered" );
        return *sound_system_instance_ptr_;
    }


    bool ServiceLocator::is_sound_system_registered( ) const
    {
        return sound_system_instance_ptr_ != nullptr;
    }

}
