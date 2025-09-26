#ifndef RST_WARREN_STARTLE_H
#define RST_WARREN_STARTLE_H

#include <rst/pch.h>


namespace rst
{
    template <typename... TMessageArgs>
    auto startle( std::format_string<TMessageArgs...> fmt, TMessageArgs&&... args ) noexcept(false) -> void
    {
        if constexpr ( sizeof...( args ) > 0U )
        {
            throw std::runtime_error{ std::format( std::move( fmt ), std::forward<TMessageArgs>( args )... ) };
        }
        else
        {
            throw std::runtime_error{ std::string{ fmt.get( ) } };
        }
    }
}


#endif //!RST_WARREN_STARTLE_H
