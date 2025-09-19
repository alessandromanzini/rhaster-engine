#include <rst/__core/__system/renderer_system.h>

#include <rst/__core/component/pelt_frame.h>
#include <rst/__core/component/transform.h>


namespace rst
{
    renderer_system::renderer_system( ) : base_system{ "renderer" } { }
    renderer_system::~renderer_system( ) noexcept = default;


    auto renderer_system::tick( ecs::registry& registry, service_locator const& locator ) noexcept -> void
    {
        auto& renderer = locator.renderer_service( );
        auto view      = registry.view<transform, pelt_frame const>( );

        // 1. queue renders
        for ( auto [_, transform, frame] : view.each( ) )
        {
            if ( frame.texture == nullptr ) { continue; }

            renderer.z_order( frame.z_index );
            renderer.render( *frame.texture, transform.world( ).location( ), frame.src_rect );
        }

        // 2. dispatch render
        // reinterpret cast is safe here because of the concept in service_locator when registering the service.
        reinterpret_cast<service::unified_render_dispatch&>( renderer ).render_dispatch( );
    }
}
