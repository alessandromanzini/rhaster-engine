#ifndef RST_META_TUPLE_H
#define RST_META_TUPLE_H

#include <rst/pch.h>

#include <rst/meta/algorithm.h>


namespace rst::meta
{
    // +--------------------------------+
    // | IS TUPLE                       |
    // +--------------------------------+
    template <typename T>
    struct is_tuple : std::false_type { };

    template <typename... Args>
    struct is_tuple<std::tuple<Args...>> : std::true_type { };

    template <typename T>
    inline constexpr bool is_tuple_v = is_tuple<T>::value;


    // +--------------------------------+
    // | SAFE PARAM                     |
    // +--------------------------------+
    template <std::size_t index, typename TTuple, typename TDefault = void>
    struct safe_tuple_element
    {
        using type = TDefault;
    };

    template <std::size_t index, typename TTuple, typename TDefault> requires ( index < std::tuple_size_v<TTuple> )
    struct safe_tuple_element<index, TTuple, TDefault>
    {
        using type = std::tuple_element_t<index, TTuple>;
    };

    template <std::size_t index, typename TTuple, typename TDefault = void>
    using safe_tuple_element_t = safe_tuple_element<index, TTuple, TDefault>::type;


    // +--------------------------------+
    // | UNWRAP SINGLE                  |
    // +--------------------------------+
    template <typename T>
    struct unwrap_single
    {
        using type = T;
    };

    /**
     * Single element tuple is unwrapped to the element type.
     * @tparam T
     */
    template <typename T>
    struct unwrap_single<std::tuple<T>>
    {
        using type = T;
    };

    template <typename T>
    using unwrap_single_t = unwrap_single<T>::type;


    // +--------------------------------+
    // | WRAP UNIQUE                    |
    // +--------------------------------+
    template <typename T>
    constexpr decltype(auto) wrap_unique( T&& item )
    {
        if constexpr ( meta::is_tuple_v<T> )
        {
            return item;
        }
        else
        {
            return std::make_tuple( item );
        }
    }


    // +--------------------------------+
    // | ELEMENT OF TYPE                |
    // +--------------------------------+
    template <typename TTarget, typename... TPack>
    constexpr auto element_of_type( std::tuple<TPack...>& tuple ) -> TTarget&
    {
        constexpr auto index = meta::index_of<TTarget, TPack...>( );
        static_assert( index < sizeof...( TPack ) && "type not found!" );
        return std::get<index>( tuple );
    }


    template <typename TTarget, typename... TPack>
    constexpr auto element_of_type( std::tuple<TPack...> const& tuple ) -> TTarget const&
    {
        constexpr auto index = meta::index_of<TTarget, TPack...>( );
        static_assert( index < sizeof...( TPack ) && "type not found!" );
        return std::get<index>( tuple );
    }


    // +--------------------------------+
    // | FIRST ELEMENT                  |
    // +--------------------------------+
    template <typename... Ts>
    struct first_element
    {
        static_assert( sizeof...( Ts ) > 0ULL, "parameter pack cannot be empty!" );
    };

    template <typename TFirst, typename... TRest>
    struct first_element<TFirst, TRest...>
    {
        using type = TFirst;
    };

    template <typename... Ts>
    using first_element_t = first_element<Ts...>::type;
}


#endif //!RST_META_TUPLE_H
