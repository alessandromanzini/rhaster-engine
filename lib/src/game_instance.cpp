#include <rst/temp/singleton/game_instance.h>

#include <rst/temp/framework/component/physics/collider_component.h>


namespace rst
{
    auto game_instance::destroy( ) -> void
    {
        controllers_.clear( );
    }


    auto game_instance::remove_controller( player_controller const& controller ) -> void
    {
        std::erase_if( controllers_, [&]( auto const& controller_ptr )
        {
            return controller_ptr.get( ) == &controller;
        } );
    }


    auto game_instance::clear_controllers( ) -> void
    {
        controllers_.clear( );
    }


    auto game_instance::set_gravity_coefficient( float const coefficient ) -> void
    {
        gravity_coefficient_ = coefficient;
    }


    auto game_instance::gravity_coefficient( ) const -> float
    {
        return gravity_coefficient_;
    }


    auto game_instance::set_screen_dimensions( glm::vec2 const dimensions ) -> void
    {
        screen_dimensions_ = dimensions;
    }


    auto game_instance::screen_dimensions( ) const -> glm::vec2
    {
        return screen_dimensions_;
    }


    game_instance& GAME_INSTANCE = game_instance::instance( );

}
