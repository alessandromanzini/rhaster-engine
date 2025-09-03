// ReSharper disable CppDFAConstantParameter
#ifndef RST_UID_H
#define RST_UID_H

#include <rst/pch.h>

#include <rst/meta/type_hash.h>


namespace rst
{
    // TODO: add debug info when debug mode
    // TODO: add enum reflection to differentiate different enums to be dissimilar (maybe magic enum) --> create enum meta lib and add vulkan proj features as well
    struct earmark final
    {
        constexpr earmark( ) : earmark{ "0" } { }
        explicit constexpr earmark( char const* const cstr ) : earmark{ std::string_view{ cstr } } { }
        explicit constexpr earmark( std::string const& str ) : earmark{ std::string_view{ str } } { }
        explicit constexpr earmark( std::string_view const view ) : value{ meta::hash::hash_cast( view ) } { }
        template <typename T> explicit constexpr earmark( T value ) : value{ meta::hash::hash_cast( value ) } { }

        explicit operator meta::hash::hash_type( ) const { return value; }

        auto operator==( earmark const& other ) const -> bool { return value == other.value; }
        auto operator<( earmark const& other ) const -> bool { return value < other.value; }

        meta::hash::hash_type value{};

        /**
         * Null mark constant. Equivalent to mark{} or mark{ "0" }.
         */
        static earmark const null;
    };
}


// +--------------------------------+
// | UID HASH FUNCTION              |
// +--------------------------------+
template <>
struct std::hash<rst::earmark>
{
    auto operator()( rst::earmark const& mark ) const noexcept -> size_t
    {
        return std::hash<rst::meta::hash::hash_type>( )( mark.value );
    }
};


#endif //!RST_UID_H
