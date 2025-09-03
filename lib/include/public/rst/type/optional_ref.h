#ifndef RST_OPTIONAL_REF_H
#define RST_OPTIONAL_REF_H


// TODO: get rid of this class
namespace rst
{
    template <typename TValue>
    class OptionalRef
    {
    public:
        // ReSharper disable CppNonExplicitConvertingConstructor
        OptionalRef( TValue& ref ) : value_ptr_{ &ref } { }
        OptionalRef( TValue* ptr ) : value_ptr_{ ptr } { }
        // ReSharper restore CppNonExplicitConvertingConstructor

        [[nodiscard]] auto has_value( ) const -> bool { return value_ptr_ != nullptr; }

        // ReSharper disable once CppNonExplicitConversionOperator
        operator bool( ) const { return has_value( ); }

        [[nodiscard]] auto value( ) const -> TValue& { return *value_ptr_; }
        [[nodiscard]] auto operator*( ) const -> TValue& { return value( ); }
        [[nodiscard]] auto operator&( ) const -> TValue* { return value_ptr_; }

    private:
        TValue* value_ptr_{ nullptr };
    };
}


#endif //!RST_OPTIONAL_REF_H
