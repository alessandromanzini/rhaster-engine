#ifndef RST_META_FUNCTION_TRAITS_H
#define RST_META_FUNCTION_TRAITS_H

#include <rst/pch.h>


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
}


#endif //!RST_META_FUNCTION_TRAITS_H
