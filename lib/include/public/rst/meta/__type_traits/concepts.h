#ifndef RST_META_CONCEPTS_H
#define RST_META_CONCEPTS_H

#include <rst/pch.h>


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
