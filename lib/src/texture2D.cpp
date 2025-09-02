#include <rst/__type/texture_type/texture2D.h>

#include <rst/temp/singleton/renderer.h>

#include <SDL.h>
#include <SDL_image.h>


namespace rst
{
    Texture2D::Texture2D( SDL_Texture* texture ) : texture_ptr_{ texture }
    {
        assert( texture_ptr_ != nullptr );
    }


    Texture2D::Texture2D( std::string const& full_path )
    {
        texture_ptr_ = IMG_LoadTexture( RENDERER.get_sdl_renderer( ), full_path.c_str( ) );
        if ( texture_ptr_ == nullptr )
        {
            throw std::runtime_error( std::string( "Failed to load texture: " ) + SDL_GetError( ) );
        }
    }


    Texture2D::~Texture2D( ) noexcept
    {
        SDL_DestroyTexture( texture_ptr_ );
    }


    auto Texture2D::get_sdl_texture( ) const -> SDL_Texture*
    {
        return texture_ptr_;
    }


    auto Texture2D::get_size( ) const -> glm::vec2
    {
        SDL_Rect dst;
        SDL_QueryTexture( get_sdl_texture( ), nullptr, nullptr, &dst.w, &dst.h );
        return { static_cast<float>( dst.w ), static_cast<float>( dst.h ) };
    }

}
