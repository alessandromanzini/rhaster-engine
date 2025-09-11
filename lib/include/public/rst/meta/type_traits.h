#ifndef RST_TYPE_TRAITS_H
#define RST_TYPE_TRAITS_H

#include <functional>


// todo: move all traits in here
namespace rst::meta
{
    // +--------------------------------+
    // | FUNCTION TRAITS                |
    // +--------------------------------+
    template <typename T>
    struct function_traits;

    namespace internal
    {
        template <typename TRawFn, typename TReturn, typename... TParams>
        struct function_traits_info
        {
            using return_t = TReturn;
            using params_t = std::tuple<TParams...>;

            using raw_fn_t = TRawFn;

            using sig_fn_t = TReturn( TParams... );
            using std_fn_t = std::function<sig_fn_t>;

            static constexpr std::size_t arity = sizeof...( TParams );
        };
    }


    // +--------------------------------+
    // | RAW                            |
    // +--------------------------------+
    template <typename TReturn, typename... TParams>
    struct function_traits<TReturn ( * )( TParams... )>
            : internal::function_traits_info<TReturn ( * )( TParams... ), TReturn, TParams...> { };


    // +--------------------------------+
    // | NON CONST MEMBER               |
    // +--------------------------------+
    template <typename TClass, typename TReturn, typename... TParams>
    struct function_traits<TReturn ( TClass::* )( TParams... )>
            : internal::function_traits_info<TReturn ( TClass::* )( TParams... ), TReturn, TParams...>
    {
        using class_type = TClass;
    };


    // +--------------------------------+
    // | CONST MEMBER                   |
    // +--------------------------------+
    template <typename TClass, typename TReturn, typename... TParams>
    struct function_traits<TReturn ( TClass::* )( TParams... ) const>
            : internal::function_traits_info<TReturn ( TClass::* )( TParams... ) const, TReturn, TParams...>
    {
        using class_type = TClass;
    };


    // +--------------------------------+
    // | STD FUNCTION                   |
    // +--------------------------------+
    template <typename TReturn, typename... TParams>
    struct function_traits<std::function<TReturn( TParams... )>>
            : internal::function_traits_info<TReturn ( * )( TParams... ), TReturn, TParams...> { };


    // +--------------------------------+
    // | FUNCTION SIGNATURE             |
    // +--------------------------------+
    template <typename TReturn, typename... TParams>
    struct function_traits<TReturn( TParams... )>
            : internal::function_traits_info<TReturn ( * )( TParams... ), TReturn, TParams...> { };


    // +--------------------------------+
    // | MEMBER OF CLASS                |
    // +--------------------------------+
    template <typename TMethod, typename TClass>
    concept method_of_class =
            std::is_member_function_pointer_v<TMethod> && std::same_as<TClass, typename function_traits<TMethod>::class_type>;


    // +--------------------------------+
    // | DECAY NO CV                    |
    // +--------------------------------+
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


    // +---------------------------+
    // | BAD CONVERSION            |
    // +---------------------------+
    template <typename TFrom, typename TTo>
    inline constexpr bool bad_conversion = false;
}


#endif //!RST_TYPE_TRAITS_H
