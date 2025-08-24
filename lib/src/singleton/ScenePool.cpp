#include <singleton/ScenePool.h>

// +--------------------------------+
// | PROJECT HEADERS				|
// +--------------------------------+
#include <framework/Scene.h>
#include <framework/component/physics/ColliderComponent.h>

// +--------------------------------+
// | STANDARD HEADERS				|
// +--------------------------------+
#include <cassert>
#include <algorithm>


namespace engine
{
    ScenePool& SCENE_POOL = ScenePool::get_instance( );


    void ScenePool::fixed_tick( ) const
    {
        if ( not active_scene_ptr_ ) { return; }
        active_scene_ptr_->fixed_tick( );
    }


    void ScenePool::tick( ) const
    {
        if ( not active_scene_ptr_ ) { return; }
        active_scene_ptr_->tick( );
        ColliderComponent::late_tick( );
    }


    void ScenePool::render( ) const
    {
        if ( not active_scene_ptr_ ) { return; }
        active_scene_ptr_->render( );
    }


    bool ScenePool::does_scene_exist( const std::string& name ) const
    {
        return std::ranges::any_of( scenes_, [&]( auto& s ) { return s->get_name( ) == name; } );
    }


    Scene& ScenePool::get_active_scene( )
    {
        return *active_scene_ptr_;
    }


    void ScenePool::cleanup( ) const
    {
        if ( not active_scene_ptr_ ) { return; }
        active_scene_ptr_->cleanup( );
    }


    Scene& ScenePool::create_scene( const std::string& name )
    {
        // TODO: use UID
        const auto& scene = scenes_.emplace_back( std::make_unique<Scene>( name ) );
        return *scene;
    }


    void ScenePool::select_scene( const std::string& name )
    {
        active_scene_ptr_ = &get_scene( name );
    }


    void ScenePool::select_first_scene( )
    {
        if ( scenes_.empty( ) ) { return; }
        active_scene_ptr_ = scenes_[0].get( );
    }


    void ScenePool::unload_scene( const std::string& name )
    {
        std::erase_if( scenes_, [&name]( auto& s ) { return s->get_name( ) == name; } );
    }


    void ScenePool::unload_all_scenes( )
    {
        scenes_.clear( );
    }


    Scene& ScenePool::get_active_scene( ) const
    {
        // Active scene is not set until game loop starts.
        // If you need to delete a gameobject, call the method on the scene directly.
        assert( active_scene_ptr_ != nullptr && "No active scene is set yet." );

        // ReSharper disable once CppDFANullDereference
        return *active_scene_ptr_;
    }


    Scene& ScenePool::get_scene( const std::string& name ) const
    {
        const auto scene =
                std::ranges::find_if( scenes_,
                                      [&name]( const auto& s ) { return s->get_name( ) == name; } );
        assert( scene != scenes_.end( ) && "Scene not found." );
        return **scene;
    }


    Scene& ScenePool::get_scene( uint16_t id ) const
    {
        const auto scene =
                std::ranges::find_if( scenes_,
                                      [&id]( const auto& s ) { return s->get_id( ) == id; } );
        assert( scene != scenes_.end( ) && "Scene not found." );
        return **scene;
    }

}
