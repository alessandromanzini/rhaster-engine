#include <rst/__core/__service/render/sdl_renderer_service.h>

#include <rst/__internal/resource/sdl_pelt.h>


namespace rst::service
{
    // +--------------------------------+
    // | HELPERS                        |
    // +--------------------------------+
    [[nodiscard]] auto fill_rect_if_empty( pelt const& texture, glm::vec4 const& rect ) noexcept -> glm::vec4
    {
        return rect.z == 0.f && rect.w == 0.f ? texture.whole_src_rect( ) : rect;
    }


    // +--------------------------------+
    // | RENDERER                       |
    // +--------------------------------+
    sdl_renderer_service::sdl_renderer_service( std::string_view const& window_title, glm::vec2 const viewport )
        : window_{ window_title, viewport }
        , renderer_{ window_ } { }


    auto sdl_renderer_service::z_order( int const z_index ) noexcept -> void
    {
        z_index_.store( z_index );
    }


    auto sdl_renderer_service::render( pelt const& texture, glm::vec2 const pos, glm::vec4 const& src ) noexcept -> void
    {
        render_queue_.insert(
            internal::request{
                .texture = &texture,
                .dst = glm::vec4{ pos, texture.dimensions( ) },
                .src = fill_rect_if_empty( texture, src ),
                .z_index = z_index_
            } );
    }


    auto sdl_renderer_service::render( pelt const& texture, glm::vec4 const& dst, glm::vec4 const& src ) noexcept -> void
    {
        render_queue_.insert(
            internal::request{
                .texture = &texture,
                .dst = dst,
                .src = fill_rect_if_empty( texture, src ),
                .z_index = z_index_
            } );
    }


    auto sdl_renderer_service::clear_color( ) const noexcept -> glm::vec4 const&
    {
        return clear_color_;
    }


    auto sdl_renderer_service::set_clear_color( glm::vec4 const& color ) noexcept -> void
    {
        clear_color_ = color;
    }


    auto sdl_renderer_service::make_pelt( earmark const mark, std::filesystem::path const& file_path ) -> std::unique_ptr<pelt>
    {
        return renderer_.load_texture( mark, file_path );
    }


    auto sdl_renderer_service::render_dispatch( ) noexcept -> void
    {
        // 1. clear the screen
        renderer_.clear( clear_color_ );

        // 2. render all queued requests
        for ( auto const& request : render_queue_ )
        {
            renderer_.render_pelt_ex( *request.texture, request.dst, request.src );
        }
        render_queue_.clear( );

        // 3. present the back-buffer
        renderer_.present( );
    }
}
