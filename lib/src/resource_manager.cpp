#include <rst/temp/singleton/resource_manager.h>

#include <rst/framework/observer.h>
#include <rst/temp/singleton/renderer.h>
#include <rst/__type/font/font.h>
#include <rst/__type/texture/texture2D.h>

#include <SDL_image.h>
#include <SDL_ttf.h>


namespace fs = std::filesystem;

namespace rst
{
    auto resource_manager::init( std::filesystem::path const& data_path ) -> void
    {
        data_path_ = data_path;

        if ( TTF_Init( ) != 0 )
        {
            throw std::runtime_error( std::string( "Failed to load support for fonts: " ) + SDL_GetError( ) );
        }
    }


    auto resource_manager::load_texture( std::filesystem::path const& path ) -> std::shared_ptr<texture_2d>
    {
        earmark const mark{ path.string( ) };
        std::filesystem::path const full_path{ data_path_ / path };
        if ( not loaded_textures_.contains( mark ) )
        {
            loaded_textures_.insert( std::pair( mark, std::make_shared<texture_2d>( full_path.string( ) ) ) );
        }
        return loaded_textures_.at( mark );
    }


    auto resource_manager::load_font( std::filesystem::path const& path, uint8_t size ) -> std::shared_ptr<font>
    {
        earmark const mark{ path.string( ) };
        std::filesystem::path const full_path{ data_path_ / path };
        auto const key = std::make_pair( mark, size );
        if ( not loaded_fonts_.contains( key ) )
        {
            loaded_fonts_.insert( std::pair( key, std::make_shared<font>( full_path.string( ), size ) ) );
        }
        return loaded_fonts_.at( key );
    }


    auto resource_manager::data_path( ) const -> std::filesystem::path const& { return data_path_; }


    auto resource_manager::add_lifetime_observer( observer& observer ) -> void
    {
        lifetime_subject_.add_observer( observer );
    }


    auto resource_manager::remove_lifetime_observer( observer const& observer ) -> void
    {
        lifetime_subject_.remove_observer( observer );
    }


    auto resource_manager::signal_lifetime_event( event::lifetime_event event, earmark value ) -> void
    {
        auto [lock, events] = queued_events_.get( );
        events.insert( std::make_pair( event, value ) );
    }


    auto resource_manager::unload_unused_resources( ) -> void
    {
        // unload_unused_resources_impl( loaded_textures_ );
        // unload_unused_resources_impl( loaded_fonts_ );

        for ( auto const& [event, value] : queued_events_.cget( ) )
        {
            lifetime_subject_.broadcast( earmark( event ), value );
        }

        auto [lock, events] = queued_events_.get( );
        events.clear( );
    }


    resource_manager& RESOURCE_MANAGER = resource_manager::instance( );
}
