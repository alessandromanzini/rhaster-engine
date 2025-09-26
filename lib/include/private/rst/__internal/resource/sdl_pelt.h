#ifndef RST_SDL_PELT_H
#define RST_SDL_PELT_H

#include <rst/pch.h>

#include <rst/__core/resource/pelt.h>


// ReSharper disable CppInconsistentNaming
struct SDL_Renderer;
struct SDL_Texture;
// ReSharper restore CppInconsistentNaming

namespace rst
{
    class sdl_pelt final : public pelt
    {
    public:
        explicit sdl_pelt( SDL_Renderer& renderer, earmark mark, std::filesystem::path const& full_path );
        ~sdl_pelt( ) noexcept override;

        sdl_pelt( sdl_pelt const& )                        = delete;
        sdl_pelt( sdl_pelt&& ) noexcept                    = delete;
        auto operator=( sdl_pelt const& ) -> sdl_pelt&     = delete;
        auto operator=( sdl_pelt&& ) noexcept -> sdl_pelt& = delete;

        [[nodiscard]] auto dimensions( ) const noexcept -> glm::vec2 override;
        [[nodiscard]] auto sdl_texture( ) const noexcept -> SDL_Texture*;

    private:
        SDL_Texture* texture_ptr_{ nullptr };
        glm::vec2 dimensions_{};
    };
}


#endif //!RST_SDL_PELT_H
