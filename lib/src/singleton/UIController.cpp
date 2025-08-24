// ReSharper disable CppMemberFunctionMayBeStatic
#include <singleton/UIController.h>

// +--------------------------------+
// | PROJECT HEADERS				|
// +--------------------------------+
#include <framework/ui_component/UIComponent.h>
#include <singleton/Renderer.h>

// +--------------------------------+
// | IMGUI HEADERS					|
// +--------------------------------+
#include <imgui.h>
#include <core/imgui_core.h>


namespace engine
{
    UIController& UI_CONTROLLER = UIController::get_instance( );


    void UIController::init( )
    {
        IMGUI_CHECKVERSION( );
        ImGui::CreateContext( );
        imgui_core::init( RENDERER.window_ptr_, RENDERER.get_sdl_renderer( ) );
    }


    void UIController::destroy( )
    {
        imgui_core::destroy( );
        ImGui::DestroyContext( );
    }


    void UIController::process_input( const SDL_Event& event )
    {
        imgui_core::process_input( event );
    }


    void UIController::tick( )
    {
        for ( const auto& [key, uiComponent] : ui_components_ )
        {
            uiComponent->tick( );
        }
    }


    void UIController::render( )
    {
        start_imgui_render( );
        for ( const auto& [key, uiComponent] : ui_components_ )
        {
            uiComponent->render( );
        }
        end_imgui_render( );
    }


    void UIController::cleanup( )
    {
        if ( deleter_.is_cleanup_needed( ) )
        {
            deleter_.cleanup( ui_components_ );
        }
    }


    void UIController::remove_ui_component( UIComponent& uiComponent )
    {
        deleter_.mark_element_for_deletion( &uiComponent );
    }


    void UIController::start_imgui_render( ) const
    {
        imgui_core::new_frame( );
        ImGui::NewFrame( );
    }


    void UIController::end_imgui_render( ) const
    {
        ImGui::Render( );
        imgui_core::render( ImGui::GetDrawData( ), RENDERER.get_sdl_renderer( ) );
    }

}
