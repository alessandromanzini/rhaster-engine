#ifndef RST_RESOURCE_MANAGER_H
#define RST_RESOURCE_MANAGER_H

#include <rst/pch.h>

#include <rst/framework/data_type.h>
#include <rst/framework/observer.h>
#include <rst/temp/singleton/Singleton.h>


namespace rst
{
    namespace event
    {
        enum class LifetimeEvent
        {
            // Unload unused for all resources
            UNLOAD_ALL,

            // Unload unused audio resources
            UNLOAD_AUDIO,

            // Unload unused texture resources
            UNLOAD_TEXTURES,

            // Unload unused font resources
            UNLOAD_FONTS
        };
    }

    // TODO: move all loading functions to services and use Observer pattern to notify when unloading
    class Texture2D;
    class Font;
    class ResourceManager final : public Singleton<ResourceManager>
    {
        friend class Singleton;
        using queued_event_t = std::pair<event::LifetimeEvent, UID>;

    public:
        auto init( std::filesystem::path const& data_path ) -> void;

        auto load_texture( std::filesystem::path const& path ) -> std::shared_ptr<Texture2D>;
        auto load_font( std::filesystem::path const& path, uint8_t size ) -> std::shared_ptr<Font>;

        [[nodiscard]] auto get_data_path( ) const -> std::filesystem::path const&;

        auto add_lifetime_observer( Observer& observer ) -> void;
        auto remove_lifetime_observer( Observer const& observer ) -> void;

        auto signal_lifetime_event( event::LifetimeEvent event, UID value = NULL_UID ) -> void;

        auto unload_unused_resources( ) -> void;

        // template <typename map_t>
        // static void unload_unused_resources_impl( map_t& map );

    private:
        std::filesystem::path data_path_{};

        thread::SafeResource<std::set<queued_event_t>> queued_events_{ {} };
        Subject lifetime_subject_{};

        std::map<UID, std::shared_ptr<Texture2D>> loaded_textures_{};
        std::map<std::pair<UID, uint8_t>, std::shared_ptr<Font>> loaded_fonts_{};

        ResourceManager( ) = default;
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


    extern ResourceManager& RESOURCE_MANAGER;
}

#endif //!RST_RESOURCE_MANAGER_H
