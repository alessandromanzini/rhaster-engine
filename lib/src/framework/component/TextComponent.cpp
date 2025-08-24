#include <framework/component/TextComponent.h>

// +--------------------------------+
// | PROJECT HEADERS				|
// +--------------------------------+
#include <framework/GameObject.h>
#include <singleton/Renderer.h>

#include <framework/resource/Font.h>
#include <framework/resource/texture/Texture2D.h>

// +--------------------------------+
// | STANDARD HEADERS				|
// +--------------------------------+
#include <SDL_ttf.h>
#include <stdexcept>


namespace engine
{
    TextComponent::TextComponent( owner_t& owner, const std::string& text, std::shared_ptr<Font> font, const bool centered )
        : Component( owner )
        , centered_{ centered }
        , font_ptr_{ std::move( font ) }
    {
        set_text( text );

        if ( font_ptr_ == nullptr )
        {
            throw std::runtime_error( "Font cannot be nullptr!" );
        }
    }


    void TextComponent::tick( )
    {
        commit( );
    }


    void TextComponent::render( ) const
    {
        if ( text_texture_ptr_ != nullptr )
        {
            RENDERER.set_z_index( 10 );

            const auto offset{ centered_ ? -text_texture_ptr_->get_size(  ) / 2.f : glm::vec2{} };
            RENDERER.render_texture( *text_texture_ptr_,  get_owner( ).get_world_transform( ).get_position( ) + offset );

            RENDERER.set_z_index( 0 );
        }
    }


    void TextComponent::set_text( const std::string& text )
    {
        if ( !text.empty( ) )
        {
            text_         = text;
            needs_update_ = true;
        }
    }


    void TextComponent::commit( )
    {
        if ( needs_update_ )
        {
            constexpr SDL_Color color = { 255, 255, 255, 255 }; // only white text is supported now
            const auto surf           = TTF_RenderText_Blended( font_ptr_->get_font( ), text_.c_str( ), color );
            if ( surf == nullptr )
            {
                throw std::runtime_error( std::string( "Render text failed: " ) + SDL_GetError( ) );
            }

            auto texture = SDL_CreateTextureFromSurface( RENDERER.get_sdl_renderer( ), surf );
            if ( texture == nullptr )
            {
                throw std::runtime_error(
                    std::string( "Create text texture from surface failed: " ) + SDL_GetError( ) );
            }

            SDL_FreeSurface( surf );
            text_texture_ptr_ = std::make_shared<Texture2D>( texture );
            needs_update_     = false;
        }
    }


    glm::vec2 TextComponent::get_text_size( ) const
    {
        return text_texture_ptr_->get_size( );
    }

}
