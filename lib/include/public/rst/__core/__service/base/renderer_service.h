#ifndef RST_RENDERER_SERVICE_H
#define RST_RENDERER_SERVICE_H

#include <rst/pch.h>


namespace rst
{
    class pelt;

    namespace system
    {
        class renderer_system;
    }
}

namespace rst
{
    class renderer_service
    {
        friend class system::renderer_system;

    public:
        renderer_service( )                   = default;
        virtual ~renderer_service( ) noexcept = default;

        renderer_service( renderer_service const& )                        = delete;
        renderer_service( renderer_service&& ) noexcept                    = delete;
        auto operator=( renderer_service const& ) -> renderer_service&     = delete;
        auto operator=( renderer_service&& ) noexcept -> renderer_service& = delete;

        virtual auto z_order( int z_index ) noexcept -> void = 0;

        virtual auto render( pelt const& texture, glm::vec2 pos, glm::vec4 const& src = {} ) noexcept -> void = 0;
        virtual auto render( pelt const& texture, glm::vec4 const& dst, glm::vec4 const& src = {} ) noexcept -> void = 0;

        [[nodiscard]] virtual auto clear_color( ) const noexcept -> glm::vec4 const& = 0;
        virtual auto set_clear_color( glm::vec4 const& color ) noexcept -> void = 0;

        // todo: make private
        [[nodiscard]] virtual auto make_pelt( earmark mark, std::filesystem::path const& file_path ) -> std::unique_ptr<pelt> = 0;
    private:
        virtual auto render_dispatch( ) noexcept -> void = 0;
    };
}


#endif //!RST_RENDERER_SERVICE_H
