#include <rst/temp/singleton/game_instance.h>

#include <rst/temp/framework/component/physics/collider_component.h>


namespace rst
{
    auto GameInstance::destroy( ) -> void
    {
        controllers_.clear( );
    }


    auto GameInstance::remove_controller( PlayerController const& controller ) -> void
    {
        std::erase_if( controllers_, [&]( auto const& controller_ptr )
        {
            return controller_ptr.get( ) == &controller;
        } );
    }


    auto GameInstance::clear_controllers( ) -> void
    {
        controllers_.clear( );
    }


    auto GameInstance::set_gravity_coefficient( float const coefficient ) -> void
    {
        gravity_coefficient_ = coefficient;
    }


    auto GameInstance::get_gravity_coefficient( ) const -> float
    {
        return gravity_coefficient_;
    }


    auto GameInstance::set_screen_dimensions( glm::vec2 const dimensions ) -> void
    {
        screen_dimensions_ = dimensions;
    }


    auto GameInstance::get_screen_dimensions( ) const -> glm::vec2
    {
        return screen_dimensions_;
    }


    GameInstance& GAME_INSTANCE = GameInstance::get_instance( );

}
