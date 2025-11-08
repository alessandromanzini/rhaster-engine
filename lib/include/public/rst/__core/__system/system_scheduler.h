#ifndef RST_SYSTEM_SCHEDULER_H
#define RST_SYSTEM_SCHEDULER_H

#include <rst/pch.h>

#include <rst/data_type/unique_ref.h>
#include <rst/meta/type_traits.h>
#include <rst/__core/ecs.h>
#include <rst/__core/__system/base_system.h>


namespace rst
{
    /**
     * @brief Manages and schedules execution of ECS systems across timing phases.
     *
     * @tparam THook Enumeration type defining execution timing/phases
     *
     * The system scheduler organizes systems into timing buckets and executes
     * them in a controlled manner. It provides the main execution framework
     * for the ECS architecture, ensuring systems run in the correct order
     * and have access to necessary engine resources.
     *
     * Features:
     * - Template-based design supporting any sequential enum for timing hooks
     * - Dynamic system registration with perfect forwarding of constructor arguments
     * - Batch execution of systems within timing phases
     * - Automatic lifetime management of registered systems
     * - Separation of concerns between system logic and scheduling
     *
     * Design Philosophy:
     * - Systems are grouped by logical execution phases (e.g., physics, render)
     * - Each timing phase can contain multiple systems
     * - Systems within a phase execute in registration order
     * - Scheduler manages system ownership via unique_ref smart pointers
     * - Registry and service locator are provided to all systems
     *
     * Usage:
     * @code
     * system_scheduler<system_timing> scheduler{registry, services};
     *
     * // Register systems for different timing phases
     * scheduler.register_system<physics_system>(system_timing::physics);
     * scheduler.register_system<renderer_system>(system_timing::render);
     * scheduler.register_system<input_system>(system_timing::early_tick);
     *
     * // Execute systems for specific timing phases
     * scheduler.signal_hook(system_timing::early_tick);
     * scheduler.signal_hook(system_timing::physics);
     * scheduler.signal_hook(system_timing::render);
     * @endcode
     */
    template <typename THook> requires meta::enum_traits<THook>::is_sequential
    class system_scheduler final
    {
    public:
        /**
         * @brief Constructs scheduler with ECS registry and service locator.
         *
         * @param registry ECS registry for entity and component management
         * @param locator Service locator for accessing engine services
         *
         * @complexity O(1)
         * @note Stores references to registry and locator for system execution
         */
        explicit system_scheduler( ecs::registry& registry, service_locator const& locator ) noexcept
            : registry_ref_{ registry }
            , service_locator_ref_{ locator } { }


        /**
         * @brief Destructor automatically cleaning up all registered systems.
         *
         * @complexity O(n), where n is total number of registered systems
         */
        ~system_scheduler( ) noexcept = default;

        system_scheduler( system_scheduler const& )                        = delete;
        system_scheduler( system_scheduler&& ) noexcept                    = delete;
        auto operator=( system_scheduler const& ) -> system_scheduler&     = delete;
        auto operator=( system_scheduler&& ) noexcept -> system_scheduler& = delete;


        /**
         * @brief Registers a system for execution during specified timing phase.
         *
         * @tparam T System type (must derive from base_system)
         * @tparam TArgs Constructor argument types for system creation
         * @param hook Timing phase when system should execute
         * @param args Constructor arguments forwarded to system constructor
         *
         * @complexity O(1) amortized (vector insertion)
         * @note System is constructed in-place with perfect forwarding
         * @note Systems within same hook execute in registration order
         *
         * Example:
         * @code
         * scheduler.register_system<movement_system>(system_timing::tick, max_velocity);
         * scheduler.register_system<collision_system>(system_timing::physics);
         * @endcode
         */
        template <std::derived_from<base_system> T, typename... TArgs> requires std::constructible_from<T, TArgs...>
        auto register_system( THook const hook, TArgs&&... args ) noexcept -> void
        {
            systems_[std::to_underlying( hook )].emplace_back( ref::make_unique<T>( std::forward<TArgs>( args )... ) );
        }


        /**
         * @brief Executes all systems registered for specified timing phase.
         *
         * @param hook Timing phase to execute
         *
         * @complexity O(n * s), where n is systems in phase, s is system complexity
         * @note Systems execute in registration order within the phase
         * @note Each system receives registry and service locator references
         *
         * Execution Flow:
         * 1. Lookup systems vector for the specified hook
         * 2. Iterate through systems in registration order
         * 3. Call tick() on each system with registry and service locator
         * 4. Continue until all systems in phase complete
         */
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
