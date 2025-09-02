#include <../include/public/rst/__type/font_type/font.h>

#include <SDL_ttf.h>


namespace rst
{
    Font::Font( std::string const& full_path, unsigned int const size )
        : font_ptr_{ nullptr }
    {
        font_ptr_ = TTF_OpenFont( full_path.c_str( ), size );
        if ( font_ptr_ == nullptr )
        {
            throw std::runtime_error( std::string( "Failed to load font: " ) + SDL_GetError( ) );
        }
    }


    Font::~Font( ) noexcept
    {
        TTF_CloseFont( font_ptr_ );
    }


    auto Font::get_font( ) const -> _TTF_Font*
    {
        return font_ptr_;
    }

}
