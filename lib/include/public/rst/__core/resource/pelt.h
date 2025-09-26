#ifndef RST_PELT_H
#define RST_PELT_H

#include <rst/pch.h>


namespace rst
{
    class pelt
    {
    public:
        pelt( earmark const mark ) : mark_{ mark } { }
        virtual ~pelt( ) noexcept = default;

        pelt( pelt const& )                        = delete;
        pelt( pelt&& ) noexcept                    = delete;
        auto operator=( pelt const& ) -> pelt&     = delete;
        auto operator=( pelt&& ) noexcept -> pelt& = delete;

        [[nodiscard]] auto operator==( pelt const& other ) const noexcept -> bool { return mark_ == other.mark_; }
        [[nodiscard]] auto operator!=( pelt const& other ) const noexcept -> bool { return not operator==( other ); }

        [[nodiscard]] virtual auto dimensions( ) const noexcept -> glm::vec2 = 0;
        [[nodiscard]] auto whole_src_rect( ) const noexcept -> glm::vec4 { return { 0.f, 0.f, dimensions( ) }; }
        [[nodiscard]] auto mark( ) const noexcept -> earmark { return mark_; }

    private:
        earmark const mark_;
    };
}


#endif //!RST_PELT_H
