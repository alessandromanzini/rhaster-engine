#ifndef SCENEPOOL_H
#define SCENEPOOL_H

// +--------------------------------+
// | PROJECT HEADERS				|
// +--------------------------------+
#include <framework/Scene.h>
#include <singleton/Singleton.h>

// +--------------------------------+
// | STANDARD HEADERS				|
// +--------------------------------+
#include <memory>
#include <vector>


namespace engine
{
    // todo: make tick and render inaccessible from outside
    class ScenePool final : public Singleton<ScenePool>
    {
        friend class Singleton;

    public:
        Scene& create_scene( const std::string& name );

        void select_scene( const std::string& name );
        void select_first_scene( );

        void unload_scene( const std::string& name );
        void unload_all_scenes( );

        void fixed_tick( ) const;
        void tick( ) const;
        void render( ) const;

        [[nodiscard]] bool does_scene_exist( const std::string& name ) const;

        [[nodiscard]] Scene& get_active_scene( );
        [[nodiscard]] Scene& get_active_scene( ) const;
        [[nodiscard]] Scene& get_scene( const std::string& name ) const;
        [[nodiscard]] Scene& get_scene( uint16_t id ) const;

        void cleanup( ) const;

    private:
        std::vector<std::unique_ptr<Scene>> scenes_;
        Scene* active_scene_ptr_{ nullptr };

        ScenePool( ) = default;

    };

    extern ScenePool& SCENE_POOL;
}

#endif //!SCENEPOOL_H
