#include <core/imgui_core.h>

#include <backends/imgui_impl_sdl2.h>
#include <backends/imgui_impl_sdlrenderer2.h>


namespace imgui_core
{
    void init( SDL_Window* const window, SDL_Renderer* const renderer )
    {
        ImGui_ImplSDL2_InitForMetal( window );
        ImGui_ImplSDLRenderer2_Init( renderer );
    }


    void destroy( )
    {
        ImGui_ImplSDLRenderer2_Shutdown( );
        ImGui_ImplSDL2_Shutdown( );
    }


    void process_input( const SDL_Event& event )
    {
        ImGui_ImplSDL2_ProcessEvent( &event );
    }


    void new_frame( )
    {
        ImGui_ImplSDLRenderer2_NewFrame( );
        ImGui_ImplSDL2_NewFrame( );
    }


    void render( ImDrawData* const data, SDL_Renderer* const renderer )
    {
        ImGui_ImplSDLRenderer2_RenderDrawData( data, renderer );
    }

}
