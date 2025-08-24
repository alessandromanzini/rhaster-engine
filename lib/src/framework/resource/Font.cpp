#include <framework/resource/Font.h>

// +--------------------------------+
// | SDL HEADERS					|
// +--------------------------------+
#include <SDL_ttf.h>

// +--------------------------------+
// | STANDARD HEADERS				|
// +--------------------------------+
#include <stdexcept>


namespace engine
{
    Font::Font( const std::string& fullPath, const unsigned int size )
        : font_ptr_{ nullptr }
    {
        font_ptr_ = TTF_OpenFont( fullPath.c_str( ), size );
        if ( font_ptr_ == nullptr )
        {
            throw std::runtime_error( std::string( "Failed to load font: " ) + SDL_GetError( ) );
        }
    }


    Font::~Font( ) noexcept
    {
        TTF_CloseFont( font_ptr_ );
    }


    _TTF_Font* Font::get_font( ) const
    {
        return font_ptr_;
    }

}
