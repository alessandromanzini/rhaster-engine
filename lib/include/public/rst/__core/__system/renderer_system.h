#ifndef RST_SYSTEM_SDL_RENDERER_SYSTEM_H
#define RST_SYSTEM_SDL_RENDERER_SYSTEM_H

#include <rst/pch.h>

#include <rst/__core/__system/base_system.h>


namespace rst::system
{
    /**
     * @brief Concrete system responsible for rendering all drawable entities.
     *
     * Processes entities with rendering components and submits them to the
     * rendering service for display. Handles the ECS side of the rendering
     * pipeline by collecting renderable entities and their transformation data.
     *
     * Responsibilities:
     * - Query entities with drawable components (sprites, textures, text)
     * - Extract transform and rendering data from components
     * - Submit render commands to the renderer service
     * - Handle layer ordering and depth sorting
     * - Manage visibility culling (if implemented)
     *
     * Design:
     * - Operates during the render timing phase of the system scheduler
     * - Uses service locator to access renderer service for actual drawing
     * - Processes entities in a cache-friendly manner via ECS views
     * - Maintains separation between ECS logic and rendering implementation
     *
     * Usage:
     * @code
     * // Registered with system scheduler during engine initialization
     * scheduler.register_system<renderer_system>(system_timing::render);
     *
     * // System automatically processes entities like:
     * auto sprite_entity = registry.create();
     * registry.emplace<transform>(sprite_entity, glm::vec2{100, 200});
     * registry.emplace<sprite_component>(sprite_entity, texture_id, rect);
     * @endcode
     */
    class renderer_system final : public base_system
    {
    public:
        /**
         * @brief Constructs renderer system with appropriate name.
         *
         * @complexity O(1)
         */
        explicit renderer_system( );

        /**
         * @brief Destructor handling any necessary cleanup.
         *
         * @complexity O(1)
         */
        ~renderer_system( ) noexcept override;

        renderer_system( renderer_system const& )                        = delete;
        renderer_system( renderer_system&& ) noexcept                    = delete;
        auto operator=( renderer_system const& ) -> renderer_system&     = delete;
        auto operator=( renderer_system&& ) noexcept -> renderer_system& = delete;

        /**
         * @brief Executes rendering logic for one frame.
         *
         * @param registry ECS registry containing entities and components
         * @param locator Service locator for accessing renderer service
         *
         * @complexity O(n), where n is the number of drawable entities
         *
         * Process:
         * 1. Query registry for entities with rendering components
         * 2. Extract transform and visual data from components
         * 3. Sort entities by layer/depth if necessary
         * 4. Submit render commands to renderer service
         * 5. Handle any post-processing or cleanup
         *
         * @note Called by system scheduler during render timing phase
         * @note Accesses renderer service through service locator
         */
        auto tick( ecs::registry& registry, service_locator const& locator ) noexcept -> void override;
    };
}


#endif //!RST_SYSTEM_SDL_RENDERER_SYSTEM_H
