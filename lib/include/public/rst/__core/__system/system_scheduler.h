#ifndef RST_SYSTEM_SCHEDULER_H
#define RST_SYSTEM_SCHEDULER_H

#include <rst/pch.h>

#include <rst/data_type/unique_ref.h>
#include <rst/meta/type_traits.h>
#include <rst/__core/ecs.h>
#include <rst/__core/__system/base_system.h>


namespace rst
{
    template <typename THook> requires meta::enum_traits<THook>::is_sequential
    class system_scheduler final
    {
    public:
        explicit system_scheduler( ecs::registry& registry, service_locator const& locator ) noexcept
            : registry_ref_{ registry }
            , service_locator_ref_{ locator } { }


        ~system_scheduler( ) noexcept = default;

        system_scheduler( system_scheduler const& )                        = delete;
        system_scheduler( system_scheduler&& ) noexcept                    = delete;
        auto operator=( system_scheduler const& ) -> system_scheduler&     = delete;
        auto operator=( system_scheduler&& ) noexcept -> system_scheduler& = delete;


        template <std::derived_from<base_system> T, typename... TArgs> requires std::constructible_from<T, TArgs...>
        auto register_system( THook const hook, TArgs&&... args ) noexcept -> void
        {
            systems_[std::to_underlying( hook )].emplace_back( ref::make_unique<T>( std::forward<TArgs>( args )... ) );
        }


        auto signal_hook( THook const hook ) noexcept -> void
        {
            for ( auto& system : systems_[std::to_underlying( hook )] )
            {
                system->tick( registry_ref_, service_locator_ref_ );
            }
        }

    private:
        ecs::registry& registry_ref_;
        service_locator const& service_locator_ref_;
        std::array<std::vector<unique_ref<base_system>>, meta::enum_traits<THook>::count> systems_{};
    };
}


#endif //!RST_SYSTEM_SCHEDULER_H
