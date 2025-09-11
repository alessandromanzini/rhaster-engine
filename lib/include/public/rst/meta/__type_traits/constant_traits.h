#ifndef RST_META_CONSTANT_TRAITS_H
#define RST_META_CONSTANT_TRAITS_H


namespace rst::meta
{
    // +---------------------------+
    // | BAD CONVERSION            |
    // +---------------------------+
    template <typename TFrom, typename TTo>
    inline constexpr bool bad_conversion_v = false;
}


#endif //!RST_META_CONSTANT_TRAITS_H
