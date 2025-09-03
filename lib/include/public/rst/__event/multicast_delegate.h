#ifndef RST_MULTICAST_DELEGATE_H
#define RST_MULTICAST_DELEGATE_H

#include <rst/pch.h>

#include <rst/meta/function_traits.h>
#include <rst/__event/dispatcher.h>


namespace rst
{
    // +---------------------------+
    // | GENERAL TYPE              |
    // +---------------------------+
    // TODO: support all std::function signatures
    // TODO: improve binding without binder pointer and easier lambda linking
    template <typename... TParams>
    class multicast_delegate final
    {
    public:
        using dispatcher_type = dispatcher<TParams...>;

        using raw_delegate_type = meta::function_traits<void( TParams... )>::raw_fn_t;
        using delegate_type    = meta::function_traits<void( TParams... )>::std_fn_t;


        explicit multicast_delegate( dispatcher_type& dispatcher )
            : dispatcher_ref_{ dispatcher }
        {
            dispatcher_ref_.set_delegates_pool( delegates_ );
        }


        ~multicast_delegate( ) = default;

        multicast_delegate( multicast_delegate const& )                        = delete;
        multicast_delegate( multicast_delegate&& ) noexcept                    = delete;
        auto operator=( multicast_delegate const& ) -> multicast_delegate&     = delete;
        auto operator=( multicast_delegate&& ) noexcept -> multicast_delegate& = delete;


        auto bind( raw_delegate_type delegate ) -> void
        {
            delegates_.emplace_back( nullptr, delegate_type{ delegate } );
        }


        template <typename TClass, typename TMethod> requires std::is_same_v<delegate_type, typename meta::function_traits<
            TMethod>::std_fn_t>
        auto bind( TClass* binder, TMethod delegate ) -> void
        {
            assert( binder && "MulticastDelegate::bind: Binder cannot be nullptr!" );
            delegates_.emplace_back(
                binder, delegate_type{
                    [=]( TParams&&... args ) { std::invoke( delegate, binder, std::forward<TParams>( args )... ); }
                } );
        }


        auto unbind( raw_delegate_type delegate ) -> void
        {
            std::erase_if(
                delegates_, [delegate]( typename dispatcher_type::Info& info )
                {
                    using raw_function_type = auto( ) -> void;
                    auto** address1       = info.delegate.template target<raw_function_type*>( );
                    auto** address2       = delegate.template target<raw_function_type*>( );
                    return reinterpret_cast<size_t>( *address1 ) == reinterpret_cast<size_t>( *address2 );
                } );
        }


        template <typename TCLass>
        auto unbind( TCLass* binder ) -> void
        {
            std::erase_if( delegates_, [binder]( typename dispatcher_type::info_type& info ) { return info.binder == binder; } );
        }


        auto empty( ) const -> bool
        {
            return delegates_.empty( );
        }

    private :
        dispatcher_type& dispatcher_ref_;
        std::vector<typename dispatcher_type::info_type> delegates_{};
    };
}


#endif //!RST_MULTICAST_DELEGATE_H
