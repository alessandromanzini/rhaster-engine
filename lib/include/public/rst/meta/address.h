#ifndef RST_META_ADDRESS_H
#define RST_META_ADDRESS_H

#include <rst/pch.h>


namespace rst::meta
{
    template <typename T> requires std::is_member_function_pointer_v<T>
    [[nodiscard]] auto method_address_of( T method ) noexcept -> uintptr_t
    {
        if constexpr ( sizeof( T ) <= sizeof( uintptr_t ) )
        {
            uintptr_t member_addr{};
            std::memcpy( &member_addr, &method, sizeof( T ) );
            return member_addr;
        }
        else
        {
            std::array<unsigned char, sizeof( T )> bytes{};
            std::memcpy( bytes.data( ), &method, sizeof( T ) );
            return std::hash<std::string_view>{}( std::string_view{ reinterpret_cast<char*>( bytes.data( ) ), sizeof( T ) } );
        }
    }
}


#endif //!RST_META_ADDRESS_H
