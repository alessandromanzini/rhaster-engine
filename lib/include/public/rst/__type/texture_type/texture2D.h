#ifndef RST_TEXTURE_2D_H
#define RST_TEXTURE_2D_H

#include <rst/pch.h>


struct SDL_Texture;


// TODO: add documentation, pimpl sdl
namespace rst
{
    /**
     * Simple RAII wrapper for an SDL_Texture
     */
    class Texture2D final
    {
    public:
        explicit Texture2D( SDL_Texture* texture );
        explicit Texture2D( std::string const& full_path );
        ~Texture2D( ) noexcept;

        Texture2D( Texture2D const& )                        = delete;
        Texture2D( Texture2D&& ) noexcept                    = delete;
        auto operator=( Texture2D const& ) -> Texture2D&     = delete;
        auto operator=( Texture2D&& ) noexcept -> Texture2D& = delete;

        [[nodiscard]] auto get_sdl_texture( ) const -> SDL_Texture*;

        [[nodiscard]] auto get_size( ) const -> glm::vec2;

    private:
        SDL_Texture* texture_ptr_;
    };
}


#endif //!RST_TEXTURE_2D_H
