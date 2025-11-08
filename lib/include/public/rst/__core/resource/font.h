#ifndef FONT_H
#define FONT_H

#include <rst/pch.h>


struct _TTF_Font; // NOLINT(*-reserved-identifier)


// TODO: pimpl sdl and add documentation
namespace rst
{
    /**
     * Simple RAII wrapper for a _TTF_Font
     */
    class font final
    {
    public:
        explicit font( std::string const& full_path, unsigned int size );
        ~font( ) noexcept;

        font( font const& )                     = delete;
        font( font&& )                          = delete;
        auto operator=( font const& ) -> font&  = delete;
        auto operator=( font const&& ) -> font& = delete;

        [[nodiscard]] auto handle( ) const -> _TTF_Font*;

    private:
        _TTF_Font* font_ptr_;
    };
}


#endif //!FONT_H
