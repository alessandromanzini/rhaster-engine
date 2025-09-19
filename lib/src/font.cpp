#include <rst/__core/resource/font.h>

#include <SDL_ttf.h>


namespace rst
{
    font::font( std::string const& full_path, unsigned int const size )
        : font_ptr_{ nullptr }
    {
        font_ptr_ = TTF_OpenFont( full_path.c_str( ), size );
        if ( font_ptr_ == nullptr )
        {
            throw std::runtime_error( std::string( "Failed to load font: " ) + SDL_GetError( ) );
        }
    }


    font::~font( ) noexcept
    {
        TTF_CloseFont( font_ptr_ );
    }


    auto font::handle( ) const -> _TTF_Font*
    {
        return font_ptr_;
    }

}
