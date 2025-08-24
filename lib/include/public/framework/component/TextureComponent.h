#ifndef TEXTURECOMPONENT_H
#define TEXTURECOMPONENT_H

// +--------------------------------+
// | PROJECT HEADERS				|
// +--------------------------------+
#include <framework/component/Component.h>

#include <vec2.hpp>

#include <memory>
#include <string>


namespace engine
{
    class Texture2D;
    class TextureComponent final : public Component
    {
    public:
        explicit TextureComponent( owner_t& owner, const std::string& filename, glm::vec2 offset = {} );

        void render( ) const override;

    private:
        const glm::vec2 offset_{};

        std::shared_ptr<Texture2D> texture_ptr_;

    };
}

#endif //!TEXTURECOMPONENT_H
