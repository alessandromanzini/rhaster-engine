#ifndef RST_META_CONCEPTS_H
#define RST_META_CONCEPTS_H

#include <rst/pch.h>


namespace rst::meta
{
    template <typename T, typename E>
    concept expected_like = requires( T t, E e ) { T{ std::unexpected<E>{ e } }; };


    template <typename TDerived, typename... TBases>
    concept derived_from_all = ( std::derived_from<TDerived, TBases> && ... );


    // +--------------------------------+
    // | DECAY NO CV                    |
    // +--------------------------------+
    // todo: move to a better place
    template <typename T>
    struct decay_no_cv
    {
        using type = std::conditional_t<
            std::is_array_v<std::remove_reference_t<T>>,
            std::add_pointer_t<std::remove_extent_t<std::remove_reference_t<T>>>,

            std::conditional_t<
                std::is_function_v<std::remove_reference_t<T>>,
                std::add_pointer_t<std::remove_reference_t<T>>,
                std::remove_reference_t<T>>>;
    };

    template <typename T>
    using decay_no_cv_t = decay_no_cv<T>::type;
}


#endif //!RST_META_CONCEPTS_H
