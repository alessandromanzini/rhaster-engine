#include <singleton/ResourceManager.h>

// +--------------------------------+
// | PROJECT HEADERS				|
// +--------------------------------+
#include <framework/event/Observer.h>
#include <framework/resource/Font.h>
#include <framework/resource/texture/Texture2D.h>
#include <singleton/Renderer.h>

// +--------------------------------+
// | SDL HEADERS					|
// +--------------------------------+
#include <SDL_image.h>
#include <SDL_ttf.h>

// +--------------------------------+
// | STANDARD HEADERS				|
// +--------------------------------+
#include <stdexcept>


namespace fs = std::filesystem;

namespace engine
{
    ResourceManager& RESOURCE_MANAGER = ResourceManager::get_instance( );


    void ResourceManager::init( const std::filesystem::path& dataPath )
    {
        data_path_ = dataPath;

        if ( TTF_Init( ) != 0 )
        {
            throw std::runtime_error( std::string( "Failed to load support for fonts: " ) + SDL_GetError( ) );
        }
    }


    std::shared_ptr<Texture2D> ResourceManager::load_texture( const std::filesystem::path& path )
    {
        const UID uid{ path.string( ) };

        const auto fullPath = data_path_ / path;
        const auto filename = fs::path( fullPath ).filename( ).string( );

        if ( not loaded_textures_.contains( uid ) )
        {
            loaded_textures_.insert( std::pair( uid, std::make_shared<Texture2D>( fullPath.string( ) ) ) );
        }
        return loaded_textures_.at( uid );
    }


    std::shared_ptr<Font> ResourceManager::load_font( const std::filesystem::path& path, uint8_t size )
    {
        const UID uid{ path.string( ) };

        const auto fullPath = data_path_ / path;
        const auto filename = fs::path( fullPath ).filename( ).string( );

        const auto key      = std::make_pair( uid, size );
        if ( not loaded_fonts_.contains( key ) )
        {
            loaded_fonts_.insert( std::pair( key, std::make_shared<Font>( fullPath.string( ), size ) ) );
        }
        return loaded_fonts_.at( key );
    }


    const std::filesystem::path& ResourceManager::get_data_path( ) const { return data_path_; }


    void ResourceManager::add_lifetime_observer( Observer& observer )
    {
        lifetime_subject_.add_observer( observer );
    }


    void ResourceManager::remove_lifetime_observer( const Observer& observer )
    {
        lifetime_subject_.remove_observer( observer );
    }


    void ResourceManager::signal_lifetime_event( event::LifetimeEvent event, UID value )
    {
        auto [lock, events] = queued_events_.get( );
        events.insert( std::make_pair( event, value ) );
    }


    void ResourceManager::unload_unused_resources( )
    {
        // unload_unused_resources_impl( loaded_textures_ );
        // unload_unused_resources_impl( loaded_fonts_ );

        for ( const auto& [event, value] : queued_events_.cget(  ) )
        {
            lifetime_subject_.broadcast( UID( event ), value );
        }

        auto [lock, events] = queued_events_.get( );
        events.clear( );
    }

}
