#ifndef RST_META_REFERENCE_H
#define RST_META_REFERENCE_H

#include <rst/pch.h>


// todo: make this class concept pool
namespace rst::meta
{
    template <typename T>
    concept ref = std::is_reference_v<T>;

    template <typename T>
    concept non_ref = not std::is_reference_v<T>;

    template <typename T>
    concept cv_qualified = std::is_const_v<T>;

    template <typename T>
    concept non_cv_qualified = not std::is_const_v<T>;

    template <typename T>
    concept decayed = std::same_as<T, std::decay_t<T>>;

    template <typename T, typename E>
    concept expected_like = requires( T t, E e ) { T{ std::unexpected<E>{ e } }; };
}


#endif //!RST_META_REFERENCE_H
