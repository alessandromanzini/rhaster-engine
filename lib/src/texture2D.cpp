#include <rst/__type/texture/texture2D.h>

#include <rst/temp/singleton/renderer.h>

#include <SDL.h>
#include <SDL_image.h>


namespace rst
{
    texture_2d::texture_2d( SDL_Texture* texture ) : texture_ptr_{ texture }
    {
        assert( texture_ptr_ != nullptr );
    }


    texture_2d::texture_2d( std::string const& full_path )
    {
        texture_ptr_ = IMG_LoadTexture( RENDERER.sdl_renderer( ), full_path.c_str( ) );
        if ( texture_ptr_ == nullptr )
        {
            throw std::runtime_error( std::string( "Failed to load texture: " ) + SDL_GetError( ) );
        }
    }


    texture_2d::~texture_2d( ) noexcept
    {
        SDL_DestroyTexture( texture_ptr_ );
    }


    auto texture_2d::sdl_texture( ) const -> SDL_Texture*
    {
        return texture_ptr_;
    }


    auto texture_2d::size( ) const -> glm::vec2
    {
        SDL_Rect dst;
        SDL_QueryTexture( sdl_texture( ), nullptr, nullptr, &dst.w, &dst.h );
        return { static_cast<float>( dst.w ), static_cast<float>( dst.h ) };
    }

}
