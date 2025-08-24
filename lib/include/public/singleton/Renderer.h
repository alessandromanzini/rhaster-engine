#ifndef RENDERER_H
#define RENDERER_H

// +--------------------------------+
// | SDL HEADERS					|
// +--------------------------------+
#include <SDL.h>

// +--------------------------------+
// | PROJECT HEADERS				|
// +--------------------------------+
#include <singleton/Singleton.h>
#include <singleton/UIController.h>

#include <glm.hpp>

#include <set>


namespace engine
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
            bool operator( )( const RenderRequest& lhs, const RenderRequest& rhs ) const
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
        void init( SDL_Window* window );
        void render( );
        void destroy( );

        void set_z_index( int zIndex );

        void render_texture( const Texture2D& texture, glm::vec2 position );
        void render_texture( const Texture2D& texture, const glm::vec4& dstRect );

        void render_partial_texture( const Texture2D& texture, glm::vec2 position, const glm::vec4& srcRect );
        void render_partial_texture( const Texture2D& texture, const glm::vec4& dstRect, const glm::vec4& srcRect );

        [[nodiscard]] SDL_Renderer* get_sdl_renderer( ) const;

        [[nodiscard]] const SDL_Color& get_background_color( ) const;

        void set_background_color( const SDL_Color& color );

        friend void UIController::init( );

    private:
        SDL_Renderer* renderer_ptr_{};
        SDL_Window* window_ptr_{};
        SDL_Color clear_color_{};

        int z_index_{ 0 };
        std::multiset<render::RenderRequest, render::RenderRequestComparator> render_queue_{};

        Renderer( ) = default;

        void render_requests( );

        void render_texture_ex_impl( SDL_Texture* texture, const SDL_Rect* dst, SDL_Rect* src ) const;
        void render_texture_impl( SDL_Texture* texture, const SDL_Rect* dst, const SDL_Rect* src ) const;

    };

    extern Renderer& RENDERER;

}

#endif //!RENDERER_H
