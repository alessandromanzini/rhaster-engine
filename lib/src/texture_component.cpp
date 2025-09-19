// #include <rst/temp/framework/component/texture_component.h>
//
// #include <rst/temp/singleton/renderer_service.h>
// #include <rst/temp/singleton/resource_manager.h>
// #include <rst/__core/gameobject.h>
//
//
// namespace rst
// {
//     texture_component::texture_component( owner_type& owner, std::string const& filename, glm::vec2 const offset )
//         : component{ owner }
//         , offset_{ offset }
//         , texture_ptr_{ RESOURCE_MANAGER.load_texture( filename ) }
//     {
//     }
//
//
//     auto texture_component::render( ) const -> void
//     {
//         auto const pos = owner( ).world_transform( ).position( ) + offset_;
//         RENDERER.render_texture( *texture_ptr_, pos );
//     }
//
// }
