// ReSharper disable CppDFAConstantParameter
#ifndef RST_UID_H
#define RST_UID_H

#include <rst/pch.h>

#include <rst/meta/type_hash.h>


namespace rst
{
    // TODO: add debug info when debug mode
    struct Uid final
    {
        constexpr Uid( ) : Uid{ "0" } { }


        template <typename TValue>
        explicit constexpr Uid( TValue value ) : uid{ meta::hash::hash_cast( static_cast<meta::hash::size_hash_t>( value ) ) } { }


        explicit constexpr Uid( char const* cstr ) : Uid{ std::string_view{ cstr } } { }
        explicit constexpr Uid( std::string const& str ) : Uid{ std::string_view{ str } } { }
        explicit constexpr Uid( std::string_view const view ) : uid{ meta::hash::hash_cast( view ) } { }

        explicit operator meta::hash::hash_value_t( ) const { return uid; }

        auto operator==( Uid const& other ) const -> bool { return uid == other.uid; }
        auto operator<( Uid const& other ) const -> bool { return uid < other.uid; }

        meta::hash::hash_value_t uid{};
    };

    namespace uid
    {
        /**
         * Null uid constant. Equivalent to Uid{} or Uid{ "0" }.
         */
        static constexpr Uid NONE{};
    }
}


// +--------------------------------+
// | UID HASH FUNCTION              |
// +--------------------------------+
template <>
struct std::hash<rst::Uid>
{
    auto operator()( rst::Uid const& uid ) const noexcept -> size_t
    {
        return std::hash<rst::meta::hash::hash_value_t>( )( uid.uid );
    }
};


#endif //!RST_UID_H
