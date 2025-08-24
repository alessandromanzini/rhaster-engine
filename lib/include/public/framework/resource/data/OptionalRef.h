// ReSharper disable All
#ifndef OPTIONALREF_H
#define OPTIONALREF_H


namespace engine
{
    template <typename value_t>
    class OptionalRef
    {
    public:
        OptionalRef( value_t& ref )
            : value_ptr_{ &ref } { }


        OptionalRef( value_t* ptr )
            : value_ptr_{ ptr } { }


        bool has_value( ) const
        {
            return value_ptr_ != nullptr;
        }


        operator bool( ) const
        {
            return has_value( );
        }


        value_t& value( ) const
        {
            return *value_ptr_;
        }


        value_t& operator*() const
        {
            return value( );
        }


        value_t* operator&() const
        {
            return value_ptr_;
        }


    private:
        value_t* value_ptr_{ nullptr };

    };

}


#endif //!OPTIONALREF_H
