#ifndef RST_FUNCTION_TRAITS_H
#define RST_FUNCTION_TRAITS_H

#include <functional>


namespace rst::meta
{
    // +--------------------------------+
    // | FUNCTION TRAITS                |
    // +--------------------------------+
    template <typename T>
    struct function_traits;

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


    // +--------------------------------+
    // | RAW                            |
    // +--------------------------------+
    template <typename TReturn, typename... TParams>
    struct function_traits<TReturn ( * )( TParams... )> : function_traits_info<TReturn ( * )( TParams... ), TReturn, TParams...> {
    };


    // +--------------------------------+
    // | NON CONST MEMBER               |
    // +--------------------------------+
    template <typename TClass, typename TReturn, typename... TParams>
    struct function_traits<TReturn ( TClass::* )( TParams... )>
            : function_traits_info<TReturn ( TClass::* )( TParams... ), TReturn, TParams...>
    {
        using class_type = TClass;
    };


    // +--------------------------------+
    // | CONST MEMBER                   |
    // +--------------------------------+
    template <typename TClass, typename TReturn, typename... TParams>
    struct function_traits<TReturn ( TClass::* )( TParams... ) const>
            : function_traits_info<TReturn ( TClass::* )( TParams... ) const, TReturn, TParams...>
    {
        using class_type = TClass;
    };


    // +--------------------------------+
    // | STD FUNCTION                   |
    // +--------------------------------+
    template <typename TReturn, typename... TParams>
    struct function_traits<std::function<TReturn( TParams... )>>
            : function_traits_info<TReturn ( * )( TParams... ), TReturn, TParams...> { };


    // +--------------------------------+
    // | FUNCTION SIGNATURE             |
    // +--------------------------------+
    template <typename TReturn, typename... TParams>
    struct function_traits<TReturn( TParams... )> : function_traits_info<TReturn ( * )( TParams... ), TReturn, TParams...> { };


    // +--------------------------------+
    // | MEMBER OF CLASS                |
    // +--------------------------------+
    template <typename TClass, typename TMethod>
    concept member_of_class =
            std::is_member_function_pointer_v<TMethod> && std::same_as<TClass, typename function_traits<TMethod>::class_type>;


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


    // +---------------------------+
    // | BAD CONVERSION            |
    // +---------------------------+
    template <typename TFrom, typename TTo>
    constexpr bool bad_conversion = false;
}


#endif //!RST_FUNCTION_TRAITS_H
