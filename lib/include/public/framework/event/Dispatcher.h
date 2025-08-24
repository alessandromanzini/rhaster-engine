#ifndef DISPATCHER_H
#define DISPATCHER_H

#include <functional>
#include <vector>


namespace engine
{
    namespace event
    {
        template <typename... delegate_params_t>
        struct DelegateInfo final
        {
            using delegate_t = std::function<void( delegate_params_t... )>;
            void* binder{ nullptr };
            delegate_t delegate{};
        };
    }

    template <typename... delegate_params_t>
    class Dispatcher final
    {
    public:
        using delegate_info_t = event::DelegateInfo<delegate_params_t...>;

        void set_delegates_pool( std::vector<delegate_info_t>& delegates )
        {
            delegate_infos_ptr_ = &delegates;
        }


        void broadcast( delegate_params_t&&... args ) const
        {
            if ( not delegate_infos_ptr_ ) { return; }
            for ( const auto& info : *delegate_infos_ptr_ )
            {
                info.delegate( std::forward<delegate_params_t>( args )... );
            }
        }

    private:
        std::vector<delegate_info_t>* delegate_infos_ptr_{};

    };

}


#endif //!DISPATCHER_H
