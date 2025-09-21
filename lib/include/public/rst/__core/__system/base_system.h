#ifndef RST_BASE_SYSTEM_H
#define RST_BASE_SYSTEM_H

#include <rst/pch.h>

#include <rst/__core/__ecs/registry.h>
#include <rst/__core/__service/service_locator.h>


namespace rst
{
    /**
     * @brief Abstract base class for all systems in the ECS architecture.
     *
     * Defines the interface that all systems must implement to participate
     * in the ECS update cycle. Systems operate on entities and components
     * through the registry and access engine services via the service locator.
     *
     * Features:
     * - Pure virtual tick() method for system-specific logic implementation
     * - Named systems for debugging and profiling identification
     * - Non-copyable and non-movable for unique system ownership
     * - Direct access to ECS registry and service locator
     *
     * Design Philosophy:
     * - Systems contain logic, not data (data lives in components)
     * - Systems operate on entities matching specific component patterns
     * - Each system has a single responsibility (e.g., rendering, physics, input)
     * - Systems are scheduled and executed by the system scheduler
     *
     * Usage:
     * @code
     * class movement_system final : public base_system
     * {
     * public:
     *     movement_system() : base_system("MovementSystem") {}
     *
     *     auto tick(ecs::registry& registry, service_locator const& locator) noexcept -> void override
     *     {
     *         // Process entities with transform and velocity components
     *         auto view = registry.view<transform, velocity>();
     *         for (auto entity : view) {
     *             auto& [pos, vel] = view.get<transform, velocity>(entity);
     *             pos.local().translate(vel.delta * time.delta_time());
     *         }
     *     }
     * };
     * @endcode
     */
    class base_system
    {
    public:
        /**
         * @brief Constructs system with a debug name.
         *
         * @param name Human-readable name for debugging and profiling
         *
         * @note Name should be descriptive and unique for identification
         */
        explicit base_system( char const* name ) noexcept : name_{ name } { }

        /**
         * @brief Virtual destructor for proper cleanup of derived systems.
         */
        virtual ~base_system( ) = default;

        base_system( base_system const& )                        = delete;
        base_system( base_system&& ) noexcept                    = delete;
        auto operator=( base_system const& ) -> base_system&     = delete;
        auto operator=( base_system&& ) noexcept -> base_system& = delete;

        /**
         * @brief Gets the system's debug name.
         *
         * @return std::string_view const& Reference to system name
         *
         * @complexity O(1)
         * @note Used for debugging, profiling, and logging purposes
         */
        [[nodiscard]] auto name( ) const noexcept -> std::string_view const& { return name_; }

        /**
         * @brief Executes system logic for one frame/update cycle.
         *
         * @param registry ECS registry containing all entities and components
         * @param locator Service locator for accessing engine services
         *
         * @complexity Depends on system implementation
         * @note Called by system scheduler during appropriate timing phase
         * @note Must be implemented by all derived systems
         *
         * Implementation Guidelines:
         * - Query registry for entities with required components
         * - Process entities and update their components
         * - Access services (renderer, input, etc.) through locator
         * - Keep logic focused on single responsibility
         * - Avoid storing state in the system (use components instead)
         */
        virtual auto tick( ecs::registry& registry, service_locator const& locator ) noexcept -> void = 0;

    private:
        std::string_view const name_; ///< Debug name for system identification
    };
}


#endif //!RST_BASE_SYSTEM_H
