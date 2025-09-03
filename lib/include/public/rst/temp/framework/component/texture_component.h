#ifndef RST_TEXTURE_COMPONENT_H
#define RST_TEXTURE_COMPONENT_H

#include <rst/pch.h>

#include <rst/__core/component.h>


// TODO: remove shared_ptr
namespace rst
{
    class texture_2d;
}

namespace rst
{
    class texture_component final : public component
    {
    public:
        explicit texture_component( owner_type& owner, std::string const& filename, glm::vec2 offset = {} );
        auto render( ) const -> void override;

    private:
        glm::vec2 const offset_{};
        std::shared_ptr<texture_2d> texture_ptr_;
    };
}


#endif //!RST_TEXTURE_COMPONENT_H
