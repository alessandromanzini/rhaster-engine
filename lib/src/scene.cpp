#include <rst/__core/scene.h>

#include <rst/__core/gameobject.h>


namespace rst
{
    Scene::Scene( std::string name )
        : name_{ std::move( name ) }
        , id_( s_id_counter_++ ) { }


    auto Scene::add( std::unique_ptr<GameObject> object ) -> void
    {
        assert( object != nullptr && "Object pointer cannot be null!" );
        objects_.emplace_back( std::move( object ) );
    }


    auto Scene::remove( GameObject& object ) -> void
    {
        deleter_.mark_element_for_deletion( &object );

        // remove children as well
        for ( auto* child : object.get_children( ) )
        {
            remove( *child );
        }
    }


    auto Scene::remove_all( ) -> void
    {
        for ( auto& object : objects_ )
        {
            deleter_.mark_element_for_deletion( object.get( ) );
        }
    }


    auto Scene::find_object( std::function<bool( GameObject const& )> const& predicate ) const -> OptionalRef<GameObject>
    {
        for ( auto const& object : objects_ )
        {
            if ( predicate( *object ) )
            {
                return *object;
            }
        }
        return nullptr;
    }


    auto Scene::create_object( ) -> GameObject&
    {
        return *objects_.emplace_back( std::make_unique<GameObject>( *this ) );
    }


    auto Scene::get_name( ) const -> std::string const&
    {
        return name_;
    }


    auto Scene::get_id( ) const -> uint16_t
    {
        return id_;
    }


    auto Scene::fixed_tick( ) const -> void
    {
        for ( auto const& object : objects_ )
        {
            object->fixed_tick( );
        }
    }


    auto Scene::tick( ) const -> void
    {
        for ( auto const& object : objects_ )
        {
            object->tick( );
        }
    }


    auto Scene::render( ) const -> void
    {
        for ( auto const& object : objects_ )
        {
            object->render( );
        }
    }


    auto Scene::cleanup( ) -> void
    {
        // delete marked objects
        if ( deleter_.is_cleanup_needed( ) )
        {
            deleter_.cleanup( objects_ );
        }

        // cleanup objects
        for ( auto const& object : objects_ )
        {
            object->cleanup( );
        }
    }


    uint16_t Scene::s_id_counter_ = 0;
}
