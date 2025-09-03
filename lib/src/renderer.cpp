#include <rst/temp/singleton/renderer.h>

#include <rst/temp/singleton/scene_pool.h>
#include <rst/__type/texture/texture2D.h>


namespace rst
{
    renderer& RENDERER = renderer::instance( );

    // +---------------------------+
    // | HELPERS                   |
    // +---------------------------+
    auto get_open_gl_driver_index( ) -> int
    {
        auto opengl_index       = -1;
        auto const driver_count = SDL_GetNumRenderDrivers( );
        for ( auto i = 0; i < driver_count; i++ )
        {
            SDL_RendererInfo info;
            if ( !SDL_GetRenderDriverInfo( i, &info ) )
            {
                if ( !strcmp( info.name, "opengl" ) )
                {
                    opengl_index = i;
                }
            }
        }
        return opengl_index;
    }


    auto create_rect( glm::vec4 const& rect ) -> SDL_Rect
    {
        return SDL_Rect{
            .x = static_cast<int>( rect.x ),
            .y = static_cast<int>( rect.y ),
            .w = static_cast<int>( rect.z ),
            .h = static_cast<int>( rect.w )
        };
    }


    auto create_rect( glm::vec2 const pos ) -> SDL_Rect
    {
        return SDL_Rect{
            .x = static_cast<int>( pos.x ),
            .y = static_cast<int>( pos.y ),
            .w = 0,
            .h = 0
        };
    }


    // +---------------------------+
    // | RENDERER                  |
    // +---------------------------+
    auto renderer::init( SDL_Window* window ) -> void
    {
        window_ptr_   = window;
        renderer_ptr_ = SDL_CreateRenderer( window, get_open_gl_driver_index( ), SDL_RENDERER_ACCELERATED );
        if ( renderer_ptr_ == nullptr )
        {
            throw std::runtime_error( std::string( "SDL_CreateRenderer Error: " ) + SDL_GetError( ) );
        }
    }


    auto renderer::render( ) -> void
    {
        auto const& [r, g, b, a] = background_color( );

        SDL_SetRenderDrawColor( renderer_ptr_, r, g, b, a );
        SDL_RenderClear( renderer_ptr_ );

        SCENE_POOL.render( );

        render_requests( );
        SDL_RenderPresent( renderer_ptr_ );
    }


    auto renderer::destroy( ) -> void
    {
        if ( renderer_ptr_ != nullptr )
        {
            SDL_DestroyRenderer( renderer_ptr_ );
            renderer_ptr_ = nullptr;
        }
    }


    auto renderer::set_z_index( int const z_index ) -> void
    {
        z_index_ = z_index;
    }


    auto renderer::render_texture( texture_2d const& texture, glm::vec2 const position ) -> void
    {
        render::request request{
            .texture = texture.sdl_texture( ),
            .dst = create_rect( position ),
            .ex = false,
            .z_index = z_index_
        };
        SDL_QueryTexture( request.texture, nullptr, nullptr, &request.dst.w, &request.dst.h );
        render_queue_.insert( request );
    }


    auto renderer::render_texture( texture_2d const& texture, glm::vec4 const& dst_rect ) -> void
    {
        render_queue_.insert(
            {
                .texture = texture.sdl_texture( ),
                .dst = create_rect( dst_rect ),
                .ex = false,
                .z_index = z_index_
            } );
    }


    auto renderer::render_partial_texture(
        texture_2d const& texture, glm::vec2 const position, glm::vec4 const& src_rect ) -> void
    {
        render::request request{
            .texture = texture.sdl_texture( ),
            .dst = create_rect( position ),
            .src = create_rect( src_rect ),
            .ex = true,
            .z_index = z_index_
        };
        request.dst.w = request.src.w;
        request.dst.h = request.src.h;
        render_queue_.insert( request );
    }


    auto renderer::render_partial_texture(
        texture_2d const& texture, glm::vec4 const& dst_rect, glm::vec4 const& src_rect ) -> void
    {
        render_queue_.insert(
            {
                .texture = texture.sdl_texture( ),
                .dst = create_rect( dst_rect ),
                .src = create_rect( src_rect ),
                .ex = true,
                .z_index = z_index_
            } );
    }


    auto renderer::sdl_renderer( ) const -> SDL_Renderer*
    {
        return renderer_ptr_;
    }


    auto renderer::background_color( ) const -> SDL_Color const&
    {
        return clear_color_;
    }


    auto renderer::set_background_color( SDL_Color const& color ) -> void
    {
        clear_color_ = color;
    }


    auto renderer::render_requests( ) -> void
    {
        for ( auto const& request : render_queue_ )
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


    auto renderer::render_texture_ex_impl( SDL_Texture* texture, SDL_Rect const* dst, SDL_Rect* src ) const -> void
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

        SDL_RenderCopyEx( sdl_renderer( ), texture, src, dst, 0, nullptr, flip );
    }


    auto renderer::render_texture_impl( SDL_Texture* texture, SDL_Rect const* dst, SDL_Rect const* src ) const -> void
    {
        SDL_RenderCopy( sdl_renderer( ), texture, src, dst );
    }
}
