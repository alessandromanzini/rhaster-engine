#ifndef RST_SERVICE_LOCATOR_H
#define RST_SERVICE_LOCATOR_H

#include <rst/pch.h>

#include <rst/meta/type_traits.h>
#include <rst/__core/__service/renderer_service.h>
#include <rst/__core/__service/sound_service.h>
#include <rst/__core/__service/render/unified_render_dispatch.h>


// todo: null service instead of assert
namespace rst
{
    class service_locator final
    {
    public:
        template <std::derived_from<sound_service> T, typename... TArgs> requires std::constructible_from<T, TArgs...>
        auto register_sound_service( TArgs&&... args ) noexcept(std::is_nothrow_constructible_v<T>) -> sound_service&
        {
            assert( !is_sound_service_registered( ) && "service_locator::register_sound_service: instance already registered!" );
            sound_service_ptr_ = std::make_unique<T>( std::forward<TArgs>( args )... );
            return *sound_service_ptr_;
        }


        template <meta::derived_from_all<renderer_service, service::unified_render_dispatch> T, typename... TArgs>
            requires std::constructible_from<T, TArgs...>
        auto register_renderer_service( TArgs&&... args ) noexcept(std::is_nothrow_constructible_v<T>) -> renderer_service&
        {
            assert(
                !is_renderer_service_registered( ) &&
                "service_locator::register_renderer_service: instance already registered!" );
            renderer_service_ptr_ = std::make_unique<T>( std::forward<TArgs>( args )... );
            return *renderer_service_ptr_;
        }


        [[nodiscard]] auto sound_service( ) const noexcept -> sound_service&;
        [[nodiscard]] auto is_sound_service_registered( ) const noexcept -> bool;

        [[nodiscard]] auto renderer_service( ) const noexcept -> renderer_service&;
        [[nodiscard]] auto is_renderer_service_registered( ) const noexcept -> bool;

    private:
        std::unique_ptr<rst::sound_service> sound_service_ptr_{ nullptr };
        std::unique_ptr<rst::renderer_service> renderer_service_ptr_{ nullptr };
    };
}


#endif //!RST_SERVICE_LOCATOR_H
