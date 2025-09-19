#ifndef RST_SERVICE_UNIFIED_RENDER_DISPATCH_H
#define RST_SERVICE_UNIFIED_RENDER_DISPATCH_H


namespace rst::service
{
    class unified_render_dispatch
    {
    public:
        unified_render_dispatch( )                   = default;
        virtual ~unified_render_dispatch( ) noexcept = default;

        unified_render_dispatch( unified_render_dispatch const& )                        = delete;
        unified_render_dispatch( unified_render_dispatch&& ) noexcept                    = delete;
        auto operator=( unified_render_dispatch const& ) -> unified_render_dispatch&     = delete;
        auto operator=( unified_render_dispatch&& ) noexcept -> unified_render_dispatch& = delete;

        virtual auto render_dispatch( ) noexcept -> void = 0;
    };
}


#endif //!RST_SERVICE_UNIFIED_RENDER_DISPATCH_H
