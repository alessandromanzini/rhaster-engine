#include <rst/__core/resource/type_erasure/sdl_erasure.h>

#include <rst/diagnostic.h>
#include <rst/__internal/resource/sdl_pelt.h>

#include <SDL.h>


namespace rst::internal::sdl
{
    // +--------------------------------+
    // | HELPERS                        |
    // +--------------------------------+
    auto get_open_gl_driver_index( ) -> int
    {
        int opengl_index       = -1;
        int const driver_count = SDL_GetNumRenderDrivers( );
        for ( int i = 0; i < driver_count; i++ )
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


    [[nodiscard]] auto create_rect( glm::vec4 const& rect ) noexcept -> SDL_Rect
    {
        return SDL_Rect{
            .x = static_cast<int>( rect.x ),
            .y = static_cast<int>( rect.y ),
            .w = static_cast<int>( rect.z ),
            .h = static_cast<int>( rect.w )
        };
    }


    [[nodiscard]] auto create_rect( glm::vec2 const pos, int const w = 0, int const h = 0 ) noexcept -> SDL_Rect
    {
        return SDL_Rect{
            .x = static_cast<int>( pos.x ),
            .y = static_cast<int>( pos.y ),
            .w = w,
            .h = h
        };
    }


    // +--------------------------------+
    // | WINDOW                         |
    // +--------------------------------+
    struct opaque_window::impl
    {
        SDL_Window* window{ nullptr };


        impl( char const* const title, glm::vec2 const viewport ) noexcept
        {
            window = SDL_CreateWindow(
                title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, viewport.x, viewport.y, SDL_WINDOW_OPENGL );
        }


        ~impl( ) noexcept
        {
            SDL_DestroyWindow( window );
        }


        impl( impl const& )                        = delete;
        impl( impl&& ) noexcept                    = delete;
        auto operator=( impl const& ) -> impl&     = delete;
        auto operator=( impl&& ) noexcept -> impl& = delete;
    };


    opaque_window::opaque_window( std::string_view const& title, glm::vec2 const viewport )
        : impl_ptr_{ std::make_unique<impl>( title.data( ), viewport ) }
        , viewport_{ viewport }
    {
        if ( impl_ptr_->window == nullptr )
        {
            startle( "SDL_CreateWindow error: {}", SDL_GetError( ));
        }
    }


    opaque_window::~opaque_window( ) noexcept = default;

    auto opaque_window::get( ) noexcept -> void* { return impl_ptr_->window; }

    auto opaque_window::get( ) const noexcept -> void const* { return impl_ptr_->window; }

    auto opaque_window::viewport( ) const noexcept -> glm::vec2 { return viewport_; }


    // +--------------------------------+
    // | RENDERER                       |
    // +--------------------------------+
    struct opaque_renderer::impl
    {
        SDL_Renderer* renderer{ nullptr };


        explicit impl( SDL_Window* window ) noexcept
        {
            renderer = SDL_CreateRenderer(
                window, get_open_gl_driver_index( ), SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC );
        }


        ~impl( ) noexcept
        {
            if ( renderer )
            {
                SDL_DestroyRenderer( renderer );
            }
        }
    };


    opaque_renderer::opaque_renderer( opaque_window& window )
        : impl_ptr_{ std::make_unique<impl>( static_cast<SDL_Window*>( window.get( ) ) ) }
    {
        if ( impl_ptr_->renderer == nullptr )
        {
            startle( "SDL_CreateRenderer error: {}", SDL_GetError( ) );
        }
    }


    opaque_renderer::~opaque_renderer( ) noexcept = default;


    auto opaque_renderer::load_texture( earmark const mark, std::filesystem::path const& full_path ) const -> std::unique_ptr<pelt>
    {
        return std::make_unique<sdl_pelt>( *impl_ptr_->renderer, mark, full_path );
    }


    auto opaque_renderer::render_pelt( pelt const& texture, glm::vec4 const& dst ) const noexcept -> void
    {
        SDL_Rect const dst_rect = create_rect( dst );
        SDL_RenderCopy( impl_ptr_->renderer, static_cast<sdl_pelt const&>( texture ).sdl_texture( ), nullptr, &dst_rect );
    }


    auto opaque_renderer::render_pelt( pelt const& texture, glm::vec4 const& dst, glm::vec4 const& src ) const noexcept -> void
    {
        SDL_Rect const src_rect = create_rect( src );
        SDL_Rect const dst_rect = create_rect( dst );
        SDL_RenderCopy( impl_ptr_->renderer, static_cast<sdl_pelt const&>( texture ).sdl_texture( ), &src_rect, &dst_rect );
    }


    auto opaque_renderer::render_pelt_ex( pelt const& texture, glm::vec4 const& dst, glm::vec4 const& src ) const noexcept -> void
    {
        SDL_Rect src_rect       = create_rect( src );
        SDL_Rect const dst_rect = create_rect( dst );
        SDL_RendererFlip flip{ SDL_FLIP_NONE };

        // flip if width or height are negative
        if ( src_rect.w < 0 )
        {
            flip       = static_cast<SDL_RendererFlip>( flip | SDL_FLIP_HORIZONTAL );
            src_rect.w = -src_rect.w;
        }
        if ( src_rect.h < 0 )
        {
            flip       = static_cast<SDL_RendererFlip>( flip | SDL_FLIP_VERTICAL );
            src_rect.h = -src_rect.h;
        }

        SDL_RenderCopyEx(
            impl_ptr_->renderer, static_cast<sdl_pelt const&>( texture ).sdl_texture( ), &src_rect, &dst_rect, 0, nullptr, flip );
    }


    auto opaque_renderer::clear( glm::vec4 const& color ) const noexcept -> void
    {
        SDL_SetRenderDrawColor( impl_ptr_->renderer, color.r, color.g, color.b, color.a );
        SDL_RenderClear( impl_ptr_->renderer );
    }


    auto opaque_renderer::present( ) const noexcept -> void
    {
        SDL_RenderPresent( impl_ptr_->renderer );
    }
}
