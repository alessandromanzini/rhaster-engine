#include <rst/temp/singleton/scene_pool.h>

#include <rst/temp/framework/component/physics/collider_component.h>
#include <rst/__core/scene.h>


namespace rst
{
    auto scene_pool::create_scene( std::string const& name ) -> rst::scene&
    {
        // TODO: use UID
        auto const& scene = scenes_.emplace_back( std::make_unique<rst::scene>( name ) );
        return *scene;
    }


    auto scene_pool::select_scene( std::string const& name ) -> void
    {
        active_scene_ptr_ = &scene( name );
    }


    auto scene_pool::select_first_scene( ) -> void
    {
        if ( scenes_.empty( ) ) { return; }
        active_scene_ptr_ = scenes_[0].get( );
    }


    auto scene_pool::unload_scene( std::string const& name ) -> void
    {
        std::erase_if( scenes_, [&name]( auto& s ) { return s->name( ) == name; } );
    }


    auto scene_pool::unload_all_scenes( ) -> void
    {
        scenes_.clear( );
    }


    auto scene_pool::fixed_tick( ) const -> void
    {
        if ( not active_scene_ptr_ ) { return; }
        active_scene_ptr_->fixed_tick( );
    }


    auto scene_pool::tick( ) const -> void
    {
        if ( not active_scene_ptr_ ) { return; }
        active_scene_ptr_->tick( );
        // collider_component::late_tick( );
    }


    auto scene_pool::render( ) const -> void
    {
        if ( not active_scene_ptr_ ) { return; }
        active_scene_ptr_->render( );
    }


    auto scene_pool::does_scene_exist( std::string const& name ) const -> bool
    {
        return std::ranges::any_of( scenes_, [&]( auto& s ) { return s->name( ) == name; } );
    }


    auto scene_pool::active_scene( ) -> rst::scene&
    {
        return *active_scene_ptr_;
    }


    auto scene_pool::active_scene( ) const -> rst::scene&
    {
        // active scene is not set until game loop starts.
        // if you need to delete a gameobject, call the method on the scene directly.
        assert( active_scene_ptr_ != nullptr && "No active scene is set yet." );

        // ReSharper disable once CppDFANullDereference
        return *active_scene_ptr_;
    }


    auto scene_pool::scene( std::string const& name ) const -> rst::scene&
    {
        auto const scene = std::ranges::find_if( scenes_, [&name]( auto const& s ) { return s->name( ) == name; } );
        assert( scene != scenes_.end( ) && "Scene not found." );
        return **scene;
    }


    auto scene_pool::scene( uint16_t id ) const -> rst::scene&
    {
        auto const scene = std::ranges::find_if( scenes_, [&id]( auto const& s ) { return s->id( ) == id; } );
        assert( scene != scenes_.end( ) && "Scene not found." );
        return **scene;
    }


    auto scene_pool::cleanup( ) const -> void
    {
        if ( not active_scene_ptr_ ) { return; }
        active_scene_ptr_->cleanup( );
    }


    scene_pool& SCENE_POOL = scene_pool::instance( );
}
