#ifndef RST_TYPE_HASH_H
#define RST_TYPE_HASH_H

#include <rst/pch.h>

#include <rst/meta/type_name.h>


// TODO: add documentation
namespace rst::meta::hash
{
    // +--------------------------------+
    // | HASH TYPE                      |
    // +--------------------------------+
    using hash_type = uint64_t;


    // +--------------------------------+
    // | HASH FUNCTIONS                 |
    // +--------------------------------+
    namespace internal
    {
        class xxhash64 final
        {
        public:
            constexpr explicit xxhash64( size_t const hash_size ) noexcept : hash_{ prime64_5_ + hash_size } { }


            template <typename THasher> requires std::integral<THasher>
            constexpr auto step( THasher hasher ) noexcept -> xxhash64&
            {
                hash_ ^= hasher;
                hash_ = ( hash_ << 27ULL | hash_ >> 37ULL ) * prime64_1_ + prime64_4_;
                return *this;
            }


            constexpr auto yield( ) const noexcept -> hash_type
            {
                hash_type yielded = hash_;
                yielded ^= yielded >> 33ULL;
                yielded *= prime64_2_;
                yielded ^= yielded >> 29ULL;
                yielded *= prime64_3_;
                yielded ^= yielded >> 32ULL;
                return yielded;
            }

        private:
            // xxHash64 constants
            static constexpr hash_type prime64_1_{ 0x9E3779B185EBCA87ULL };
            static constexpr hash_type prime64_2_{ 0xC2B2AE3D27D4EB4FULL };
            static constexpr hash_type prime64_3_{ 0x165667B19E3779F9ULL };
            static constexpr hash_type prime64_4_{ 0x85EBCA77C2B2AE63ULL };
            static constexpr hash_type prime64_5_{ 0x27D4EB2F165667C5ULL };

            hash_type hash_;
        };
    }


    // +--------------------------------+
    // | COMPILE TIME HASH CASTING      |
    // +--------------------------------+
    template <typename T> requires std::is_integral_v<std::decay_t<T>> || std::is_enum_v<std::decay_t<T>>
    [[nodiscard]] constexpr auto hash_cast( T&& castee ) -> hash_type
    {
        using hashee_type = uint64_t;
        return internal::xxhash64( sizeof( hashee_type ) ).step( static_cast<hashee_type>( castee ) ).yield( );
    }


    [[nodiscard]] constexpr auto hash_cast( std::string_view const& view ) -> hash_type
    {
        using hashee_type = uint64_t;
        internal::xxhash64 hash_operator{ view.size( ) };
        for ( char const character : view )
        {
            hash_operator.step( static_cast<hashee_type>( character ) );
        }
        return hash_operator.yield( );
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
