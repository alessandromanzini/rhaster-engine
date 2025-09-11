#ifndef RST_META_PARAMETER_PACK_TRAITS_H
#define RST_META_PARAMETER_PACK_TRAITS_H

#include <rst/pch.h>


namespace rst::meta
{
    // +--------------------------------+
    // | INDEX OF                       |
    // +--------------------------------+
    template <typename TTarget, typename... TPack>
    struct index_of
    {
    private:
        [[nodiscard]] static consteval auto get_index( ) noexcept -> std::size_t
        {
            static_assert( sizeof...( TPack ) != 0U, "pack must have a size of at least 1!" );
            constexpr bool matches[] = { std::is_same_v<TTarget, TPack>... };
            for ( std::size_t i{}; i < sizeof...( TPack ); ++i )
            {
                if ( matches[i] ) { return i; }
            }
            return sizeof...( TPack );
        }

    public:
        using type = TTarget;
        static constexpr std::size_t value{ get_index( ) };
    };

    template <typename TTarget, typename... TPack>
    inline constexpr std::size_t index_of_v = index_of<TTarget, TPack...>::value;


    // +--------------------------------+
    // | FIRST ELEMENT                  |
    // +--------------------------------+
    template <typename... Ts>
    struct first_element
    {
        static_assert( sizeof...( Ts ) > 0U, "meta::element_of_type: parameter pack cannot be empty!" );
    };

    template <typename TFirst, typename... TRest>
    struct first_element<TFirst, TRest...>
    {
        using type = TFirst;
    };

    template <typename... Ts>
    using first_element_t = first_element<Ts...>::type;


    // +--------------------------------+
    // | FALLBACK                       |
    // +--------------------------------+
    // todo: perhaps need better name
    template <template<typename> class T, typename... Ts>
    struct fallback
    {
        template <typename... Us>
        struct if_empty
        {
            using type = std::conditional_t<sizeof...( Ts ) == 0U, T<Us...>, T<Ts...>>;
        };
    };


    // +--------------------------------+
    // | CONTAINS TYPE                  |
    // +--------------------------------+
    template <typename TTarget, typename... TPack>
    concept contains_type = ( std::is_same_v<TTarget, TPack> || ... );

    template <typename TTarget, typename... TPack>
    concept contains_decay_type = ( std::is_same_v<std::remove_cv_t<TTarget>, std::remove_cv_t<TPack>> || ... );
}


#endif //!RST_META_PARAMETER_PACK_TRAITS_H
