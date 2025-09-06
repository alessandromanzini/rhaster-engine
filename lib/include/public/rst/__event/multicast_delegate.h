#ifndef RST_MULTICAST_DELEGATE_H
#define RST_MULTICAST_DELEGATE_H

#include <rst/pch.h>

#include <rst/data_type/token_generator.h>
#include <rst/meta/address.h>
#include <rst/meta/function_traits.h>
#include <rst/__event/scoped_binding.h>
#include <rst/__event/token.h>


//todo:
//  1. Memory optimization - Could benefit from small-buffer optimization
//  2. Iterator support - No way to iterate over bound delegates
//  3. Callback prioritization - No ordering/priority system
namespace rst
{
    // +--------------------------------+
    // | DELEGATE TRACE                 |
    // +--------------------------------+
    namespace event::internal
    {
        enum class callable_type : uint8_t
        {
            raw_function, member_function, lambda_or_callable
        };

        template <typename... TParams> struct delegate_trace
        {
            using type = std::function<void( TParams... )>;

            callable_type call_type{ callable_type::raw_function };
            uintptr_t underlying_addr{ 0U };
            void* owner{ nullptr };
            type delegate{};
        };
    }


    // +---------------------------+
    // | MULTICAST DELEGATE        |
    // +---------------------------+
    /**
     * @brief A high-performance multicast delegate for event-driven programming with dual binding modes.
     * 
     * Provides a unified interface for managing multiple callbacks with different binding strategies:
     * - Direct binding for function pointers and member functions (optimized vector storage)
     * - Token-based binding for lambdas and callables (lifetime management via tokens)
     * 
     * Features:
     * - O(1) binding and unbinding for all callback types
     * - Cache-friendly broadcasting via packed storage
     * - RAII-compliant resource management
     * - Support for scoped bindings with automatic cleanup
     * - Mixed binding patterns within a single delegate
     * 
     * @tparam TParams Parameter types for the callback signature
     */
    template <typename... TParams>
    class multicast_delegate final
    {
        using delegate_trace_type = event::internal::delegate_trace<TParams...>;
        using delegate_type       = delegate_trace_type::type;

    public:
        using scoped_binding_type   = event::scoped_binding<TParams...>;
        using raw_delegate_sig_type = meta::function_traits<void( TParams... )>::raw_fn_t;

        /**
         * @brief Default constructor creating an empty multicast delegate.
         * @complexity O(1)
         */
        explicit multicast_delegate( ) noexcept = default;

        /**
         * @brief Destructor that cleans up all bound delegates.
         * @complexity O(n) where n is the number of bound delegates
         */
        ~multicast_delegate( ) noexcept = default;

        multicast_delegate( multicast_delegate const& )                        = delete;
        multicast_delegate( multicast_delegate&& ) noexcept                    = delete;
        auto operator=( multicast_delegate const& ) -> multicast_delegate&     = delete;
        auto operator=( multicast_delegate&& ) noexcept -> multicast_delegate& = delete;


        /**
         * @brief Binds a raw function pointer to this delegate.
         * @complexity O(1) amortized
         * @param raw_fn_ptr Function pointer to bind (must not be nullptr)
         * @note Uses optimized vector storage for cache-friendly iteration
         */
        auto bind( raw_delegate_sig_type raw_fn_ptr ) noexcept -> void
        {
            assert( raw_fn_ptr && "multicast_delegate::bind: raw_fn_ptr cannot be nullptr!" );
            delegate_traces_.emplace_back(
                delegate_trace_type{
                    .call_type = event::internal::callable_type::raw_function,
                    .underlying_addr = reinterpret_cast<uintptr_t>( raw_fn_ptr ),
                    .owner = nullptr, .delegate = delegate_type{ raw_fn_ptr }
                } );
        }


        /**
         * @brief Binds a member function to this delegate.
         * @complexity O(1) amortized
         * @tparam TClass The class type that owns the method
         * @tparam TMethod The member function type
         * @param owner Pointer to the object instance (must not be nullptr)
         * @param method Member function pointer to bind
         * @note Uses optimized vector storage and tracks owner for cleanup
         */
        template <typename TClass, typename TMethod> requires meta::member_of_class<TClass, TMethod>
        auto bind( TClass* owner, TMethod method ) noexcept -> void
        {
            assert( owner && "multicast_delegate::bind: owner cannot be nullptr!" );
            delegate_traces_.emplace_back(
                delegate_trace_type{
                    .call_type = event::internal::callable_type::member_function,
                    .underlying_addr = meta::method_address_of( method ),
                    .owner = owner, .delegate = delegate_type{
                        [=]( TParams&&... args ) { std::invoke( method, owner, std::forward<TParams>( args )... ); }
                    }
                } );
        }


