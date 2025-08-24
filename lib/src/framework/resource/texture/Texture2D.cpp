#include <framework/resource/texture/Texture2D.h>

// +--------------------------------+
// | PROJECT HEADERS				|
// +--------------------------------+
#include <singleton/Renderer.h>

// +--------------------------------+
// | SDL HEADERS					|
// +--------------------------------+
#include <SDL.h>
#include <SDL_image.h>

// +--------------------------------+
// | STANDARD HEADERS				|
// +--------------------------------+
#include <stdexcept>


namespace engine
{
    Texture2D::Texture2D( const std::string& fullPath )
    {
        texture_ptr_ = IMG_LoadTexture( RENDERER.get_sdl_renderer( ), fullPath.c_str( ) );
        if ( texture_ptr_ == nullptr )
            throw std::runtime_error( std::string( "Failed to load texture: " ) + SDL_GetError( ) );
    }


    Texture2D::Texture2D( SDL_Texture* texture ) : texture_ptr_{ texture }
    {
        assert( texture_ptr_ != nullptr );
    }


    Texture2D::~Texture2D( ) noexcept
    {
        SDL_DestroyTexture( texture_ptr_ );
    }


    glm::vec2 Texture2D::get_size( ) const
    {
        SDL_Rect dst;
        SDL_QueryTexture( get_SDL_texture( ), nullptr, nullptr, &dst.w, &dst.h );
        return { static_cast<float>( dst.w ), static_cast<float>( dst.h ) };
    }


    SDL_Texture* Texture2D::get_SDL_texture( ) const
    {
        return texture_ptr_;
    }

}
