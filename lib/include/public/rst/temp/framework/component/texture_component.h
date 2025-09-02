#ifndef RST_TEXTURE_COMPONENT_H
#define RST_TEXTURE_COMPONENT_H

#include <rst/pch.h>

#include <rst/__core/component.h>


// TODO: remove shared_ptr
namespace rst
{
    class Texture2D;
    class TextureComponent final : public Component
    {
    public:
        explicit TextureComponent( owner_t& owner, std::string const& filename, glm::vec2 offset = {} );
        auto render( ) const -> void override;

    private:
        glm::vec2 const offset_{};
        std::shared_ptr<Texture2D> texture_ptr_;
    };
}


#endif //!RST_TEXTURE_COMPONENT_H
