#include <rst/temp/singleton/service_locator.h>


namespace rst
{
    auto service_locator::register_sound_system( std::unique_ptr<rst::sound_system>&& ss ) -> rst::sound_system&
    {
        sound_system_instance_ptr_ = std::move( ss );
        return sound_system( );
    }


    auto service_locator::sound_system( ) const -> rst::sound_system&
    {
        assert( is_sound_system_registered( ) && "SoundSystem instance not registered" );
        return *sound_system_instance_ptr_;
    }


    auto service_locator::is_sound_system_registered( ) const -> bool
    {
        return sound_system_instance_ptr_ != nullptr;
    }


    service_locator& SERVICE_LOCATOR = service_locator::instance( );

}
