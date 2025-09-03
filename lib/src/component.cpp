#include <rst/__core/component.h>

#include <rst/__core/gameobject.h>


namespace rst
{
    component::~component( )
    {
        owner( ).on_deletion.unbind( this );
    }


    auto component::owner( ) const -> owner_type&
    {
        return owner_ref_;
    }


    auto component::mark_for_deletion( ) -> void
    {
        owner( ).remove_component( *this );
    }


    component::component( owner_type& owner )
        : owner_ref_{ owner }
    {
        owner.on_deletion.bind( this, &component::begin_owner_deletion );
    }

}
