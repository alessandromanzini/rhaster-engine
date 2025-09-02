#include <rst/temp/singleton/service_locator.h>


namespace rst
{
    auto ServiceLocator::register_sound_system( std::unique_ptr<SoundSystem>&& ss ) -> SoundSystem&
    {
        sound_system_instance_ptr_ = std::move( ss );
        return get_sound_system( );
    }


    auto ServiceLocator::get_sound_system( ) const -> SoundSystem&
    {
        assert( is_sound_system_registered( ) && "SoundSystem instance not registered" );
        return *sound_system_instance_ptr_;
    }


    auto ServiceLocator::is_sound_system_registered( ) const -> bool
    {
        return sound_system_instance_ptr_ != nullptr;
    }


    ServiceLocator& SERVICE_LOCATOR = ServiceLocator::get_instance( );

}
