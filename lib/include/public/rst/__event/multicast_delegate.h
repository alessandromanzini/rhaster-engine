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
    class MulticastDelegate final
    {
    public:
        using DispatcherType = Dispatcher<TParams...>;

        using RawDelegate = meta::function_traits<void( TParams... )>::raw_fn_t;
        using Delegate    = meta::function_traits<void( TParams... )>::std_fn_t;


        explicit MulticastDelegate( DispatcherType& dispatcher )
            : dispatcher_ref_{ dispatcher }
        {
            dispatcher_ref_.set_delegates_pool( delegates_ );
        }


        ~MulticastDelegate( ) = default;

        MulticastDelegate( MulticastDelegate const& )                        = delete;
        MulticastDelegate( MulticastDelegate&& ) noexcept                    = delete;
        auto operator=( MulticastDelegate const& ) -> MulticastDelegate&     = delete;
        auto operator=( MulticastDelegate&& ) noexcept -> MulticastDelegate& = delete;


        auto bind( RawDelegate delegate ) -> void
        {
            delegates_.emplace_back( nullptr, Delegate{ delegate } );
        }


        template <typename TClass, typename TMethod> requires std::is_same_v<Delegate, typename meta::function_traits<
            TMethod>::std_fn_t>
        auto bind( TClass* binder, TMethod delegate ) -> void
        {
            assert( binder && "MulticastDelegate::bind: Binder cannot be nullptr!" );
            delegates_.emplace_back(
                binder, Delegate{
                    [=]( TParams&&... args ) { std::invoke( delegate, binder, std::forward<TParams>( args )... ); }
                } );
        }


        auto unbind( RawDelegate delegate ) -> void
        {
            std::erase_if(
                delegates_, [delegate]( typename DispatcherType::Info& info )
                {
                    using RawFunctionType = auto( ) -> void;
                    auto** address1       = info.delegate.template target<RawFunctionType*>( );
                    auto** address2       = delegate.template target<RawFunctionType*>( );
                    return reinterpret_cast<size_t>( *address1 ) == reinterpret_cast<size_t>( *address2 );
                } );
        }


        template <typename class_t>
        auto unbind( class_t* binder ) -> void
        {
            std::erase_if( delegates_, [binder]( typename DispatcherType::Info& info ) { return info.binder == binder; } );
        }


        auto empty( ) const -> bool
        {
            return delegates_.empty( );
        }

    private :
        DispatcherType& dispatcher_ref_;
        std::vector<typename DispatcherType::Info> delegates_{};
    };
}


#endif //!RST_MULTICAST_DELEGATE_H
