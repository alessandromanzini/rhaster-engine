#ifndef RST_SYSTEM_TIMING_H
#define RST_SYSTEM_TIMING_H

#include <rst/pch.h>

#include <rst/meta/__type_traits/enum_traits.h>


namespace rst
{
    /**
     * These represent logical ordering of when systems should run.
     */
    enum class system_timing : std::size_t
    {
        early_tick = 0U, // [VARIABLE]

        fixed_tick,   // [FIXED]
        pre_physics,  // [FIXED]
        post_physics, // [FIXED]

        tick,      // [VARIABLE]
        late_tick, // [VARIABLE]

        pre_render,  // [VARIABLE]
        post_render, // [VARIABLE]
    };


    template <>
    struct meta::enum_traits<system_timing>
    {
        static constexpr auto max_value     = system_timing::post_render;
        static constexpr std::size_t count  = std::to_underlying( max_value ) + 1U;
        static constexpr bool is_sequential = true;

        // check that values are sequential and start from 0
        static_assert( std::to_underlying( system_timing::early_tick ) == 0U );
        static_assert( count == 8U );
    };


    namespace internal
    {
        struct system_timing_to_index
        {
            constexpr auto operator()( system_timing const timing ) const noexcept -> std::size_t
            {
                return std::to_underlying( timing );
            }
        };
    }
}


#endif //!RST_SYSTEM_TIMING_H
