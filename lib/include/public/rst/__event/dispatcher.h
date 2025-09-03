#ifndef RST_DISPATCHER_H
#define RST_DISPATCHER_H

#include <rst/pch.h>


// TODO: consider removing dispatcher
namespace rst
{
    namespace event
    {
        template <typename... TParams>
        struct delegate_info final
        {
            using type = std::function<void( TParams... )>;
            void* binder{ nullptr };
            type delegate{};
        };
    }

    template <typename... TParams>
    class dispatcher final
    {
    public:
        using info_type = event::delegate_info<TParams...>;


        auto set_delegates_pool( std::span<info_type> delegates ) -> void
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
        std::span<info_type> delegates_view_{};
    };
}


#endif //!RST_DISPATCHER_H
