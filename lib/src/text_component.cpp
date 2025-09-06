#include <rst/temp/framework/component/text_component.h>

#include <rst/temp/singleton/renderer.h>
#include <rst/__core/gameobject.h>

#include <SDL_ttf.h>
#include <rst/__resource_type/font/font.h>
#include <rst/__resource_type/texture/texture2D.h>


namespace rst
{
    text_component::text_component( owner_type& owner, std::string const& text, std::shared_ptr<font> font, bool const centered )
        : component{ owner }
        , centered_{ centered }
        , font_ptr_{ std::move( font ) }
    {
        set_text( text );

        if ( font_ptr_ == nullptr )
        {
            throw std::runtime_error( "Font cannot be nullptr!" );
        }
    }


    auto text_component::tick( ) -> void
    {
        commit( );
    }


    auto text_component::render( ) const -> void
    {
        if ( text_texture_ptr_ != nullptr )
        {
            RENDERER.set_z_index( 10 );

            auto const offset{ centered_ ? -text_texture_ptr_->size( ) / 2.f : glm::vec2{} };
            RENDERER.render_texture( *text_texture_ptr_, owner( ).world_transform( ).position( ) + offset );

            RENDERER.set_z_index( 0 );
        }
    }


    auto text_component::set_text( std::string const& text ) -> void
    {
        if ( !text.empty( ) )
        {
            text_         = text;
            needs_update_ = true;
        }
    }


    auto text_component::commit( ) -> void
    {
        if ( needs_update_ )
        {
            constexpr SDL_Color color = { 255, 255, 255, 255 }; // only white text is supported now
            auto const surf           = TTF_RenderText_Blended( font_ptr_->handle( ), text_.c_str( ), color );
            if ( surf == nullptr )
            {
                throw std::runtime_error( std::string( "Render text failed: " ) + SDL_GetError( ) );
            }

            auto texture = SDL_CreateTextureFromSurface( RENDERER.sdl_renderer( ), surf );
            if ( texture == nullptr )
            {
                throw std::runtime_error(
                    std::string( "Create text texture from surface failed: " ) + SDL_GetError( ) );
            }

            SDL_FreeSurface( surf );
            text_texture_ptr_ = std::make_shared<texture_2d>( texture );
            needs_update_     = false;
        }
    }


    auto text_component::text_size( ) const -> glm::vec2
    {
        return text_texture_ptr_->size( );
    }
}
