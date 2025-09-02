#ifndef RST_DISPATCHER_H
#define RST_DISPATCHER_H

#include <rst/pch.h>


// TODO: consider removing dispatcher
namespace rst
{
    namespace event
    {
        template <typename... TParams>
        struct DelegateInfo final
        {
            using delegate_t = std::function<void( TParams... )>;
            void* binder{ nullptr };
            delegate_t delegate{};
        };
    }

    template <typename... TParams>
    class Dispatcher final
    {
    public:
        using Info = event::DelegateInfo<TParams...>;


        auto set_delegates_pool( std::span<Info> delegates ) -> void
        {
            delegates_view_ = std::move( delegates );
        }


        auto broadcast( TParams&&... args ) const -> void
        {
            for ( auto const& info : delegates_view_ )
            {
                info.delegate( std::forward<TParams>( args )... );
            }
        }

    private:
        std::span<Info> delegates_view_{};
    };
}


#endif //!RST_DISPATCHER_H
