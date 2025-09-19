#include <rst/__core/__service/service_locator.h>


namespace rst
{
    auto service_locator::sound_service( ) const noexcept -> rst::sound_service&
    {
        assert( is_sound_service_registered( ) && "service_locator::sound_service: instance not registered!" );
        return *sound_service_ptr_;
    }


    auto service_locator::is_sound_service_registered( ) const noexcept -> bool
    {
        return sound_service_ptr_ != nullptr;
    }


    auto service_locator::renderer_service( ) const noexcept -> rst::renderer_service&
    {
        assert( is_renderer_service_registered( ) && "service_locator::renderer_service: instance not registered!" );
        return *renderer_service_ptr_;
    }


    auto service_locator::is_renderer_service_registered( ) const noexcept -> bool
    {
        return renderer_service_ptr_ != nullptr;
    }
}
