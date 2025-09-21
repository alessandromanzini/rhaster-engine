#ifndef RST_SYSTEM_TIMING_H
#define RST_SYSTEM_TIMING_H

#include <rst/pch.h>

#include <rst/meta/__type_traits/enum_traits.h>


namespace rst
{
    /**
     * @brief Enumeration defining logical execution phases for ECS systems.
     *
     * Organizes system execution into well-defined timing phases to ensure
     * correct order of operations and predictable behavior. Systems are
     * grouped by when they should logically execute in the frame cycle.
     *
     * Phase Categories:
     * - VARIABLE: Frame rate dependent, timing varies with performance
     * - FIXED: Fixed timestep, consistent timing regardless of frame rate
     *
     * Design Philosophy:
     * - Early phases handle input and preparation
     * - Fixed phases ensure deterministic physics simulation
     * - Variable phases handle frame-dependent operations like rendering
     * - Late phases handle cleanup and post-processing
     *
     * Typical Frame Execution Order:
     * 1. early_tick: Input processing, state preparation
     * 2. fixed_tick/physics: Deterministic simulation steps
     * 3. tick/late_tick: Game logic, AI, non-physics updates
     * 4. render phases: Visual processing and display
     */
    enum class system_timing : std::size_t
    {
        early_tick = 0U,  ///< [VARIABLE] Input processing, early frame setup

        fixed_tick,       ///< [FIXED] Fixed timestep game logic updates
        pre_physics,      ///< [FIXED] Physics preparation, constraint setup
        physics,          ///< [FIXED] Physics simulation, collision detection/response
        post_physics,     ///< [FIXED] Physics cleanup, constraint resolution

        tick,             ///< [VARIABLE] Main game logic, AI, gameplay systems
        late_tick,        ///< [VARIABLE] Late updates, dependent system processing

        pre_render,       ///< [VARIABLE] Rendering preparation, culling, sorting
        render,           ///< [VARIABLE] Actual rendering, draw calls submission
        post_render,      ///< [VARIABLE] Post-processing, UI, debug rendering
    };


    /**
     * @brief Specialization of enum_traits for system_timing.
     *
     * Provides compile-time metadata about the system_timing enumeration
     * to enable template-based system scheduler functionality.
     *
     * Properties:
     * - max_value: Highest enumeration value for bounds checking
     * - count: Total number of enumeration values for array sizing
     * - is_sequential: Indicates values are sequential starting from 0
     *
     * @note Static assertions verify enumeration layout assumptions
     */
    template <>
    struct meta::enum_traits<system_timing>
    {
        static constexpr auto max_value     = system_timing::post_render;           ///< Maximum enumeration value
        static constexpr std::size_t count  = std::to_underlying( max_value ) + 1U; ///< Total number of timing phases
        static constexpr bool is_sequential = true;                                 ///< Values are sequential from 0

        // Compile-time verification of enumeration layout
        static_assert( std::to_underlying( system_timing::early_tick ) == 0U );
        static_assert( count == 10U );
    };


    /**
     * @brief Internal utilities for system timing operations.
     */
    namespace internal
    {
        /**
         * @brief Function object converting system_timing to array index.
         *
         * Provides a callable interface for converting enumeration values
         * to array indices, useful for indexing into timing-based containers.
         *
         * Usage:
         * @code
         * system_timing_to_index converter{};
         * std::size_t index = converter(system_timing::render); // Gets index for render phase
         * @endcode
         */
        struct system_timing_to_index
        {
            /**
             * @brief Converts system timing enumeration to array index.
             *
             * @param timing System timing phase to convert
             * @return std::size_t Zero-based index corresponding to timing phase
             *
             * @complexity O(1)
             * @note Result is suitable for direct array indexing
             */
            constexpr auto operator()( system_timing const timing ) const noexcept -> std::size_t
            {
                return std::to_underlying( timing );
            }
        };
    }
}


#endif //!RST_SYSTEM_TIMING_H
