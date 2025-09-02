#include <rst/__core/component.h>

#include <rst/__core/gameobject.h>


namespace rst
{
    Component::~Component( )
    {
        get_owner( ).on_deletion.unbind( this );
    }


    auto Component::get_owner( ) const -> owner_t&
    {
        return owner_ref_;
    }


    auto Component::mark_for_deletion( ) -> void
    {
        get_owner( ).remove_component( *this );
    }


    Component::Component( owner_t& owner )
        : owner_ref_{ owner }
    {
        get_owner( ).on_deletion.bind( this, &Component::begin_owner_deletion );
    }

}
