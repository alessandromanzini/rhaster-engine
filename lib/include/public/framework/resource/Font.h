#ifndef FONT_H
#define FONT_H

// +--------------------------------+
// | STANDARD HEADERS				|
// +--------------------------------+
#include <string>

struct _TTF_Font; // NOLINT(*-reserved-identifier)


namespace engine
{
    /**
     * Simple RAII wrapper for a _TTF_Font
     */
    class Font final
    {
    public:
        explicit Font( const std::string& fullPath, unsigned int size );
        ~Font( ) noexcept;

        Font( const Font& )             = delete;
        Font( Font&& )                  = delete;
        Font& operator=( const Font& )  = delete;
        Font& operator=( const Font&& ) = delete;

        [[nodiscard]] _TTF_Font* get_font( ) const;

    private:
        _TTF_Font* font_ptr_;

    };
}

#endif //!FONT_H
