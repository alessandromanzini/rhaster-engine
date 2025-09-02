#ifndef RST_SCENE_POOL_H
#define RST_SCENE_POOL_H

#include <rst/pch.h>

#include <rst/temp/singleton/Singleton.h>
#include <rst/__core/scene.h>


namespace rst
{
    // todo: make tick and render inaccessible from outside
    class ScenePool final : public Singleton<ScenePool>
    {
        friend class Singleton;

    public:
        auto create_scene( std::string const& name ) -> Scene&;

        auto select_scene( std::string const& name ) -> void;
        auto select_first_scene( ) -> void;

        auto unload_scene( std::string const& name ) -> void;
        auto unload_all_scenes( ) -> void;

        auto fixed_tick( ) const -> void;
        auto tick( ) const -> void;
        auto render( ) const -> void;

        [[nodiscard]] auto does_scene_exist( std::string const& name ) const -> bool;

        [[nodiscard]] auto get_active_scene( ) -> Scene&;
        [[nodiscard]] auto get_active_scene( ) const -> Scene&;
        [[nodiscard]] auto get_scene( std::string const& name ) const -> Scene&;
        [[nodiscard]] auto get_scene( uint16_t id ) const -> Scene&;

        auto cleanup( ) const -> void;

    private:
        std::vector<std::unique_ptr<Scene>> scenes_;
        Scene* active_scene_ptr_{ nullptr };

        ScenePool( ) = default;
    };

    extern ScenePool& SCENE_POOL;
}


#endif //!RST_SCENE_POOL_H
