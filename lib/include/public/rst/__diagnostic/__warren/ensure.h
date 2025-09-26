#ifndef RST_WARREN_ENSURE_H
#define RST_WARREN_ENSURE_H

#include <rst/pch.h>

#include <rst/__diagnostic/__warren/alert.h>


namespace rst
{
    namespace internal
    {
        template <typename... TMessageArgs>
        auto ensure_impl(
            bool const condition, std::format_string<TMessageArgs...> fmt, char const* file, int line, char const* func,
            TMessageArgs&&... args ) -> void
        {
            if ( not condition )
            {
                std::string const formatted_msg = std::format( std::move( fmt ), std::forward<TMessageArgs>( args )... );
                alert( "ensure failed at {}:{} in {}: {}", file, line, func, formatted_msg );
                std::abort( );
            }
        }
    }

#ifdef NDEBUG
// ReSharper disable once CppInconsistentNaming
#define ensure(condition, message, ...) ((void)0)
#else
// ReSharper disable once CppInconsistentNaming
#define ensure(condition, message, ...) ::rst::internal::ensure_impl(condition, message, __FILE__, __LINE__, __func__ __VA_OPT__(,) __VA_ARGS__)
#endif
}


#endif //!RST_WARREN_ENSURE_H
