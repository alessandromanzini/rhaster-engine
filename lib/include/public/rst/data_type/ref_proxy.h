#ifndef RST_REF_PROXY_H
#define RST_REF_PROXY_H

#include <rst/pch.h>

#include <rst/meta/reference.h>


namespace rst::meta
{
    // +--------------------------------+
    // | REF PROXY                      |
    // +--------------------------------+
    /**
     * @brief A lightweight proxy for managing references with consistent interface.
     *
     * The ref_proxy provides a uniform interface for working with references, offering
     * pointer-like semantics while maintaining reference safety. It's useful for generic
     * programming scenarios where references need to be stored or passed around.
     *
     * Key features:
     * - Lightweight wrapper around a reference (stores only a pointer).
     * - Const-correct access patterns.
     * - Implicit conversion back to reference type.
     * - Pointer-like access operators (* and ->).
     * - Compile-time const detection.
     *
     * @code
     * // basic usage with mutable references
     * int value = 42;
     * ref_proxy<int> proxy{value};
     * 
     * // access the referenced value
     * int& ref = *proxy;           // dereference operator
     * ref = 100;
     * 
     * // implicit conversion to reference
     * int& direct_ref = proxy;     // implicit conversion
     * direct_ref = 200;
     * 
     * // pointer-like access (if T has members)
     * struct point { int x, y; };
     * point pt{10, 20};
     * ref_proxy<point> point_proxy{pt};
     * point_proxy->x = 30;         // member access
     * 
     * // const correctness
     * int const const_value = 50;
     * ref_proxy<int const> const_proxy{const_value};
     * // const_proxy->some_mutating_method(); // compilation error
     * 
     * // factory function usage
     * auto proxy2 = make_ref_proxy(value);
     * *proxy2 = 300;
     * @endcode
     *
     * @tparam T The type being referenced. If the type itself is a reference, the reference is stripped.
     */
    template <typename T>
    class ref_proxy final
    {
    public:
        using value_type     = std::remove_reference_t<T>;
        using reference_type = value_type&;
        using pointer_type   = value_type*;

        static constexpr bool is_const = std::is_const_v<T>;

        /**
         * @brief Constructs a ref_proxy from a reference.
         * 
         * @complexity O(1)
         * @param ref The reference to wrap.
         */
        // ReSharper disable once CppNonExplicitConvertingConstructor
        ref_proxy( reference_type ref ) : ptr_{ std::addressof( ref ) } { }

        /**
         * @brief Explicitly converts the proxy back to a reference.
         * 
         * @complexity O(1)
         * @return Reference to the wrapped object.
         */
        explicit operator reference_type( ) const noexcept { return *ptr_; }

        /**
         * @brief Dereference operator providing access to the referenced object.
         * 
         * @complexity O(1)
         * @return Reference to the wrapped object.
         */
        auto operator*( ) const noexcept -> reference_type { return *ptr_; }

        /**
         * @brief Member access operator for pointer-like semantics.
         * 
         * @complexity O(1)
         * @return Pointer to the wrapped object for member access.
         */
        auto operator->( ) const noexcept -> pointer_type { return ptr_; }

    private:
        pointer_type ptr_{ nullptr };
    };


    // +--------------------------------+
    // | ADD REF PROXY                  |
    // +--------------------------------+
    /**
     * @brief Type trait to convert types to their ref_proxy equivalent.
     * 
     * Transforms reference types and reference_wrapper types into appropriate
     * ref_proxy types, enabling consistent reference handling in template code.
     * 
     * @tparam T The type to convert to ref_proxy.
     */
    template <class T>
    struct add_ref_proxy
    {
        using type = ref_proxy<std::remove_reference_t<T>>;
    };

    /**
     * @brief Specialization for std::reference_wrapper types.
     * 
     * Converts std::reference_wrapper<T> to ref_proxy<T>, maintaining
     * the underlying type without additional reference stripping.
     * 
     * @tparam T The wrapped type in std::reference_wrapper<T>.
     */
    template <class T>
    struct add_ref_proxy<std::reference_wrapper<T>>
    {
        using type = ref_proxy<T>;
    };

    /**
     * @brief Convenience alias for add_ref_proxy<T>::type.
     * 
     * @tparam T The type to convert to ref_proxy.
     */
    template <class T>
    using add_ref_proxy_t = add_ref_proxy<T>::type;


    // +--------------------------------+
    // | MAKE REF PROXY                 |
    // +--------------------------------+
    /**
     * @brief Factory function to create a ref_proxy from a reference.
     * 
     * Provides a convenient way to create ref_proxy instances with automatic
     * type deduction, similar to std::make_pair or std::make_tuple.
     * 
     * @code
     * // automatic type deduction
     * int value = 42;
     * auto proxy = make_ref_proxy(value);  // deduces ref_proxy<int>
     * 
     * // const correctness maintained
     * int const const_value = 100;
     * auto const_proxy = make_ref_proxy(const_value);  // deduces ref_proxy<int const>
     * @endcode
     * 
     * @complexity O(1)
     * @tparam T The referenced type (automatically deduced).
     * @param ref The reference to wrap in a ref_proxy.
     * @return A ref_proxy<T> wrapping the provided reference.
     */
    template <non_ref T>
    constexpr auto make_ref_proxy( T& ref ) -> ref_proxy<T>
    {
        return ref_proxy<T>{ ref };
    }
}


#endif //!RST_REF_PROXY_H
