#include <rst/temp/singleton/resource_manager.h>

#include <rst/framework/observer.h>
#include <rst/framework/resource_type.h>
#include <rst/temp/singleton/renderer.h>

#include <SDL_image.h>
#include <SDL_ttf.h>


namespace fs = std::filesystem;

namespace rst
{
    auto ResourceManager::init( std::filesystem::path const& data_path ) -> void
    {
        data_path_ = data_path;

        if ( TTF_Init( ) != 0 )
        {
            throw std::runtime_error( std::string( "Failed to load support for fonts: " ) + SDL_GetError( ) );
        }
    }


    auto ResourceManager::load_texture( std::filesystem::path const& path ) -> std::shared_ptr<Texture2D>
    {
        Uid const uid{ path.string( ) };
        std::filesystem::path const full_path{ data_path_ / path };
        if ( not loaded_textures_.contains( uid ) )
        {
            loaded_textures_.insert( std::pair( uid, std::make_shared<Texture2D>( full_path.string( ) ) ) );
        }
        return loaded_textures_.at( uid );
    }


    auto ResourceManager::load_font( std::filesystem::path const& path, uint8_t size ) -> std::shared_ptr<Font>
    {
        Uid const uid{ path.string( ) };
        std::filesystem::path const full_path{ data_path_ / path };
        auto const key = std::make_pair( uid, size );
        if ( not loaded_fonts_.contains( key ) )
        {
            loaded_fonts_.insert( std::pair( key, std::make_shared<Font>( full_path.string( ), size ) ) );
        }
        return loaded_fonts_.at( key );
    }


    auto ResourceManager::get_data_path( ) const -> std::filesystem::path const& { return data_path_; }


    auto ResourceManager::add_lifetime_observer( Observer& observer ) -> void
    {
        lifetime_subject_.add_observer( observer );
    }


    auto ResourceManager::remove_lifetime_observer( Observer const& observer ) -> void
    {
        lifetime_subject_.remove_observer( observer );
    }


    auto ResourceManager::signal_lifetime_event( event::LifetimeEvent event, Uid value ) -> void
    {
        auto [lock, events] = queued_events_.get( );
        events.insert( std::make_pair( event, value ) );
    }


    auto ResourceManager::unload_unused_resources( ) -> void
    {
        // unload_unused_resources_impl( loaded_textures_ );
        // unload_unused_resources_impl( loaded_fonts_ );

        for ( auto const& [event, value] : queued_events_.cget( ) )
        {
            lifetime_subject_.broadcast( Uid( event ), value );
        }

        auto [lock, events] = queued_events_.get( );
        events.clear( );
    }


    ResourceManager& RESOURCE_MANAGER = ResourceManager::get_instance( );
}
