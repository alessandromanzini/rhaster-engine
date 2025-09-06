#ifndef RST_MULTICAST_DELEGATE_H
#define RST_MULTICAST_DELEGATE_H

#include <rst/pch.h>

#include <rst/data_type/token_generator.h>
#include <rst/meta/function_traits.h>
#include <rst/__event/scoped_binding.h>
#include <rst/__event/token.h>


namespace rst
{
    // +--------------------------------+
    // | DELEGATE INFO STRUCT           |
    // +--------------------------------+
    namespace event
    {
        template <typename... TParams> struct delegate_info
        {
            using type = std::function<void( TParams... )>;

            delegate_token_type token{ invalid_token };
            void* binder{ nullptr };
            type delegate{};
        };
    }


    // +---------------------------+
    // | DELEGATE TYPE             |
    // +---------------------------+
    template <typename... TParams>
    class multicast_delegate final
    {
    public:
        using delegate_info_type = event::delegate_info<TParams...>;
        using delegate_type      = delegate_info_type::type;

        using scoped_binding_type = event::scoped_binding<TParams...>;

        using raw_delegate_sig_type = meta::function_traits<void( TParams... )>::raw_fn_t;


        explicit multicast_delegate( ) = default;
        ~multicast_delegate( )         = default;

        multicast_delegate( multicast_delegate const& )                        = delete;
        multicast_delegate( multicast_delegate&& ) noexcept                    = delete;
        auto operator=( multicast_delegate const& ) -> multicast_delegate&     = delete;
        auto operator=( multicast_delegate&& ) noexcept -> multicast_delegate& = delete;


        auto bind( raw_delegate_sig_type raw_fn_ptr ) -> void
        {
            delegates_.emplace_back( event::invalid_token, nullptr, delegate_type{ raw_fn_ptr } );
        }


        template <typename TClass, typename TMethod> requires std::is_member_function_pointer_v<TMethod>
        auto bind( TClass* binder, TMethod method ) -> void
        {
            assert( binder && "multicast_delegate::bind: binder cannot be nullptr!" );
            delegates_.emplace_back(
                event::invalid_token, binder, delegate_type{
                    [=]( TParams&&... args ) { std::invoke( method, binder, std::forward<TParams>( args )... ); }
                } );
        }


        template <typename TCallable> requires std::is_invocable_v<TCallable, TParams...>
        auto bind_scoped( TCallable&& callable ) -> scoped_binding_type
        {
            event::delegate_token_type const token = token_gen_.generate( );
            delegates_.emplace_back( token, nullptr, delegate_type{ std::forward<TCallable>( callable ) } );
            return scoped_binding_type{ token, *this };
        }


        auto unbind( raw_delegate_sig_type raw_fn_ptr ) -> size_t
        {
            assert( raw_fn_ptr && "multicast_delegate::unbind: raw_fn_ptr cannot be nullptr!" );
            size_t const erased_count = std::erase_if(
                delegates_, [raw_fn_ptr]( auto const& info )
                {
                    if ( info.token != event::invalid_token ) { return false; }
                    auto* const target_ptr = info.delegate.template target<raw_delegate_sig_type>( );
                    return target_ptr && *target_ptr == raw_fn_ptr;
                } );
            return erased_count;
        }


        template <typename TClass>
        auto unbind( TClass* binder ) -> size_t
        {
            size_t const erased_count =
                    std::erase_if( delegates_, [binder]( auto const& info ) { return info.binder == binder; } );
            return erased_count;
        }


        auto unbind_token( event::delegate_token_type token ) -> bool
        {
            if ( token == event::invalid_token ) { return false; }

            auto it = std::ranges::find_if( delegates_, [token]( auto const& info ) { return info.token == token; } );
            if ( it != delegates_.end( ) )
            {
                delegates_.erase( it );
                return true;
            }
            return false;
        }


        auto broadcast( TParams&&... args ) const -> void
        {
            for ( auto const& info : delegates_ )
            {
                info.delegate( std::forward<TParams>( args )... );
            }
        }


        auto empty( ) const -> bool
        {
            return delegates_.empty( );
        }


        auto size( ) const -> size_t
        {
            return delegates_.size( );
        }

    private:
        std::vector<delegate_info_type> delegates_{};
        token_generator<event::delegate_token_type, event::invalid_token> token_gen_{};
    };
}


#endif //!RST_MULTICAST_DELEGATE_H
