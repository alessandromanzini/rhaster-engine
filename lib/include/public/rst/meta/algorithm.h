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
    // | FIND SMALLEST                  |
    // +--------------------------------+
    template <typename TBase, std::derived_from<TBase>... TRanges>
    [[nodiscard]] constexpr auto find_smallest( TRanges&... ranges ) noexcept -> TBase*
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


    template <typename TBase, std::derived_from<TBase>... TRanges>
    [[nodiscard]] constexpr auto find_smallest( std::tuple<TRanges&...> ranges ) noexcept -> TBase*
    {
        return std::apply( []( TRanges&... unpacked ) { return meta::find_smallest<TBase, TRanges...>( unpacked... ); }, ranges );
    }
}


#endif //!RST_META_ALGORITHM_H
