#include <rst/__internal/resource/sdl_pelt.h>

#include <SDL.h>
#include <SDL_image.h>


namespace rst
{
    sdl_pelt::sdl_pelt( SDL_Renderer& renderer, earmark const mark, std::filesystem::path const& full_path )
        : pelt{ mark }
    {
        // 1. load the image
        texture_ptr_ = IMG_LoadTexture( &renderer, full_path.c_str( ) );
        if ( texture_ptr_ == nullptr )
        {
            throw std::runtime_error{ std::format( "IMG_LoadTexture error: {}", SDL_GetError( ) ) };
        }

        // 2. query the texture to get its width and height
        SDL_Rect dst;
        SDL_QueryTexture( sdl_texture( ), nullptr, nullptr, &dst.w, &dst.h );
        dimensions_ = glm::vec2{ static_cast<float>( dst.w ), static_cast<float>( dst.h ) };
    }


    sdl_pelt::~sdl_pelt( ) noexcept
    {
        SDL_DestroyTexture( texture_ptr_ );
    }


    auto sdl_pelt::dimensions( ) const noexcept -> glm::vec2
    {
        return dimensions_;
    }


    auto sdl_pelt::sdl_texture( ) const noexcept -> SDL_Texture*
    {
        return texture_ptr_;
    }
}
