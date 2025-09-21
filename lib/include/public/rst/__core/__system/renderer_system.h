#ifndef RST_SYSTEM_SDL_RENDERER_SYSTEM_H
#define RST_SYSTEM_SDL_RENDERER_SYSTEM_H

#include <rst/pch.h>

#include <rst/__core/__system/base_system.h>


namespace rst::system
{
    class renderer_system final : public base_system
    {
    public:
        explicit renderer_system( );
        ~renderer_system( ) noexcept override;

        renderer_system( renderer_system const& )                        = delete;
        renderer_system( renderer_system&& ) noexcept                    = delete;
        auto operator=( renderer_system const& ) -> renderer_system&     = delete;
        auto operator=( renderer_system&& ) noexcept -> renderer_system& = delete;

        auto tick( ecs::registry& registry, service_locator const& locator ) noexcept -> void override;
    };
}


#endif //!RST_SYSTEM_SDL_RENDERER_SYSTEM_H
