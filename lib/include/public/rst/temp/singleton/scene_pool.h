#ifndef RST_SCENE_POOL_H
#define RST_SCENE_POOL_H

#include <rst/pch.h>

#include <rst/temp/singleton/Singleton.h>
#include <rst/__core/scene.h>


namespace rst
{
    // todo: make tick and render inaccessible from outside
    class scene_pool final : public singleton<scene_pool>
    {
        friend class singleton;

    public:
        auto create_scene( std::string const& name ) -> scene&;

        auto select_scene( std::string const& name ) -> void;
        auto select_first_scene( ) -> void;

        auto unload_scene( std::string const& name ) -> void;
        auto unload_all_scenes( ) -> void;

        auto fixed_tick( ) const -> void;
        auto tick( ) const -> void;
        auto render( ) const -> void;

        [[nodiscard]] auto does_scene_exist( std::string const& name ) const -> bool;

        [[nodiscard]] auto active_scene( ) -> scene&;
        [[nodiscard]] auto active_scene( ) const -> scene&;
        [[nodiscard]] auto scene( std::string const& name ) const -> scene&;
        [[nodiscard]] auto scene( uint16_t id ) const -> rst::scene&;

        auto cleanup( ) const -> void;

    private:
        std::vector<std::unique_ptr<rst::scene>> scenes_;
        rst::scene* active_scene_ptr_{ nullptr };

        scene_pool( ) = default;
    };

    extern scene_pool& SCENE_POOL;
}


#endif //!RST_SCENE_POOL_H