        /**
         * @brief Binds a callable object (lambda, functor) with token-based lifetime management.
         * @complexity O(1) average, O(log n) worst case (hash table insertion)
         * @tparam TCallable The callable type
         * @param callable The callable object to bind
         * @return Token for later unbinding via unbind_token()
         * @note Uses hash table storage for token-based management
         */
        template <typename TCallable> requires std::invocable<TCallable, TParams...>
        auto bind( TCallable&& callable ) noexcept -> event::delegate_token_type
        {
            event::delegate_token_type const token = token_gen_.generate( );
            token_delegates_.insert( { token, delegate_type{ std::forward<TCallable>( callable ) } } );
            return token;
        }


        /**
         * @brief Creates a scoped binding that automatically unbinds when destroyed.
         * @complexity O(1) average, O(log n) worst case (hash table insertion)
         * @tparam TCallable The callable type
         * @param callable The callable object to bind
         * @return RAII scoped_binding object for automatic cleanup
         * @note Preferred for temporary bindings and exception safety
         */
        template <typename TCallable> requires std::is_invocable_v<TCallable, TParams...>
        [[nodiscard]] auto bind_scoped( TCallable&& callable ) noexcept -> scoped_binding_type
        {
            return scoped_binding_type{ this->bind( callable ), *this };
        }


        /**
         * @brief Unbinds all instances of the given raw function pointer.
         * @complexity O(n) where n is the number of bound delegates
         * @param raw_fn_ptr Function pointer to unbind
         * @return Number of delegates that were unbound
         */
        auto unbind( raw_delegate_sig_type raw_fn_ptr ) noexcept -> size_t
        {
            if ( not raw_fn_ptr ) { return 0U; }

            size_t const erased_count = std::erase_if(
                delegate_traces_, [raw_fn_addr = reinterpret_cast<uintptr_t>( raw_fn_ptr )]( auto const& trace )
                {
                    return trace.call_type == event::internal::callable_type::raw_function &&
                           trace.underlying_addr == raw_fn_addr;
                } );
            return erased_count;
        }


        /**
         * @brief Unbinds a specific member function from a specific owner.
         * @complexity O(n) where n is the number of bound delegates
         * @tparam TClass The class type that owns the method
         * @tparam TMethod The member function type
         * @param owner Pointer to the object instance
         * @param method Member function pointer to unbind
         * @return Number of delegates that were unbound
         */
        template <typename TClass, typename TMethod> requires meta::member_of_class<TClass, TMethod>
        auto unbind( TClass* owner, TMethod method ) noexcept -> size_t
        {
            size_t const erased_count = std::erase_if(
                delegate_traces_, [owner, method_addr = meta::method_address_of( method )]( auto const& trace )
                {
                    return trace.call_type == event::internal::callable_type::member_function &&
                           trace.underlying_addr == method_addr && trace.owner == owner;
                } );
            return erased_count;
        }


        /**
         * @brief Unbinds all member functions belonging to the given owner.
         * @complexity O(n) where n is the number of bound delegates
         * @tparam TClass The owner class type
         * @param owner Pointer to the object whose methods should be unbound
         * @return Number of delegates that were unbound
         * @note Useful for cleanup when an object is being destroyed
         */
        template <typename TClass>
        auto unbind( TClass* owner ) noexcept -> size_t
        {
            if ( not owner ) { return 0U; }
            size_t const erased_count =
                    std::erase_if( delegate_traces_, [owner]( auto const& trace ) { return trace.owner == owner; } );
            return erased_count;
        }


        /**
         * @brief Unbinds a token-based delegate.
         * @complexity O(1) average, O(log n) worst case (hash table removal)
         * @param token The token returned from bind() for a callable
         * @return True if a delegate was found and removed, false otherwise
         */
        auto unbind_token( event::delegate_token_type token ) noexcept -> bool
        {
            if ( token == event::invalid_token ) { return false; }
            return token_delegates_.erase( token ) > 0U;
        }


        /**
         * @brief Invokes all bound delegates with the given arguments.
         * @complexity O(n) where n is the number of bound delegates
         * @param args Arguments to forward to each bound delegate
         * @note Calls direct bindings first, then token-based bindings
         */
        auto broadcast( TParams&&... args ) const -> void
        {
            for ( auto const& trace : delegate_traces_ )
            {
                trace.delegate( std::forward<TParams>( args )... );
            }
            for ( auto const& [_, delegate] : token_delegates_ )
            {
                delegate( std::forward<TParams>( args )... );
            }
        }


        /**
         * @brief Checks if the delegate has no bound callbacks.
         * @complexity O(1)
         * @return True if no delegates are bound, false otherwise
         */
        auto empty( ) const noexcept -> bool
        {
            return delegate_traces_.empty( ) && token_delegates_.empty( );
        }


        /**
         * @brief Returns the total number of bound delegates.
         * @complexity O(1)
         * @return Sum of direct bindings and token-based bindings
         */
        auto size( ) const noexcept -> size_t
        {
            return delegate_traces_.size( ) + token_delegates_.size( );
        }

    private:
        std::vector<delegate_trace_type> delegate_traces_{};

        token_generator<event::delegate_token_type, event::invalid_token> token_gen_{};
        std::unordered_map<event::delegate_token_type, delegate_type> token_delegates_{};
    };
}


#endif //!RST_MULTICAST_DELEGATE_H
