#ifndef RST_TEXT_COMPONENT_H
#define RST_TEXT_COMPONENT_H

#include <rst/pch.h>

#include <rst/__core/component.h>


namespace rst
{
    class Font;
    class Texture2D;
}

namespace rst
{
    // TODO: remove shared ptr
    class TextComponent final : public Component
    {
    public:
        explicit TextComponent( owner_t& owner, std::string const& text, std::shared_ptr<Font> font, bool centered = false );

        auto tick( ) -> void override;
        auto render( ) const -> void override;

        auto set_text( std::string const& text ) -> void;
        auto commit( ) -> void;
        [[nodiscard]] auto get_text_size( ) const -> glm::vec2;

    private:
        bool const centered_{ false };

        bool needs_update_{};
        std::string text_;

        std::shared_ptr<Font> font_ptr_{ nullptr };
        std::shared_ptr<Texture2D> text_texture_ptr_{ nullptr };
    };
}


#endif //!RST_TEXT_COMPONENT_H
