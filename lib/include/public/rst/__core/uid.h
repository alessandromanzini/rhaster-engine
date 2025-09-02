#ifndef RST_UID_H
#define RST_UID_H

#include <rst/pch.h>

#include <rst/meta/type_hash.h>


namespace rst
{
    // TODO: add debug info when debug mode
    struct UID final
    {
        template <typename TValue>
        explicit constexpr UID( TValue value ) : uid{ meta::hash::hash_cast( static_cast<meta::hash::size_hash_t>( value ) ) } { }


        explicit constexpr UID( char const* cstr ) : uid{ meta::hash::hash_cast( std::string_view{ cstr } ) } { }
        explicit constexpr UID( std::string_view const view ) : uid{ meta::hash::hash_cast( view ) } { }
        explicit constexpr UID( std::string const& str ) : UID( std::string_view{ str } ) { }

        explicit operator meta::hash::hash_value_t( ) const { return uid; }

        auto operator==( UID const& other ) const -> bool { return uid == other.uid; }
        auto operator<( UID const& other ) const -> bool { return uid < other.uid; }

        meta::hash::hash_value_t uid{};
    };

    static constexpr UID NULL_UID{ "0" };
}


// +--------------------------------+
// | UID HASH FUNCTION              |
// +--------------------------------+
template <>
struct std::hash<rst::UID>
{
    auto operator()( rst::UID const& uid ) const noexcept -> size_t
    {
        return std::hash<rst::meta::hash::hash_value_t>( )( uid.uid );
    }
};


#endif //!RST_UID_H
