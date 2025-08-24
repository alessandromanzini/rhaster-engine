#include <framework/component/Component.h>

#include <framework/GameObject.h>


namespace engine
{
    Component::~Component( )
    {
        get_owner( ).on_deletion.unbind( this );
    }


    Component::owner_t& Component::get_owner( ) const
    {
        return owner_ref_;
    }


    void Component::mark_for_deletion( )
    {
        get_owner( ).remove_component( *this );
    }


    Component::Component( owner_t& owner )
        : owner_ref_{ owner }
    {
        get_owner( ).on_deletion.bind( this, &Component::begin_owner_deletion );
    }

}
