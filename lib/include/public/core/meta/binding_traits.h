#ifndef BINDINGTRAITS_H
#define BINDINGTRAITS_H

#include <functional>


namespace engine::meta
{
    // +---------------------------+
    // | FUNCTION TRAITS           |
    // +---------------------------+
    template <typename sig_t>
    struct function_traits;

    template <typename _raw_fn_t, typename _return_t, typename... _params_t>
    struct function_traits_info
    {
        using return_t = _return_t;
        using params_t = std::tuple<_params_t...>;

        using raw_fn_t = _raw_fn_t;

        using sig_fn_t = _return_t( _params_t... );
        using std_fn_t = std::function<sig_fn_t>;

        static constexpr std::size_t ARITY = sizeof...( _params_t );
    };

    // Raw function
    template <typename _return_t, typename... _params_t>
    struct function_traits<_return_t ( * )( _params_t... )>
        : function_traits_info<_return_t ( * )( _params_t... ), _return_t, _params_t...> { };

    // Non-const member
    template <typename _class_t, typename _return_t, typename... _params_t>
    struct function_traits<_return_t ( _class_t::* )( _params_t... )>
        : function_traits_info<_return_t ( _class_t::* )( _params_t... ), _return_t, _params_t...>
    {
        using class_t = _class_t;
    };

    // Const member
    template <typename _class_t, typename _return_t, typename... _params_t>
    struct function_traits<_return_t ( _class_t::* )( _params_t... ) const>
        : function_traits_info<_return_t ( _class_t::* )( _params_t... ) const, _return_t, _params_t...>
    {
        using class_t = _class_t;
    };

    // std::function
    template <typename _return_t, typename... _params_t>
    struct function_traits<std::function<_return_t( _params_t... )>>
        : function_traits_info<_return_t ( * )( _params_t... ), _return_t, _params_t...> { };

    // function signature
    template <typename _return_t, typename... _params_t>
    struct function_traits<_return_t( _params_t... )>
        : function_traits_info<_return_t ( * )( _params_t... ), _return_t, _params_t...> { };


    // +---------------------------+
    // | SAFE PARAM                |
    // +---------------------------+
    template <std::size_t index, typename tuple_t, typename default_t = void>
    struct safe_tuple_element
    {
        using type = default_t;
    };

    template <std::size_t index, typename tuple_t, typename default_t> requires ( index < std::tuple_size_v<tuple_t> )
    struct safe_tuple_element<index, tuple_t, default_t>
    {
        using type = std::tuple_element_t<index, tuple_t>;
    };

    template <std::size_t index, typename tuple_t, typename default_t = void>
    using safe_tuple_element_t = typename safe_tuple_element<index, tuple_t, default_t>::type;

    // +---------------------------+
    // | BAD CONVERSION            |
    // +---------------------------+
    template <typename from_t, typename to_t>
    constexpr bool BAD_CONVERSION = false;

}


#endif //!BINDINGTRAITS_H
