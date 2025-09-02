#include <rst/temp/framework/component/texture_component.h>

#include <rst/temp/singleton/renderer.h>
#include <rst/temp/singleton/resource_manager.h>
#include <rst/__core/gameobject.h>


namespace rst
{
    TextureComponent::TextureComponent( owner_t& owner, std::string const& filename, glm::vec2 const offset )
        : Component{ owner }
        , offset_{ offset }
        , texture_ptr_{ RESOURCE_MANAGER.load_texture( filename ) }
    {
    }


    auto TextureComponent::render( ) const -> void
    {
        auto const pos = get_owner( ).get_world_transform( ).get_position( ) + offset_;
        RENDERER.render_texture( *texture_ptr_, pos );
    }

}
