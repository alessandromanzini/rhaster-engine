#include <framework/ui_component/InputDisplayUIComponent.h>

// +--------------------------------+
// | PROJECT HEADERS				|
// +--------------------------------+
// #include <singletons/InputSystem.h>

// +--------------------------------+
// | IMGUI HEADERS					|
// +--------------------------------+
//#include <imgui.h>


namespace engine
{
    InputDisplayUIComponent::InputDisplayUIComponent( UIController& owner )
        : UIComponent( owner ) { }


    void InputDisplayUIComponent::render( )
    {
        //ImGui::Begin( "Input Display" );

        /*ImGui::Text( "Controller status:" );
        ImGui::SameLine( );
        if ( SDL_NumJoysticks( ) )
        {
            ImGui::TextColored( ImVec4{ .1f, .8f, .1f, 1.f }, "connected" );
        }
        else
        {
            ImGui::TextColored( ImVec4{ .75f, .75f, .1f, 1.f }, "awaiting..." );
        }

        ImGui::Separator( );

        ImGui::Text( "Thumb sticks" );
        ImGui::Spacing( );
        ImGui::Text( "LX: %.2f, %.2f", INPUT_SYSTEM.get_LX( ).x, INPUT_SYSTEM.get_LX( ).y );
        ImGui::Text( "RX: %.2f, %.2f", INPUT_SYSTEM.get_RX( ).x, INPUT_SYSTEM.get_RX( ).y );

        ImGui::Separator( );

        ImGui::Text( "Buttons" );
        ImGui::Spacing( );

        ImGui::Text( "A:" );
        ImGui::SameLine( );
        if ( INPUT_SYSTEM.is_button_pressed( BTN_MASK_CODE::BTN_A ) )
        {
            ImGui::TextColored( ImVec4{ .1f, .6f, .6f, 1.f }, "pressed" );
        }
        else
        {
            ImGui::TextColored( ImVec4{ .4f, .4f, .4f, 1.f }, "idle" );
        }

        ImGui::Text( "B:" );
        ImGui::SameLine( );
        if ( INPUT_SYSTEM.is_button_pressed( BTN_MASK_CODE::BTN_B ) )
        {
            ImGui::TextColored( ImVec4{ .1f, .6f, .6f, 1.f }, "pressed" );
        }
        else
        {
            ImGui::TextColored( ImVec4{ .4f, .4f, .4f, 1.f }, "idle" );
        }

        ImGui::Text( "A || B:" );
        ImGui::SameLine( );
        if ( INPUT_SYSTEM.is_button_pressed( BTN_MASK_CODE::BTN_A | BTN_MASK_CODE::BTN_B ) )
        {
            ImGui::TextColored( ImVec4{ .1f, .6f, .6f, 1.f }, "pressed" );
        }
        else
        {
            ImGui::TextColored( ImVec4{ .4f, .4f, .4f, 1.f }, "idle" );
        }

        ImGui::Text( "A && B:" );
        ImGui::SameLine( );
        if ( INPUT_SYSTEM.is_button_pressed( BTN_MASK_CODE::BTN_A | BTN_MASK_CODE::BTN_B, true ) )
        {
            ImGui::TextColored( ImVec4{ .1f, .6f, .6f, 1.f }, "pressed" );
        }
        else
        {
            ImGui::TextColored( ImVec4{ .4f, .4f, .4f, 1.f }, "idle" );
        }*/

        //ImGui::End( );
    }

}
