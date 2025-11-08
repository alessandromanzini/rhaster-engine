#ifndef RST_SERVICE_SDL_RENDERER_H
#define RST_SERVICE_SDL_RENDERER_H

#include <rst/pch.h>

#include <rst/__core/resource/type_erasure/sdl_erasure.h>
#include <rst/__core/__service/base/renderer_service.h>


namespace rst
{
    class pelt;
}

namespace rst::service
{
    namespace internal
    {
        struct request
        {
            pelt const* texture{ nullptr };
            glm::vec4 dst{};
            glm::vec4 src{};
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


    class sdl_renderer_service final : public renderer_service
    {
    public:
        sdl_renderer_service( std::string_view const& window_title, glm::vec2 viewport );

        auto z_order( int z_index ) noexcept -> void override;

        auto render( pelt const& texture, glm::vec2 pos, glm::vec4 const& src ) noexcept -> void override;
        auto render( pelt const& texture, glm::vec4 const& dst, glm::vec4 const& src ) noexcept -> void override;

        [[nodiscard]] auto clear_color( ) const noexcept -> glm::vec4 const& override;
        auto set_clear_color( glm::vec4 const& color ) noexcept -> void override;

    private:
        rst::internal::sdl::opaque_window window_;
        rst::internal::sdl::opaque_renderer renderer_;

        glm::vec4 clear_color_{};

        std::atomic<int> z_index_{ 0 };
        std::multiset<internal::request, internal::request_comparator> render_queue_{};

        [[nodiscard]] auto make_pelt( earmark mark, std::filesystem::path const& file_path ) -> std::unique_ptr<pelt> override;
        auto render_dispatch( ) noexcept -> void override;
    };
}


#endif //!RST_SERVICE_SDL_RENDERER_H
