#ifndef RST_WARREN_ALERT_H
#define RST_WARREN_ALERT_H

#include <rst/pch.h>


namespace rst
{
    auto set_global_alert_stream( std::ostream& os ) noexcept -> void;


    namespace internal
    {
        auto alert_impl( std::string_view message ) noexcept -> void;
    }


    template <typename... TMessageArgs>
    auto alert( std::format_string<TMessageArgs...> fmt, TMessageArgs&&... args ) -> void
    {
        if constexpr ( sizeof...( args ) > 0 )
        {
            internal::alert_impl( std::format( std::move( fmt ), std::forward<TMessageArgs>( args )... ) );
        }
        else
        {
            internal::alert_impl( fmt.get( ) );
        }
    }
}


#endif //!RST_WARREN_ALERT_H
