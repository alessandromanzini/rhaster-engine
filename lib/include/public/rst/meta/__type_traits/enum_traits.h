#ifndef RST_META_ENUM_TRAITS_H
#define RST_META_ENUM_TRAITS_H


namespace rst::meta
{
    template <typename E> requires std::is_enum_v<E>
    struct enum_traits
    {
        static_assert( "enum_traits was not specialized for this type!" );
    };
}


#endif //!RST_META_ENUM_TRAITS_H
