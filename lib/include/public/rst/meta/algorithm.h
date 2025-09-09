#ifndef RST_META_ALGORITHM_H
#define RST_META_ALGORITHM_H

#include <rst/pch.h>


namespace rst::meta
{
    // +--------------------------------+
    // | FIND OR                        |
    // +--------------------------------+
    template <typename TRange, typename TPredicate>
    auto find_or_none( TRange&& range, TPredicate&& predicate ) noexcept -> std::ranges::range_value_t<TRange>*
    {
        if ( auto const it = std::ranges::find_if( range, std::forward<TPredicate>( predicate ) );
            it != std::ranges::end( range ) )
        {
            return &*it;
        }
        return nullptr;
    }


    template <typename TRange, typename TPredicate>
    auto find_or_default(
        TRange&& range, TPredicate&& predicate, auto&& default_value ) noexcept -> std::ranges::range_value_t<TRange>*
    {
        if ( auto const it = std::ranges::find_if( range, std::forward<TPredicate>( predicate ) );
            it != std::ranges::end( range ) )
        {
            return &*it;
        }
        return &default_value;
    }


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


    template <typename TTarget, typename... TPack>
    concept contains_type = ( std::is_same_v<TTarget, TPack> || ... );


    // +--------------------------------+
    // | FIND SMALLEST                  |
    // +--------------------------------+
    template <typename TBase, std::derived_from<TBase>... TRanges>
    [[nodiscard]] auto find_smallest( TRanges&... ranges ) noexcept -> TBase*
    {
        if constexpr ( sizeof...( TRanges ) == 0U )
        {
            return nullptr;
        }
        else
        {
            std::array<TBase*, sizeof...( TRanges )> range_arr{ &ranges... };
            return *std::ranges::min_element( range_arr, []( auto const* a, auto const* b ) { return a->size( ) < b->size( ); } );
        }
    }
}


#endif //!RST_META_ALGORITHM_H
