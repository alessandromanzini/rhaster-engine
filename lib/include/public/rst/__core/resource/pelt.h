#ifndef RST_BASE_TEXTURE_H
#define RST_BASE_TEXTURE_H

#include <rst/pch.h>


namespace rst
{
    class pelt
    {
    public:
        pelt( )                   = default;
        virtual ~pelt( ) noexcept = default;

        pelt( pelt const& )                        = delete;
        pelt( pelt&& ) noexcept                    = delete;
        auto operator=( pelt const& ) -> pelt&     = delete;
        auto operator=( pelt&& ) noexcept -> pelt& = delete;

        [[nodiscard]] virtual auto dimensions( ) const noexcept -> glm::vec2 = 0;
        [[nodiscard]] auto whole_src_rect( ) const noexcept -> glm::vec4 { return { 0.f, 0.f, dimensions( ) }; }
    };
}


#endif //!RST_BASE_TEXTURE_H
