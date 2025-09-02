#ifndef RST_OPTIONAL_REF_H
#define RST_OPTIONAL_REF_H


// TODO: get rid of this class
namespace rst
{
    template <typename value_t>
    class OptionalRef
    {
    public:
        // ReSharper disable CppNonExplicitConvertingConstructor
        OptionalRef( value_t& ref ) : value_ptr_{ &ref } { }
        OptionalRef( value_t* ptr ) : value_ptr_{ ptr } { }
        // ReSharper restore CppNonExplicitConvertingConstructor

        [[nodiscard]] auto has_value( ) const -> bool { return value_ptr_ != nullptr; }

        // ReSharper disable once CppNonExplicitConversionOperator
        operator bool( ) const { return has_value( ); }

        [[nodiscard]] auto value( ) const -> value_t& { return *value_ptr_; }
        [[nodiscard]] auto operator*( ) const -> value_t& { return value( ); }
        [[nodiscard]] auto operator&( ) const -> value_t* { return value_ptr_; }

    private:
        value_t* value_ptr_{ nullptr };
    };
}


#endif //!RST_OPTIONAL_REF_H
