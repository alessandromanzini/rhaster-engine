#ifndef GAMEINSTANCE_H
#define GAMEINSTANCE_H

// +--------------------------------+
// | PROJECT HEADERS				|
// +--------------------------------+
#include <framework/controller/PlayerController.h>
#include <singleton/Singleton.h>


namespace engine
{
    class ColliderComponent;
    class GameInstance final : public Singleton<GameInstance>
    {
        friend class Singleton;

    public:
        void destroy( );

        template <typename controller_t, typename... controller_args_t>
            requires std::derived_from<controller_t, BaseController> && std::constructible_from<controller_t, controller_args_t...>
        controller_t& add_controller( controller_args_t&&... args );
        void remove_controller( const PlayerController& controller );
        void clear_controllers( );

        void set_gravity_coefficient( float coeffiecient );
        [[nodiscard]] float get_gravity_coefficient( ) const;

        void set_screen_dimensions( glm::vec2 dimensions );
        [[nodiscard]] glm::vec2 get_screen_dimensions( ) const;

    private:
        std::vector<std::unique_ptr<PlayerController>> controllers_{};
        std::vector<ColliderComponent*> colliders_{};
        float gravity_coefficient_{ 10.f };
        glm::vec2 screen_dimensions_{ 0.f };

    };


    template <typename controller_t, typename ... controller_args_t> requires std::derived_from<controller_t, BaseController> &&
        std::constructible_from<controller_t, controller_args_t...>
    controller_t& GameInstance::add_controller( controller_args_t&&... args )
    {
        auto& controller = controllers_.emplace_back( std::make_unique<controller_t>( std::forward<controller_args_t>( args )... ) );
        return static_cast<controller_t&>( *controller );
    }


    extern GameInstance& GAME_INSTANCE;

}


#endif //!GAMEINSTANCE_H
