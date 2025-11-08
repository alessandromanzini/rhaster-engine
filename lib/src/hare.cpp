#include <rst/__core/hare.h>

#include <rst/core.h>
#include <rst/diagnostic.h>

#include <rst/temp/singleton/game_instance.h>
#include <rst/temp/singleton/game_time.h>
#include <rst/temp/singleton/input_system.h>
#include <rst/temp/singleton/resource_manager.h>
#include <rst/temp/singleton/scene_pool.h>

#include <SDL.h>


namespace rst
{
    hare::hare( std::string const& window_title, std::filesystem::path const& /* data_path */, glm::vec2 const viewport )
        : viewport_{ viewport }
    {
        if ( SDL_Init( SDL_INIT_VIDEO | SDL_INIT_GAMECONTROLLER ) != 0 )
        {
            startle( "SDL_Init error: {}", SDL_GetError( ) );
        }

        // initialize singletons
        // RENDERER.init( g_window_ptr );
        // RESOURCE_MANAGER.init( data_path );
        service_locator_.register_renderer_service<service::sdl_renderer_service>( window_title, viewport_ );
        scheduler_.register_system<system::renderer_system>( system_timing::render );
    }


    hare::~hare( ) noexcept
    {
        // destroy singletons
        // RENDERER.destroy( );
        SDL_Quit( );
    }


    auto hare::registry( ) noexcept -> ecs::registry&
    {
        return registry_;
    }


    auto hare::service_locator( ) noexcept -> rst::service_locator&
    {
        return service_locator_;
    }


    auto hare::scheduler( ) noexcept -> system_scheduler<system_timing>&
    {
        return scheduler_;
    }


    auto hare::run( ) noexcept -> detail::hop_result try
    {
        GAME_TIME.reset( );
        GAME_INSTANCE.set_screen_dimensions( viewport_ );
        while ( !request_quit_ )
        {
            run_one_frame( );
        }
        // SCENE_POOL.unload_all_scenes( );
        GAME_INSTANCE.destroy( );
        return detail::hop_result::success;
    }
    catch ( std::exception const& e )
    {
        alert( "fatal error: {}", e.what( ) );
        return detail::hop_result::pitfall;
    }
    catch ( ... )
    {
        alert( "fatal error: unknown" );
        return detail::hop_result::unknown;
    }


    auto hare::run_one_frame( ) -> void
    {
        // +--------------------------------+
        // | TIME CALCULATIONS ( ticking )  |
        // +--------------------------------+
        GAME_TIME.tick( );

        // +--------------------------------+
        // | EARLY TICK & INPUT             |
        // +--------------------------------+
        scheduler_.signal_hook( system_timing::early_tick );
        request_quit_ = !INPUT_SYSTEM.process_input( );

        // +--------------------------------+
        // | FIXED TICK                     |
        // +--------------------------------+
        while ( GAME_TIME.is_fixed_tick_required( ) )
        {
            GAME_TIME.set_timing_type( time::timing_type::fixed );
            scheduler_.signal_hook( system_timing::fixed_tick );
            //SCENE_POOL.fixed_tick( );

            scheduler_.signal_hook( system_timing::pre_physics );
            scheduler_.signal_hook( system_timing::physics );
            scheduler_.signal_hook( system_timing::post_physics );

            GAME_TIME.fixed_tick( );
        }
        // todo add service locator as tick parameter

        // +--------------------------------+
        // | TICK                           |
        // +--------------------------------+
        GAME_TIME.set_timing_type( time::timing_type::variable );
        scheduler_.signal_hook( system_timing::tick );
        scheduler_.signal_hook( system_timing::late_tick );
        // SCENE_POOL.tick( );

        // +--------------------------------+
        // | RENDER                         |
        // +--------------------------------+
        scheduler_.signal_hook( system_timing::pre_render );
        scheduler_.signal_hook( system_timing::render );
        scheduler_.signal_hook( system_timing::post_render );

        // +--------------------------------+
        // | DESTROYED OBJECTS DELETION     |
        // +--------------------------------+
        // SCENE_POOL.cleanup( );
        RESOURCE_MANAGER.unload_unused_resources( );

        // +--------------------------------+
        // | SLEEPING                       |
        // +--------------------------------+
        std::this_thread::sleep_for( GAME_TIME.sleep_time( ) );
    }
}
