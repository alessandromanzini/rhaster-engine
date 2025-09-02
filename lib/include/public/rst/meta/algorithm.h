#ifndef RST_META_ALGORITHM_H
#define RST_META_ALGORITHM_H

#include <rst/pch.h>


namespace rst::meta
{
    template <typename TRange, typename TPredicate>
    auto find_or_none( TRange&& range, TPredicate&& predicate ) -> std::ranges::range_value_t<TRange>*
    {
        if ( auto const it = std::ranges::find_if( range, std::forward<TPredicate>( predicate ) );
            it != std::ranges::end( range ) )
        {
            return &*it;
        }
        return nullptr;
    }


    template <typename TRange, typename TPredicate>
    auto find_or_default( TRange&& range, TPredicate&& predicate, auto&& default_value ) -> std::ranges::range_value_t<TRange>*
    {
        if ( auto const it = std::ranges::find_if( range, std::forward<TPredicate>( predicate ) );
            it != std::ranges::end( range ) )
        {
            return &*it;
        }
        return &default_value;
    }
}


#endif //!RST_META_ALGORITHM_H
