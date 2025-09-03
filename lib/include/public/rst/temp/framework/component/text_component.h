#ifndef RST_TEXT_COMPONENT_H
#define RST_TEXT_COMPONENT_H

#include <rst/pch.h>

#include <rst/__core/component.h>


namespace rst
{
    class font;
    class texture_2d;
}

namespace rst
{
    // TODO: remove shared ptr
    class text_component final : public component
    {
    public:
        explicit text_component( owner_type& owner, std::string const& text, std::shared_ptr<font> font, bool centered = false );

        auto tick( ) -> void override;
        auto render( ) const -> void override;

        auto set_text( std::string const& text ) -> void;
        auto commit( ) -> void;
        [[nodiscard]] auto text_size( ) const -> glm::vec2;

    private:
        bool const centered_{ false };

        bool needs_update_{};
        std::string text_;

        std::shared_ptr<font> font_ptr_{ nullptr };
        std::shared_ptr<texture_2d> text_texture_ptr_{ nullptr };
    };
}


#endif //!RST_TEXT_COMPONENT_H
