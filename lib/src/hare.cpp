#include <rst/__core/hare.h>

#include <rst/temp/singleton/game_instance.h>
#include <rst/temp/singleton/game_time.h>
#include <rst/temp/singleton/input_system.h>
#include <rst/temp/singleton/renderer.h>
#include <rst/temp/singleton/resource_manager.h>
#include <rst/temp/singleton/scene_pool.h>

#include <SDL.h>


namespace rst
{
    SDL_Window* g_window_ptr{};


    Hare::Hare( std::string const& window_title, std::filesystem::path const& data_path, glm::vec2 const viewport )
        : viewport_{ viewport }
    {
        if ( SDL_Init( SDL_INIT_VIDEO | SDL_INIT_GAMECONTROLLER ) != 0 )
        {
            throw std::runtime_error( std::string( "SDL_Init Error: " ) + SDL_GetError( ) );
        }

        g_window_ptr = SDL_CreateWindow(
            window_title.c_str( ), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, viewport_.x, viewport_.y, SDL_WINDOW_OPENGL );
        if ( g_window_ptr == nullptr )
        {
            throw std::runtime_error( std::string( "SDL_CreateWindow Error: " ) + SDL_GetError( ) );
        }

        // initialize singletons
        RENDERER.init( g_window_ptr );
        RESOURCE_MANAGER.init( data_path );
    }


    Hare::~Hare( ) noexcept
    {
        // destroy singletons
        RENDERER.destroy( );

        SDL_DestroyWindow( g_window_ptr );
        g_window_ptr = nullptr;
        SDL_Quit( );
    }


    auto Hare::run( ) -> void
    {
        GAME_TIME.reset( );
        GAME_INSTANCE.set_screen_dimensions( viewport_ );
        while ( !request_quit_ )
        {
            run_one_frame( );
        }
        SCENE_POOL.unload_all_scenes( );
        GAME_INSTANCE.destroy( );
    }


    auto Hare::run_one_frame( ) -> void
    {
        // +--------------------------------+
        // | TIME CALCULATIONS ( ticking )  |
        // +--------------------------------+
        GAME_TIME.tick( );

        // +--------------------------------+
        // | GAME LOOP                      |
        // +--------------------------------+
        request_quit_ = !INPUT_SYSTEM.process_input( );

        // +--------------------------------+
        // | FIXED TICK                     |
        // +--------------------------------+
        while ( GAME_TIME.is_fixed_tick_required( ) )
        {
            GAME_TIME.set_timing_type( time::TimingType::FIXED_DELTA_TIME );
            SCENE_POOL.fixed_tick( );
            GAME_TIME.fixed_tick( );
        }
        // todo add service locator as tick parameter

        // +--------------------------------+
        // | TICK                           |
        // +--------------------------------+
        GAME_TIME.set_timing_type( time::TimingType::DELTA_TIME );
        SCENE_POOL.tick( );

        // +--------------------------------+
        // | UPDATE SERVICES                |
        // +--------------------------------+
        RENDERER.render( );

        // +--------------------------------+
        // | DESTROYED OBJECTS DELETION     |
        // +--------------------------------+
        SCENE_POOL.cleanup( );
        RESOURCE_MANAGER.unload_unused_resources( );

        // +--------------------------------+
        // | SLEEPING                       |
        // +--------------------------------+
        std::this_thread::sleep_for( GAME_TIME.get_sleep_time( ) );
    }
}
