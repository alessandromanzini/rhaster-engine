#include <singleton/Renderer.h>

// +--------------------------------+
// | PROJECT HEADERS				|
// +--------------------------------+
#include <framework/resource/texture/Texture2D.h>
#include <singleton/ScenePool.h>
#include <singleton/UIController.h>

// +--------------------------------+
// | STANDARD HEADERS				|
// +--------------------------------+
#include <cstring>
#include <stdexcept>


namespace engine
{
    Renderer& RENDERER = Renderer::get_instance( );

    // +---------------------------+
    // | HELPERS                   |
    // +---------------------------+
    int get_open_gl_driver_index( )
    {
        auto openglIndex       = -1;
        const auto driverCount = SDL_GetNumRenderDrivers( );
        for ( auto i = 0; i < driverCount; i++ )
        {
            SDL_RendererInfo info;
            if ( !SDL_GetRenderDriverInfo( i, &info ) )
            {
                if ( !strcmp( info.name, "opengl" ) )
                {
                    openglIndex = i;
                }
            }
        }
        return openglIndex;
    }


    SDL_Rect create_rect( const glm::vec4& rect )
    {
        return SDL_Rect{
            .x = static_cast<int>( rect.x ),
            .y = static_cast<int>( rect.y ),
            .w = static_cast<int>( rect.z ),
            .h = static_cast<int>( rect.w )
        };
    }


    SDL_Rect create_rect( const glm::vec2 pos )
    {
        return SDL_Rect{
            .x = static_cast<int>( pos.x ),
            .y = static_cast<int>( pos.y )
        };
    }


    // +---------------------------+
    // | RENDERER                  |
    // +---------------------------+
    void Renderer::init( SDL_Window* window )
    {
        window_ptr_   = window;
        renderer_ptr_ = SDL_CreateRenderer( window, get_open_gl_driver_index( ), SDL_RENDERER_ACCELERATED );
        if ( renderer_ptr_ == nullptr )
        {
            throw std::runtime_error( std::string( "SDL_CreateRenderer Error: " ) + SDL_GetError( ) );
        }
    }


    void Renderer::render( )
    {
        const auto& [r, g, b, a] = get_background_color( );

        SDL_SetRenderDrawColor( renderer_ptr_, r, g, b, a );
        SDL_RenderClear( renderer_ptr_ );

        SCENE_POOL.render( );
        UI_CONTROLLER.render( );

        render_requests(  );
        SDL_RenderPresent( renderer_ptr_ );
    }


    void Renderer::destroy( )
    {
        if ( renderer_ptr_ != nullptr )
        {
            SDL_DestroyRenderer( renderer_ptr_ );
            renderer_ptr_ = nullptr;
        }
    }


    void Renderer::set_z_index( const int zIndex )
    {
        z_index_ = zIndex;
    }


    void Renderer::render_texture( const Texture2D& texture, const glm::vec2 position )
    {
        render::RenderRequest request{
            .texture = texture.get_SDL_texture( ),
            .dst = create_rect( position ),
            .ex = false,
            .z_index = z_index_
        };
        SDL_QueryTexture( texture.get_SDL_texture( ), nullptr, nullptr, &request.dst.w, &request.dst.h );
        render_queue_.insert( request );
    }


    void Renderer::render_texture( const Texture2D& texture, const glm::vec4& dstRect )
    {
        render_queue_.insert( {
            .texture = texture.get_SDL_texture( ),
            .dst = create_rect( dstRect ),
            .ex = false,
            .z_index = z_index_
        } );
    }


    void Renderer::render_partial_texture( const Texture2D& texture, const glm::vec2 position, const glm::vec4& srcRect )
    {
        render::RenderRequest request{
            .texture = texture.get_SDL_texture( ),
            .dst = create_rect( position ),
            .src = create_rect( srcRect ),
            .ex = true,
            .z_index = z_index_
        };
        request.dst.w = request.src.w;
        request.dst.h = request.src.h;
        render_queue_.insert( request );
    }


    void Renderer::render_partial_texture( const Texture2D& texture,
                                           const glm::vec4& dstRect,
                                           const glm::vec4& srcRect )
    {
        render_queue_.insert( {
            .texture = texture.get_SDL_texture( ),
            .dst = create_rect( dstRect ),
            .src = create_rect( srcRect ),
            .ex = true,
            .z_index = z_index_
        } );
    }


    SDL_Renderer* Renderer::get_sdl_renderer( ) const
    {
        return renderer_ptr_;
    }


    const SDL_Color& Renderer::get_background_color( ) const
    {
        return clear_color_;
    }


    void Renderer::set_background_color( const SDL_Color& color )
    {
        clear_color_ = color;
    }


    void Renderer::render_requests( )
    {
        for ( const auto& request : render_queue_ )
        {
            if ( request.ex )
            {
                auto src = request.src;
                render_texture_ex_impl( request.texture, &request.dst, &src );
            }
            else
            {
                render_texture_impl( request.texture, &request.dst, nullptr );
            }
        }
        render_queue_.clear( );
    }


    void Renderer::render_texture_ex_impl( SDL_Texture* texture, const SDL_Rect* dst, SDL_Rect* src ) const
    {
        SDL_RendererFlip flip{ SDL_FLIP_NONE };
        if ( src->w < 0 )
        {
            flip   = static_cast<SDL_RendererFlip>( flip | SDL_FLIP_HORIZONTAL );
            src->w = -src->w;
        }
        if ( src->h < 0 )
        {
            flip   = static_cast<SDL_RendererFlip>( flip | SDL_FLIP_VERTICAL );
            src->h = -src->h;
        }

        SDL_RenderCopyEx( get_sdl_renderer( ), texture, src, dst, 0, nullptr, flip );
    }


    void Renderer::render_texture_impl( SDL_Texture* texture, const SDL_Rect* dst, const SDL_Rect* src ) const
    {
        SDL_RenderCopy( get_sdl_renderer( ), texture, src, dst );
    }

}
