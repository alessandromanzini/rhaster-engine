#ifndef RST_RENDERER_H
#define RST_RENDERER_H

#include <rst/pch.h>

#include <rst/temp/singleton/Singleton.h>

#include <SDL.h>


// TODO: Pimpl header
namespace rst
{
    namespace render
    {
        struct RenderRequest
        {
            SDL_Texture* texture{ nullptr };
            SDL_Rect dst{};
            SDL_Rect src{};

            bool ex{};

            int z_index{ 0 };
        };

        struct RenderRequestComparator
        {
            auto operator( )( RenderRequest const& lhs, RenderRequest const& rhs ) const -> bool
            {
                return lhs.z_index < rhs.z_index;
            }
        };
    }

    class Texture2D;
    /**
     * Simple RAII wrapper for the SDL renderer
     */
    class Renderer final : public Singleton<Renderer>
    {
        friend class Singleton;

    public:
        auto init( SDL_Window* window ) -> void;
        auto render( ) -> void;
        auto destroy( ) -> void;

        auto set_z_index( int z_index ) -> void;

        auto render_texture( Texture2D const& texture, glm::vec2 position ) -> void;
        auto render_texture( Texture2D const& texture, glm::vec4 const& dst_rect ) -> void;

        auto render_partial_texture( Texture2D const& texture, glm::vec2 position, glm::vec4 const& src_rect ) -> void;
        auto render_partial_texture( Texture2D const& texture, glm::vec4 const& dst_rect, glm::vec4 const& src_rect ) -> void;

        [[nodiscard]] auto get_sdl_renderer( ) const -> SDL_Renderer*;

        [[nodiscard]] auto get_background_color( ) const -> SDL_Color const&;

        auto set_background_color( SDL_Color const& color ) -> void;

    private:
        SDL_Renderer* renderer_ptr_{};
        SDL_Window* window_ptr_{};
        SDL_Color clear_color_{};

        int z_index_{ 0 };
        std::multiset<render::RenderRequest, render::RenderRequestComparator> render_queue_{};

        Renderer( ) = default;

        auto render_requests( ) -> void;

        auto render_texture_ex_impl( SDL_Texture* texture, SDL_Rect const* dst, SDL_Rect* src ) const -> void;
        auto render_texture_impl( SDL_Texture* texture, SDL_Rect const* dst, SDL_Rect const* src ) const -> void;
    };

    extern Renderer& RENDERER;
}


#endif //!RST_RENDERER_H
