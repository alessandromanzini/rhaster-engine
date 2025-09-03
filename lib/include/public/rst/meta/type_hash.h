#ifndef RST_TYPE_HASH_H
#define RST_TYPE_HASH_H

#include <rst/pch.h>


// TODO: add documentation
namespace rst::meta::hash
{
    // +--------------------------------+
    // | HASH TYPE                      |
    // +--------------------------------+
    using hash_type = uint64_t;


    // +--------------------------------+
    // | COMPILE TIME HASH CASTING      |
    // +--------------------------------+
    template <typename T> requires std::is_integral_v<std::decay_t<T>> || std::is_enum_v<std::decay_t<T>>
    [[nodiscard]] constexpr auto hash_cast( T&& castee ) -> hash_type
    {
        using hashee_type = uint64_t;
        auto const hasher = static_cast<hashee_type>( castee );

        // simple 64-bit FNV-1a hash
        constexpr hash_type fnv_offset_basis_64 = 0xCBF29CE484222325;

        // process the integer per byte
        hash_type hash = fnv_offset_basis_64;
        for ( size_t i{ 0U }; i < sizeof( hasher ); ++i )
        {
            constexpr hash_type fnv_prime_64 = 0x100000001B3;
            hash ^= hasher >> ( i * 8U ) & 0xFF; // extract byte
            hash *= fnv_prime_64;
        }
        return hash;
    }


    [[nodiscard]] constexpr auto hash_cast( std::string_view const& view ) -> hash_type
    {
        // simple 64-bit FNV-1a hash
        constexpr hash_type fnv_offset_basis_64 = 0xCBF29CE484222325;

        // process the string per character
        hash_type hash = fnv_offset_basis_64;
        for ( char const character : view )
        {
            constexpr hash_type fnv_prime_64 = 0x100000001B3;

            hash ^= static_cast<hash_type>( character );
            hash *= fnv_prime_64;
        }
        return hash;
    }


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


    template <typename T>
    [[nodiscard]] consteval auto type_name( ) -> std::string_view
    {
        return internal::type_name_impl<std::decay_t<T>>( );
    }


    template <typename T>
    [[nodiscard]] consteval auto type_name( T&& ) -> std::string_view
    {
        return type_name<T>( );
    }


    // +--------------------------------+
    // | TYPE HASH                      |
    // +--------------------------------+
    template <typename T>
    [[nodiscard]] consteval auto type_hash( ) -> hash_type
    {
        return hash::hash_cast( type_name<T>( ) );
    }


    template <typename T>
    [[nodiscard]] consteval auto type_hash( T&& ) -> hash_type
    {
        return hash::type_hash<T>( );
    }
}


#endif //!RST_TYPE_HASH_H
