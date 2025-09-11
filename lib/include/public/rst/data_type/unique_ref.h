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
     * Key features:
     * - Automatic memory management with RAII principles.
     * - Move-only semantics to prevent accidental copying.
     * - Const-correct accessors that preserve const-ness.
     * - Custom deleter support for specialized cleanup.
     * - Safe null pointer handling.
     * - Exception-safe operations.
     * 
     * @code
     * // basic usage example
     * unique_ref<int> ptr = ref::make_unique<int>(42);
     * 
     * // access the value
     * if (ptr.has_value()) {
     *     int& value = ptr.value(); // mutable access
     *     value = 100;
     *     
     *     // alternative access methods
     *     int& ref = *ptr;    // dereference operator
     *     // ptr->method();   // member access (if T has methods)
     * }
     * 
     * // move semantics (transfer ownership)
     * unique_ref<int> ptr2 = std::move(ptr); // ptr is now empty
     * assert(!ptr.has_value());
     * assert(ptr2.has_value());
     * 
     * // release ownership (manual memory management)
     * int* raw_ptr = ptr2.release(); // caller owns the pointer
     * delete raw_ptr; // manual cleanup required
     * 
     * // reset with new value
     * ptr2.reset(new int{200}); // old value auto-deleted
     * 
     * // const correctness example
     * unique_ref<int> const const_ptr = ref::make_unique<int>(50);
     * int const& const_ref = const_ptr.value(); // const access only
     * // const_ptr.value() = 60; // compilation error - const correctness preserved
     * @endcode
     * 
     * @tparam T The type of object to manage (must be a complete type).
     * @tparam TDeleter Deleter type for resource cleanup, defaults to std::default_delete<T>.
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

        // +--------------------------------+
        // | CTOR/DTOR                      |
        // +--------------------------------+
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
         * @return Reference to this unique_ref.
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


        // +--------------------------------+
        // | ACCESSORS                      |
        // +--------------------------------+
        /**
         * @brief Gets a mutable reference to the managed object.
         * @complexity O(1)
         * @return Mutable reference to the managed object.
         * @note Asserts if the unique_ref doesn't hold a value
         */
        [[nodiscard]] auto value( ) noexcept -> reference_type { return *assert_get( ); }

        /**
         * @brief Gets a const reference to the managed object.
         * @complexity O(1)
         * @return Const reference to the managed object.
         * @note Asserts if the unique_ref doesn't hold a value
         */
        [[nodiscard]] auto value( ) const noexcept -> const_reference_type { return *assert_get( ); }

        /**
         * @brief Dereference operator for mutable access.
         * @complexity O(1)
         * @return Mutable reference to the managed object.
         */
        [[nodiscard]] auto operator*( ) noexcept -> reference_type { return *assert_get( ); }

        /**
         * @brief Dereference operator for const access.
         * @complexity O(1)
         * @return Const reference to the managed object.
         */
        [[nodiscard]] auto operator*( ) const noexcept -> const_reference_type { return *assert_get( ); }

        /**
         * @brief Member access operator for mutable access.
         * @complexity O(1)
         * @return Mutable pointer to the managed object.
         */
        [[nodiscard]] auto operator->( ) noexcept -> pointer_type { return assert_get( ); }

        /**
         * @brief Member access operator for const access.
         * @complexity O(1)
         * @return Const pointer to the managed object.
         */
        [[nodiscard]] auto operator->( ) const noexcept -> const_pointer_type { return assert_get( ); }

        /**
         * @brief Checks if the unique_ref holds a valid object.
         * @complexity O(1)
         * @return True if it holds a value, false otherwise.
         */
        [[nodiscard]] auto has_value( ) const noexcept -> bool { return ptr_ != nullptr; }


        /**
         * @return T*'s deleter.
         */
        [[nodiscard]] auto deleter( ) const noexcept -> TDeleter { return deleter_; }


        // +--------------------------------+
        // | MUTATORS                       |
        // +--------------------------------+
        /**
         * @brief Releases ownership without destroying the managed object.
         * @complexity O(1)
         * @return Raw pointer to the previously managed object.
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

    private:
        pointer_type ptr_;
        [[no_unique_address]] TDeleter deleter_{};

        /**
         * @brief Internal helper that asserts validity before returning pointer.
         * @complexity O(1)
         * @return Raw pointer to managed object.
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
         * 
         * Factory function that constructs an object of type T with the provided arguments
         * and returns a unique_ref managing it. This is the preferred way to create unique_ref
         * instances as it provides exception safety and clear ownership semantics.
         * 
         * @code
         * // create simple types
         * auto int_ptr = ref::make_unique<int>(42);
         * auto string_ptr = ref::make_unique<std::string>("Hello");
         * 
         * // create complex objects with multiple constructor arguments
         * struct point { float x, y; point(float x, float y) : x{x}, y{y} {} };
         * auto point_ptr = ref::make_unique<point>(10.5f, 20.3f);
         * 
         * // create containers
         * auto vector_ptr = ref::make_unique<std::vector<int>>(5, 100); // 5 elements of value 100
         * @endcode
         * 
         * @complexity O(1) + T's construction time
         * @tparam T The type of object to construct (must be constructible from TArgs)
         * @tparam TArgs Constructor parameter types (automatically deduced)
         * @param args Constructor arguments to forward to T's constructor
         * @return A unique_ref managing the newly created object.
         */
        template <typename T, typename... TArgs> requires std::constructible_from<T, TArgs...>
        auto make_unique( TArgs&&... args ) -> unique_ref<T>
        {
            return unique_ref<T>{ new T{ std::forward<TArgs>( args )... } };
        }
    }
}


#endif //!RST_DYNAMIC_REF_H
