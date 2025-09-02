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
    class Font final
    {
    public:
        explicit Font( std::string const& full_path, unsigned int size );
        ~Font( ) noexcept;

        Font( Font const& )                     = delete;
        Font( Font&& )                          = delete;
        auto operator=( Font const& ) -> Font&  = delete;
        auto operator=( Font const&& ) -> Font& = delete;

        [[nodiscard]] auto get_font( ) const -> _TTF_Font*;

    private:
        _TTF_Font* font_ptr_;
    };
}


#endif //!FONT_H
