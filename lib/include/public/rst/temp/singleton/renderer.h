#ifndef RST_RENDERER_H
#define RST_RENDERER_H

#include <rst/pch.h>

#include <rst/temp/singleton/Singleton.h>

#include <SDL.h>


// TODO: Pimpl header
namespace rst
{
    class texture_2d;
}

namespace rst
{
    namespace render
    {
        struct request
        {
            SDL_Texture* texture{ nullptr };
            SDL_Rect dst{};
            SDL_Rect src{};

            bool ex{};

            int z_index{ 0 };
        };

        struct request_comparator
        {
            auto operator( )( request const& lhs, request const& rhs ) const -> bool
            {
                return lhs.z_index < rhs.z_index;
            }
        };
    }


    /**
     * Simple RAII wrapper for the SDL renderer
     */
    class renderer final : public singleton<renderer>
    {
        friend class singleton;

    public:
        auto init( SDL_Window* window ) -> void;
        auto render( ) -> void;
        auto destroy( ) -> void;

        auto set_z_index( int z_index ) -> void;

        auto render_texture( texture_2d const& texture, glm::vec2 position ) -> void;
        auto render_texture( texture_2d const& texture, glm::vec4 const& dst_rect ) -> void;

        auto render_partial_texture( texture_2d const& texture, glm::vec2 position, glm::vec4 const& src_rect ) -> void;
        auto render_partial_texture( texture_2d const& texture, glm::vec4 const& dst_rect, glm::vec4 const& src_rect ) -> void;

        [[nodiscard]] auto sdl_renderer( ) const -> SDL_Renderer*;

        [[nodiscard]] auto background_color( ) const -> SDL_Color const&;

        auto set_background_color( SDL_Color const& color ) -> void;

    private:
        SDL_Renderer* renderer_ptr_{};
        SDL_Window* window_ptr_{};
        SDL_Color clear_color_{};

        int z_index_{ 0 };
        std::multiset<render::request, render::request_comparator> render_queue_{};

        renderer( ) = default;

        auto render_requests( ) -> void;

        auto render_texture_ex_impl( SDL_Texture* texture, SDL_Rect const* dst, SDL_Rect* src ) const -> void;
        auto render_texture_impl( SDL_Texture* texture, SDL_Rect const* dst, SDL_Rect const* src ) const -> void;
    };

    extern renderer& RENDERER;
}


#endif //!RST_RENDERER_H
