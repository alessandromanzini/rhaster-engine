#ifndef RST_BASE_SYSTEM_H
#define RST_BASE_SYSTEM_H

#include <rst/pch.h>

#include <rst/__core/__ecs/registry.h>
#include <rst/__core/__service/service_locator.h>


namespace rst
{
    class base_system
    {
    public:
        explicit base_system( char const* name ) noexcept : name_{ name } { }
        virtual ~base_system( ) = default;

        base_system( base_system const& )                        = delete;
        base_system( base_system&& ) noexcept                    = delete;
        auto operator=( base_system const& ) -> base_system&     = delete;
        auto operator=( base_system&& ) noexcept -> base_system& = delete;

        [[nodiscard]] auto name( ) const noexcept -> std::string_view const& { return name_; }

        virtual auto tick( ecs::registry& registry, service_locator const& locator ) noexcept -> void = 0;

    private:
        std::string_view const name_;
    };
}


#endif //!RST_BASE_SYSTEM_H
