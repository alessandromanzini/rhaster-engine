#ifndef RST_RESOURCE_MANAGER_H
#define RST_RESOURCE_MANAGER_H

#include <rst/pch.h>

#include <rst/framework/observer.h>
#include <rst/temp/singleton/Singleton.h>
#include <rst/type/safe_resource.h>


namespace rst
{
    namespace event
    {
        enum class lifetime_event
        {
            // Unload unused for all resources
            unload_all,

            // Unload unused audio resources
            unload_audio,

            // Unload unused texture resources
            unload_textures,

            // Unload unused font resources
            unload_fonts
        };
    }

    // TODO: move all loading functions to services and use Observer pattern to notify when unloading
    class texture_2d;
    class font;
    class resource_manager final : public singleton<resource_manager>
    {
        friend class singleton;
        using queued_event_type = std::pair<event::lifetime_event, earmark>;

    public:
        auto init( std::filesystem::path const& data_path ) -> void;

        auto load_texture( std::filesystem::path const& path ) -> std::shared_ptr<texture_2d>;
        auto load_font( std::filesystem::path const& path, uint8_t size ) -> std::shared_ptr<font>;

        [[nodiscard]] auto data_path( ) const -> std::filesystem::path const&;

        auto add_lifetime_observer( observer& observer ) -> void;
        auto remove_lifetime_observer( observer const& observer ) -> void;

        auto signal_lifetime_event( event::lifetime_event event, earmark value = earmark::null ) -> void;

        auto unload_unused_resources( ) -> void;

        // template <typename map_t>
        // static void unload_unused_resources_impl( map_t& map );

    private:
        std::filesystem::path data_path_{};

        thread::safe_resource<std::set<queued_event_type>> queued_events_{ {} };
        subject lifetime_subject_{};

        std::map<earmark, std::shared_ptr<texture_2d>> loaded_textures_{};
        std::map<std::pair<earmark, uint8_t>, std::shared_ptr<font>> loaded_fonts_{};

        resource_manager( ) = default;
    };


    // template <typename map_t>
    // void ResourceManager::unload_unused_resources_impl( map_t& map )
    // {
    //     for ( auto it = map.begin( ); it != map.end( ); )
    //     {
    //         if ( it->second.use_count( ) == 1 )
    //         {
    //             it = map.erase( it );
    //         }
    //         else
    //         {
    //             ++it;
    //         }
    //     }
    // }


    extern resource_manager& RESOURCE_MANAGER;
}

#endif //!RST_RESOURCE_MANAGER_H
