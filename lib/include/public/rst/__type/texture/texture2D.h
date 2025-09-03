#ifndef RST_TEXTURE_2D_H
#define RST_TEXTURE_2D_H

#include <rst/pch.h>


// ReSharper disable once CppInconsistentNaming
struct SDL_Texture;


// TODO: add documentation, pimpl sdl
namespace rst
{
    /**
     * Simple RAII wrapper for an SDL_Texture
     */
    class texture_2d final
    {
    public:
        explicit texture_2d( SDL_Texture* texture );
        explicit texture_2d( std::string const& full_path );
        ~texture_2d( ) noexcept;

        texture_2d( texture_2d const& )                        = delete;
        texture_2d( texture_2d&& ) noexcept                    = delete;
        auto operator=( texture_2d const& ) -> texture_2d&     = delete;
        auto operator=( texture_2d&& ) noexcept -> texture_2d& = delete;

        [[nodiscard]] auto sdl_texture( ) const -> SDL_Texture*;
        [[nodiscard]] auto size( ) const -> glm::vec2;

    private:
        SDL_Texture* texture_ptr_;
    };
}


#endif //!RST_TEXTURE_2D_H
