#include <rst/temp/singleton/scene_pool.h>

#include <rst/temp/framework/component/physics/collider_component.h>
#include <rst/__core/scene.h>


namespace rst
{
    auto ScenePool::create_scene( std::string const& name ) -> Scene&
    {
        // TODO: use UID
        auto const& scene = scenes_.emplace_back( std::make_unique<Scene>( name ) );
        return *scene;
    }


    auto ScenePool::select_scene( std::string const& name ) -> void
    {
        active_scene_ptr_ = &get_scene( name );
    }


    auto ScenePool::select_first_scene( ) -> void
    {
        if ( scenes_.empty( ) ) { return; }
        active_scene_ptr_ = scenes_[0].get( );
    }


    auto ScenePool::unload_scene( std::string const& name ) -> void
    {
        std::erase_if( scenes_, [&name]( auto& s ) { return s->get_name( ) == name; } );
    }


    auto ScenePool::unload_all_scenes( ) -> void
    {
        scenes_.clear( );
    }


    auto ScenePool::fixed_tick( ) const -> void
    {
        if ( not active_scene_ptr_ ) { return; }
        active_scene_ptr_->fixed_tick( );
    }


    auto ScenePool::tick( ) const -> void
    {
        if ( not active_scene_ptr_ ) { return; }
        active_scene_ptr_->tick( );
        ColliderComponent::late_tick( );
    }


    auto ScenePool::render( ) const -> void
    {
        if ( not active_scene_ptr_ ) { return; }
        active_scene_ptr_->render( );
    }


    auto ScenePool::does_scene_exist( std::string const& name ) const -> bool
    {
        return std::ranges::any_of( scenes_, [&]( auto& s ) { return s->get_name( ) == name; } );
    }


    auto ScenePool::get_active_scene( ) -> Scene&
    {
        return *active_scene_ptr_;
    }


    auto ScenePool::get_active_scene( ) const -> Scene&
    {
        // active scene is not set until game loop starts.
        // if you need to delete a gameobject, call the method on the scene directly.
        assert( active_scene_ptr_ != nullptr && "No active scene is set yet." );

        // ReSharper disable once CppDFANullDereference
        return *active_scene_ptr_;
    }


    auto ScenePool::get_scene( std::string const& name ) const -> Scene&
    {
        auto const scene =
                std::ranges::find_if( scenes_,
                                      [&name]( auto const& s ) { return s->get_name( ) == name; } );
        assert( scene != scenes_.end( ) && "Scene not found." );
        return **scene;
    }


    auto ScenePool::get_scene( uint16_t id ) const -> Scene&
    {
        auto const scene =
                std::ranges::find_if( scenes_,
                                      [&id]( auto const& s ) { return s->get_id( ) == id; } );
        assert( scene != scenes_.end( ) && "Scene not found." );
        return **scene;
    }


    auto ScenePool::cleanup( ) const -> void
    {
        if ( not active_scene_ptr_ ) { return; }
        active_scene_ptr_->cleanup( );
    }


    ScenePool& SCENE_POOL = ScenePool::get_instance( );

}
