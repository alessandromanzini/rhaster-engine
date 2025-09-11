#ifndef RST_TYPE_NAME_H
#define RST_TYPE_NAME_H

#include <rst/pch.h>

#include <rst/meta/type_traits.h>


namespace rst::meta
{
    // +--------------------------------+
    // | TYPE NAME                      |
    // +--------------------------------+
    namespace internal
    {
        template <typename T>
        [[nodiscard]] consteval auto type_name_impl( ) -> std::string_view
        {
#if defined(__clang__) || defined(__GNUC__)
            constexpr std::string_view function = __PRETTY_FUNCTION__;
            constexpr std::string_view prefix   = "type_name_impl() [T = ";
            constexpr std::string_view suffix   = "]";
#elif defined(_MSC_VER)
            constexpr std::string_view function = __FUNCSIG__;
            constexpr std::string_view prefix   = "type_name_impl<";
            //constexpr std::string_view prefix = "std::string_view __cdecl type_name<";
            constexpr std::string_view suffix = ">";
#else
#error "Compiler not supported"
#endif
            constexpr std::size_t prefix_pos = function.find( prefix );

            // ReSharper disable once CppStaticAssertFailure
            static_assert( prefix_pos != std::string_view::npos, "type_name_impl( ): prefix format is incorrect!" );

            // cleanup
            constexpr std::size_t start = prefix_pos + prefix.size( );
            constexpr std::size_t end   = function.rfind( suffix );
            return function.substr( start, end - start );
        }
    }


    template <typename T, bool cv_elision = true>
    [[nodiscard]] consteval auto type_name( ) -> std::string_view
    {
        if constexpr ( cv_elision )
        {
            return internal::type_name_impl<std::decay_t<T>>( );
        }
        else
        {
            return internal::type_name_impl<decay_no_cv_t<T>>( );
        }
    }


    template <typename T>
    [[nodiscard]] consteval auto type_name( T&& ) -> std::string_view
    {
        return type_name<T>( );
    }
}


#endif //!RST_TYPE_NAME_H