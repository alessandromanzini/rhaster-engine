#ifndef RST_GAME_INSTANCE_H
#define RST_GAME_INSTANCE_H

#include <rst/temp/singleton/Singleton.h>

#include <rst/__controller/player_controller.h>


// TODO: remove colliders from here, move to main canvas
namespace rst
{
    class collider_component;
}


namespace rst
{
    class game_instance final : public singleton<game_instance>
    {
        friend class singleton;

    public:
        auto destroy( ) -> void;

        template <typename TController, typename... TArgs> requires
            std::derived_from<TController, base_controller> && std::constructible_from<TController, TArgs...>
        auto add_controller( TArgs&&... args ) -> TController&;
        auto remove_controller( player_controller const& controller ) -> void;
        auto clear_controllers( ) -> void;

        auto set_gravity_coefficient( float coefficient ) -> void;
        [[nodiscard]] auto gravity_coefficient( ) const -> float;

        auto set_screen_dimensions( glm::vec2 dimensions ) -> void;
        [[nodiscard]] auto screen_dimensions( ) const -> glm::vec2;

    private:
        std::vector<std::unique_ptr<player_controller>> controllers_{};
        std::vector<collider_component*> colliders_{};
        float gravity_coefficient_{ 10.f };
        glm::vec2 screen_dimensions_{ 0.f };
    };


    template <typename TController, typename... TArgs> requires std::derived_from<TController, base_controller> &&
                                                                std::constructible_from<TController, TArgs...>
    auto game_instance::add_controller( TArgs&&... args ) -> TController&
    {
        auto& controller = controllers_.emplace_back( std::make_unique<TController>( std::forward<TArgs>( args )... ) );
        return static_cast<TController&>( *controller );
    }


    extern game_instance& GAME_INSTANCE;
}


#endif //!RST_GAME_INSTANCE_H
