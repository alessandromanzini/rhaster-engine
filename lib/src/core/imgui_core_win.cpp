#include <core/imgui_core.h>

#include <backends/imgui_impl_opengl3.h>
#include <backends/imgui_impl_sdl2.h>


namespace imgui_core
{
    void init( SDL_Window* const window, SDL_Renderer* const /* renderer */ )
    {
        ImGui_ImplSDL2_InitForOpenGL( window, SDL_GL_GetCurrentContext( ) );
        ImGui_ImplOpenGL3_Init("#version 330");
    }


    void destroy( )
    {
        ImGui_ImplOpenGL3_Shutdown( );
        ImGui_ImplSDL2_Shutdown( );
    }


    void process_input( const SDL_Event& event )
    {
        ImGui_ImplSDL2_ProcessEvent( &event );
    }


    void new_frame( )
    {
        ImGui_ImplOpenGL3_NewFrame( );
        ImGui_ImplSDL2_NewFrame( );
     }


    void render( ImDrawData* const data, SDL_Renderer* const /* renderer */ )
    {
        ImGui_ImplOpenGL3_RenderDrawData( data );
    }

}
