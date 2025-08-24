#include <framework/GameObjectView.h>

// +--------------------------------+
// | STANDARD HEADERS				|
// +--------------------------------+


namespace engine
{
    const Transform& GameObjectView::get_world_transform( ) const
    {
        return gameobject_ptr_->get_world_transform( );
    }


    const Transform& GameObjectView::get_local_transform( ) const
    {
        return gameobject_ptr_->get_local_transform( );
    }


    // ReSharper disable once CppMemberFunctionMayBeConst
    void GameObjectView::set_world_transform( const Transform& transform )
    {
        gameobject_ptr_->set_world_transform( transform );
    }


    // ReSharper disable once CppMemberFunctionMayBeConst
    void GameObjectView::set_local_transform( const Transform& transform )
    {
        gameobject_ptr_->set_local_transform( transform );
    }


    // ReSharper disable once CppMemberFunctionMayBeConst
    void GameObjectView::set_local_transform( Transform&& transform )
    {
        gameobject_ptr_->set_local_transform( transform );
    }


    // ReSharper disable once CppMemberFunctionMayBeConst
    void GameObjectView::remove_component( Component& component )
    {
        gameobject_ptr_->remove_component( component );
    }


    // PRIVATE CONSTRUCTOR
    GameObjectView::GameObjectView( GameObject& gameobject )
        : gameobject_ptr_{ &gameobject } { }
}
