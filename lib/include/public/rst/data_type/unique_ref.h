// ReSharper disable CppNonExplicitConvertingConstructor
#ifndef RST_DYNAMIC_REF_H
#define RST_DYNAMIC_REF_H

#include <rst/pch.h>


namespace rst
{
    /**
     * @brief A unique owning smart pointer with const-correct accessors for RAII resource management.
     * 
     * Similar to std::unique_ptr but designed with const-correctness in mind for the engine's
     * component system. Provides exclusive ownership semantics with move-only behavior.
     * 
     * @tparam T The type of object to manage
     * @tparam TDeleter Deleter type for resource cleanup, defaults to std::default_delete<T>
     */
    template <typename T, typename TDeleter = std::default_delete<T>>
    class unique_ref final
    {
    public:
        using value_type = std::decay_t<T>;

        using pointer_type       = value_type*;
        using const_pointer_type = value_type const*;

        using reference_type       = value_type&;
        using const_reference_type = value_type const&;

#pragma region ctor_dtor
        /**
         * @brief Default constructor creating an empty unique_ref.
         */
        unique_ref( ) : unique_ref{ nullptr } { }

        /**
         * Constructs an empty unique_ref from nullptr.
         */
        explicit unique_ref( std::nullptr_t ) : ptr_{ nullptr } { }

        /**
         * @brief Constructs a unique_ref taking ownership of the given pointer.
         * @param ptr Raw pointer to take ownership of
         */
        explicit unique_ref( value_type* ptr ) : ptr_{ ptr } { }


        /**
         * @brief Destructor that automatically deletes the managed object.
         * @complexity O(1)
         */
        ~unique_ref( ) noexcept(std::is_nothrow_destructible_v<value_type>)
        {
            if ( has_value( ) )
            {
                deleter_( ptr_ );
            }
        }


        /**
         * @brief Move constructor for convertible types.
         * @tparam U Source type that must be convertible to T*
         * @tparam UDeleter Source deleter type
         * @param other The unique_ref to move from
         */
        template <typename U, typename UDeleter> requires std::convertible_to<U*, T*>
        unique_ref( unique_ref<U, UDeleter>&& other ) noexcept : ptr_{ other.release( ) } { }


        /**
         * @brief Move constructor that transfers ownership.
         * @param other The unique_ref to move from
         */
        unique_ref( unique_ref&& other ) noexcept : ptr_{ other.release( ) } { }

        /**
         * @brief Move assignment operator that transfers ownership.
         * @param other The unique_ref to move from
         * @return Reference to this unique_ref
         */
        auto operator=( unique_ref&& other ) noexcept -> unique_ref&
        {
            if ( this != &other )
            {
                reset( other.release( ) );
            }
            return *this;
        }


        // explicitly deleted copy operations to enforce move-only semantics
        unique_ref( unique_ref const& )                    = delete;
        auto operator=( unique_ref const& ) -> unique_ref& = delete;
#pragma endregion


#pragma region accessors
        /**
         * @brief Gets a mutable reference to the managed object.
         * @complexity O(1)
         * @return Mutable reference to the managed object
         * @note Asserts if the unique_ref doesn't hold a value
         */
        [[nodiscard]] auto value( ) noexcept -> reference_type { return *assert_get( ); }

        /**
         * @brief Gets a const reference to the managed object.
         * @complexity O(1)
         * @return Const reference to the managed object
         * @note Asserts if the unique_ref doesn't hold a value
         */
        [[nodiscard]] auto value( ) const noexcept -> const_reference_type { return *assert_get( ); }

        /**
         * @brief Dereference operator for mutable access.
         * @complexity O(1)
         * @return Mutable reference to the managed object
         */
        [[nodiscard]] auto operator*( ) noexcept -> reference_type { return *assert_get( ); }

        /**
         * @brief Dereference operator for const access.
         * @complexity O(1)
         * @return Const reference to the managed object
         */
        [[nodiscard]] auto operator*( ) const noexcept -> const_reference_type { return *assert_get( ); }

        /**
         * @brief Member access operator for mutable access.
         * @complexity O(1)
         * @return Mutable pointer to the managed object
         */
        [[nodiscard]] auto operator->( ) noexcept -> pointer_type { return assert_get( ); }

        /**
         * @brief Member access operator for const access.
         * @complexity O(1)
         * @return Const pointer to the managed object
         */
        [[nodiscard]] auto operator->( ) const noexcept -> const_pointer_type { return assert_get( ); }

        /**
         * @brief Checks if the unique_ref holds a valid object.
         * @complexity O(1)
         * @return True if it holds a value, false otherwise
         */
        [[nodiscard]] auto has_value( ) const noexcept -> bool { return ptr_ != nullptr; }


        /**
         * @return T*'s deleter.
         */
        [[nodiscard]] auto deleter( ) const noexcept -> TDeleter { return deleter_; }
#pragma endregion


#pragma region mutators
        /**
         * @brief Releases ownership without destroying the managed object.
         * @complexity O(1)
         * @return Raw pointer to the previously managed object
         */
        [[nodiscard]] auto release( ) noexcept -> pointer_type { return std::exchange( ptr_, nullptr ); }

        /**
         * @brief Destroys the currently owned object and resets to empty state.
         * @complexity O(1)
         */
        auto reset( ) noexcept -> void
        {
            if ( has_value( ) )
            {
                deleter_( ptr_ );
                ptr_ = nullptr;
            }
        }


        /**
         * @brief Destroys the current object and takes ownership of a new one.
         * @complexity O(1)
         * @param ptr Raw pointer to take ownership of
         */
        auto reset( pointer_type ptr ) noexcept -> void
        {
            if ( ptr_ != ptr )
            {
                if ( has_value( ) )
                {
                    deleter_( ptr_ );
                }
                ptr_ = ptr;
            }
        }
#pragma endregion

    private:
        pointer_type ptr_;
        [[no_unique_address]] TDeleter deleter_{};

        /**
         * @brief Internal helper that asserts validity before returning pointer.
         * @complexity O(1)
         * @return Raw pointer to managed object
         * @note Asserts if ptr_ is null
         */
        [[nodiscard]] auto assert_get( ) const -> pointer_type
        {
            assert( has_value( ) && "unique_ref::assert_get: dereferencing null pointer!" );
            return ptr_;
        }
    };


    namespace ref
    {
        /**
         * @brief Creates a unique_ref managing a new object of type T.
         * @complexity O(1) + T's construction time
         * @tparam T The type of object to construct
         * @tparam TArgs Constructor parameter types
         * @param args Constructor arguments to forward
         * @return A unique_ref managing the newly created object
         */
        template <typename T, typename... TArgs> requires std::constructible_from<T, TArgs...>
        auto make_unique( TArgs&&... args ) -> unique_ref<T>
        {
            return unique_ref<T>{ new T{ std::forward<TArgs>( args )... } };
        }
    }
}


#endif //!RST_DYNAMIC_REF_H
