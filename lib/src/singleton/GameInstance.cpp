#include <framework/component/physics/ColliderComponent.h>
#include <singleton/GameInstance.h>


namespace engine
{
    GameInstance& GAME_INSTANCE = GameInstance::get_instance( );


    void GameInstance::destroy( )
    {
        controllers_.clear( );
    }


    void GameInstance::remove_controller( const PlayerController& controller )
    {
        std::erase_if( controllers_, [&]( const auto& controllerPtr )
                           {
                               return controllerPtr.get( ) == &controller;
                           } );
    }


    void GameInstance::clear_controllers( )
    {
        controllers_.clear( );
    }


    void GameInstance::set_gravity_coefficient( const float coeffiecient )
    {
        gravity_coefficient_ = coeffiecient;
    }


    float GameInstance::get_gravity_coefficient( ) const
    {
        return gravity_coefficient_;
    }


    void GameInstance::set_screen_dimensions( const glm::vec2 dimensions )
    {
        screen_dimensions_ = dimensions;
    }


    glm::vec2 GameInstance::get_screen_dimensions( ) const
    {
        return screen_dimensions_;
    }

}
