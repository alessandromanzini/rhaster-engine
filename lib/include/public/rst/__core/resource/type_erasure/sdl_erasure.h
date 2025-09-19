#ifndef RST_SDL_ERASURE_H
#define RST_SDL_ERASURE_H

#include <rst/pch.h>


namespace rst
{
    class pelt;
}

namespace rst::internal::sdl
{
    /**
     * @brief PIMPL wrapper for SDL_Window to avoid namespace pollution.
     *
     * This class provides a complete abstraction over SDL_Window without
     * requiring any SDL forward declarations in public headers.
     */
    class opaque_window final
    {
    public:
        opaque_window( std::string_view const& title, glm::vec2 viewport );
        ~opaque_window( ) noexcept;

        opaque_window( opaque_window const& )                        = delete;
        opaque_window( opaque_window&& ) noexcept                    = delete;
        auto operator=( opaque_window const& ) -> opaque_window&     = delete;
        auto operator=( opaque_window&& ) noexcept -> opaque_window& = delete;

        [[nodiscard]] auto get( ) noexcept -> void*;
        [[nodiscard]] auto get( ) const noexcept -> void const*;
        [[nodiscard]] auto viewport( ) const noexcept -> glm::vec2;

    private:
        struct impl;
        std::unique_ptr<impl> impl_ptr_{ nullptr };
        glm::vec2 const viewport_;
    };


    /**
     * @brief PIMPL wrapper for SDL_Renderer to avoid namespace pollution.
     *
     * This class provides a complete abstraction over SDL_Renderer without
     * requiring any SDL forward declarations in public headers.
     */
    class opaque_renderer final
    {
    public:
        explicit opaque_renderer( opaque_window& window );
        ~opaque_renderer( ) noexcept;

        opaque_renderer( opaque_renderer const& )                        = delete;
        opaque_renderer( opaque_renderer&& ) noexcept                    = delete;
        auto operator=( opaque_renderer const& ) -> opaque_renderer&     = delete;
        auto operator=( opaque_renderer&& ) noexcept -> opaque_renderer& = delete;

        [[nodiscard]] auto load_texture( std::filesystem::path const& full_path ) const -> std::shared_ptr<pelt>;

        auto render_pelt( pelt const& texture, glm::vec4 const& dst ) const noexcept -> void;
        auto render_pelt( pelt const& texture, glm::vec4 const& dst, glm::vec4 const& src ) const noexcept -> void;
        auto render_pelt_ex( pelt const& texture, glm::vec4 const& dst, glm::vec4 const& src ) const noexcept -> void;

        auto clear( glm::vec4 const& color ) const noexcept -> void;
        auto present( ) const noexcept -> void;

    private:
        struct impl;
        std::unique_ptr<impl> impl_ptr_{ nullptr };
    };
}


#endif //!RST_SDL_ERASURE_H
