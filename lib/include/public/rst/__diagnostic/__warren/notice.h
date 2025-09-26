#ifndef RST_WARREN_NOTICE_H
#define RST_WARREN_NOTICE_H

#include <rst/pch.h>


namespace rst
{
    auto set_global_notice_stream( std::ostream& os ) noexcept -> void;


    namespace internal
    {
        auto notice_impl( std::string_view message ) noexcept -> void;
    }


    template <typename... TMessageArgs>
    auto notice( std::format_string<TMessageArgs...> fmt, TMessageArgs&&... args ) -> void
    {
        if constexpr ( sizeof...( args ) > 0 )
        {
            internal::notice_impl( std::format( std::move( fmt ), std::forward<TMessageArgs>( args )... ) );
        }
        else
        {
            internal::notice_impl( fmt.get( ) );
        }
    }
}


#endif //!RST_WARREN_NOTICE_H
