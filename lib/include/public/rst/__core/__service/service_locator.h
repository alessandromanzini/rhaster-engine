#ifndef RST_SERVICE_LOCATOR_H
#define RST_SERVICE_LOCATOR_H

#include <rst/pch.h>

#include <rst/diagnostic.h>
#include <rst/__core/service.h>


namespace rst
{
    namespace detail
    {
        template <typename T, typename TBase>
        concept derived_service = std::derived_from<T, TBase> && std::is_nothrow_destructible_v<T>;
    }


    class service_locator final
    {
    public:
        template <detail::derived_service<sound_service> T, typename... TArgs> requires std::constructible_from<T, TArgs...>
        auto register_sound_service( TArgs&&... args ) noexcept(std::is_nothrow_constructible_v<T>) -> sound_service&
        {
            sound_service_ptr_ = std::make_unique<T>( std::forward<TArgs>( args )... );
            return *sound_service_ptr_;
        }


        [[nodiscard]] auto sound_service( ) const noexcept -> sound_service&
        {
            ensure( is_sound_service_registered( ), "instance not registered!" );
            return *sound_service_ptr_;
        }


        [[nodiscard]] auto is_sound_service_registered( ) const noexcept -> bool
        {
            return sound_service_ptr_ != nullptr;
        }


        template <detail::derived_service<renderer_service> T, typename... TArgs> requires std::constructible_from<T, TArgs...>
        auto register_renderer_service( TArgs&&... args ) noexcept(std::is_nothrow_constructible_v<T>) -> renderer_service&
        {
            renderer_service_ptr_ = std::make_unique<T>( std::forward<TArgs>( args )... );
            return *renderer_service_ptr_;
        }


        [[nodiscard]] auto renderer_service( ) const noexcept -> renderer_service&
        {
            ensure( is_renderer_service_registered( ), "instance not registered!" );
            return *renderer_service_ptr_;
        }


        [[nodiscard]] auto is_renderer_service_registered( ) const noexcept -> bool
        {
            return renderer_service_ptr_ != nullptr;
        }

    private:
        std::unique_ptr<rst::sound_service> sound_service_ptr_{ nullptr };
        std::unique_ptr<rst::renderer_service> renderer_service_ptr_{ nullptr };
    };
}


#endif //!RST_SERVICE_LOCATOR_H
