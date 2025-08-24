#ifndef RESOURCEMANAGER_H
#define RESOURCEMANAGER_H

// +--------------------------------+
// | PROJECT HEADERS				|
// +--------------------------------+
#include <core/UID.h>
#include <framework/event/Subject.h>
#include <framework/resource/data/SafeResource.h>
#include <singleton/Singleton.h>

// +--------------------------------+
// | STANDARD HEADERS				|
// +--------------------------------+
#include <filesystem>
#include <map>
#include <set>


namespace engine
{
    namespace event
    {
        enum class LifetimeEvent
        {
            UNLOAD_ALL, // Unload unused for all resources

            UNLOAD_AUDIO,    // Unload unused audio resources
            UNLOAD_TEXTURES, // Unload unused texture resources
            UNLOAD_FONTS     // Unload unused font resources
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
        void init( const std::filesystem::path& dataPath );

        std::shared_ptr<Texture2D> load_texture( const std::filesystem::path& path );
        std::shared_ptr<Font> load_font( const std::filesystem::path& path, uint8_t size );

        [[nodiscard]] const std::filesystem::path& get_data_path( ) const;

        void add_lifetime_observer( Observer& observer );
        void remove_lifetime_observer( const Observer& observer );

        void signal_lifetime_event( event::LifetimeEvent event, UID value = NULL_UID );

        void unload_unused_resources( );

        // template <typename map_t>
        // static void unload_unused_resources_impl( map_t& map );

    private:
        std::filesystem::path data_path_{};

        threading::SafeResource<std::set<queued_event_t>> queued_events_{ {} };
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

#endif //!RESOURCEMANAGER_H
