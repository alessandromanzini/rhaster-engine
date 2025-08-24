#include <framework/ui_component/UIComponent.h>

#include <singleton/UIController.h>


namespace engine
{
    void UIComponent::mark_for_deletion( )
    {
        get_owner( ).remove_ui_component( *this );
    }


    UIComponent::UIComponent( UIController& owner )
        : owner_ref_{ owner } { }


    UIController& UIComponent::get_owner( ) const
    {
        return owner_ref_;
    }

}
