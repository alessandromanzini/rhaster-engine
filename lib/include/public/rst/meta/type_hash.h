#ifndef RST_TYPE_HASH_H
#define RST_TYPE_HASH_H

#include <rst/pch.h>


// TODO: add documentation
namespace rst::meta::hash
{
    // +--------------------------------+
    // | HASH TYPES                     |
    // +--------------------------------+
    using size_hash_t  = uint64_t;
    using hash_value_t = uint64_t;

    // +--------------------------------+
    // | TYPE NAME EVALUATION           |
    // +--------------------------------+
    namespace internal
    {
        template <typename TClass>
        [[nodiscard]] consteval auto type_name_impl( ) -> std::string_view
        {
#if defined(__clang__) || defined(__GNUC__)
            constexpr std::string_view function = __PRETTY_FUNCTION__;
            constexpr std::string_view prefix   = "type_name_impl() [TClass = ";
            constexpr std::string_view suffix   = "]";
#elif defined(_MSC_VER)
            constexpr std::string_view function = __FUNCSIG__;
            constexpr std::string_view prefix = "type_name_impl<";
            //constexpr std::string_view prefix = "std::string_view __cdecl type_name<";
            constexpr std::string_view suffix = ">";
#else
#error "Compiler not supported"
#endif
            constexpr std::size_t prefix_pos = function.find( prefix );

            static_assert( prefix_pos != std::string_view::npos, "type_name_impl( ): prefix format is incorrect!" );

            // cleanup
            constexpr std::size_t start = prefix_pos + prefix.size( );
            constexpr std::size_t end   = function.rfind( suffix );
            return function.substr( start, end - start );
        }
    }


    template <typename TClass>
    [[nodiscard]] consteval auto type_name( ) -> std::string_view
    {
        return internal::type_name_impl<std::decay_t<TClass>>( );
    }


    template <typename T>
    [[nodiscard]] consteval auto type_name( T&& ) -> std::string_view
    {
        return type_name<T>( );
    }


    // +--------------------------------+
    // | COMPILE TIME HASH CASTING      |
    // +--------------------------------+
    [[nodiscard]] constexpr auto hash_cast( size_hash_t const castee ) -> hash_value_t
    {
        // simple 64-bit FNV-1a hash
        constexpr hash_value_t FNV_OFFSET_BASIS_64 = 0xCBF29CE484222325;

        // process the integer per byte
        hash_value_t hash = FNV_OFFSET_BASIS_64;
        for ( size_t i{ 0U }; i < sizeof( castee ); ++i )
        {
            constexpr hash_value_t FNV_PRIME_64 = 0x100000001B3;
            hash ^= castee >> ( i * 8U ) & 0xFF; // extract byte
            hash *= FNV_PRIME_64;
        }
        return hash;
    }


    [[nodiscard]] constexpr auto hash_cast( std::string_view const& view ) -> hash_value_t
    {
        // simple 64-bit FNV-1a hash
        constexpr hash_value_t FNV_OFFSET_BASIS_64 = 0xCBF29CE484222325;

        // process the string per character
        hash_value_t hash = FNV_OFFSET_BASIS_64;
        for ( char const character : view )
        {
            constexpr hash_value_t FNV_PRIME_64 = 0x100000001B3;

            hash ^= static_cast<hash_value_t>( character );
            hash *= FNV_PRIME_64;
        }
        return hash;
    }


    // +--------------------------------+
    // | TYPE HASHING UTILITY           |
    // +--------------------------------+
    template <typename T>
    [[nodiscard]] consteval auto type_hash( ) -> hash_value_t
    {
        return hash_cast( type_name<T>( ) );
    }


    template <typename T>
    [[nodiscard]] consteval auto type_hash( T&& ) -> hash_value_t
    {
        return type_hash<T>( );
    }
}


#endif //!RST_TYPE_HASH_H
