#ifndef RST_GAME_INSTANCE_H
#define RST_GAME_INSTANCE_H

#include <rst/temp/singleton/Singleton.h>

#include <rst/__controller/player_controller.h>


// TODO: remove colliders from here, move to main canvas
namespace rst
{
    class ColliderComponent;
    class GameInstance final : public Singleton<GameInstance>
    {
        friend class Singleton;

    public:
        auto destroy( ) -> void;

        template <typename TController, typename... TArgs>
            requires std::derived_from<TController, BaseController> && std::constructible_from<TController, TArgs...>
        auto add_controller( TArgs&&... args ) -> TController&;
        auto remove_controller( PlayerController const& controller ) -> void;
        auto clear_controllers( ) -> void;

        auto set_gravity_coefficient( float coefficient ) -> void;
        [[nodiscard]] auto get_gravity_coefficient( ) const -> float;

        auto set_screen_dimensions( glm::vec2 dimensions ) -> void;
        [[nodiscard]] auto get_screen_dimensions( ) const -> glm::vec2;

    private:
        std::vector<std::unique_ptr<PlayerController>> controllers_{};
        std::vector<ColliderComponent*> colliders_{};
        float gravity_coefficient_{ 10.f };
        glm::vec2 screen_dimensions_{ 0.f };

    };


    template <typename TController, typename ... TArgs> requires std::derived_from<TController, BaseController> &&
        std::constructible_from<TController, TArgs...>
    auto GameInstance::add_controller( TArgs&&... args ) -> TController&
    {
        auto& controller = controllers_.emplace_back( std::make_unique<TController>( std::forward<TArgs>( args )... ) );
        return static_cast<TController&>( *controller );
    }


    extern GameInstance& GAME_INSTANCE;

}


#endif //!RST_GAME_INSTANCE_H
