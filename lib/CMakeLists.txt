# # "cmake-lists": rhaster-engine, "author": alessandromanzini
# rhaster-engine specific logic here.
#
project( "rhaster-engine" )

add_library(
        ${PROJECT_NAME}
        # include/public/rst/__behaviour/
        "include/public/rst/__behaviour/fsm/state_machine/base_finite_state_machine.h"
        "include/public/rst/__behaviour/fsm/state_machine/finite_multi_state_machine.h"
        "include/public/rst/__behaviour/fsm/state_machine/finite_state_machine.h"
        "include/public/rst/__behaviour/fsm/state_machine/state_stack.h"
        "include/public/rst/__behaviour/fsm/transition/base_transition.h"
        "include/public/rst/__behaviour/fsm/transition/logic/and.h"
        "include/public/rst/__behaviour/fsm/transition/logic/combine.h"
        "include/public/rst/__behaviour/fsm/transition/logic/nand.h"
        "include/public/rst/__behaviour/fsm/transition/logic/nor.h"
        "include/public/rst/__behaviour/fsm/transition/logic/not.h"
        "include/public/rst/__behaviour/fsm/transition/logic/or.h"

        # include/public/rst/__command/
        "include/public/rst/__command/command.h"

        # include/public/rst/__controller/
        "include/public/rst/__input/controller/base_controller.h"
        "include/public/rst/__input/controller/player_controller.h"

        # include/public/rst/__core/
        "include/public/rst/__core/earmark.h"
        "include/public/rst/__core/__ecs/registry_pool.h"
        "include/public/rst/__core/__ecs/ecs_error.h"
        "include/public/rst/__core/__ecs/entity.h"
        "include/public/rst/__core/__ecs/entity_allocator.h"
        "include/public/rst/__core/__ecs/entity_handle.h"
        "include/public/rst/__core/__ecs/registry.h"
        "include/public/rst/__core/__ecs/view.h"
        "include/public/rst/__core/hare.h"
        "include/public/rst/__core/scene.h"
        "include/public/rst/__core/test.h"
        "include/public/rst/__core/component/transform.h"

        # include/public/rst/__event/
        "include/public/rst/data_type/event/event_queue.h"
        "include/public/rst/data_type/event/multicast_delegate.h"
        "include/public/rst/data_type/event/observer.h"
        "include/public/rst/data_type/event/scoped_binding.h"
        "include/public/rst/data_type/event/subject.h"
        "include/public/rst/data_type/event/token.h"

        # include/public/rst/__input/
        "include/public/rst/__input/binding_device.h"
        "include/public/rst/__input/input_buffer.h"
        "include/public/rst/__input/input_mapping_context.h"
        "include/public/rst/__input/input_merge.h"
        "include/public/rst/__input/input_type.h"

        # include/public/rst/__resource_type/
        "include/public/rst/__core/resource/font.h"
        "include/public/rst/__core/resource/audio.h"
        "include/public/rst/__core/resource/pelt_batch.h"

        # include/public/rst/__service/
        "include/public/rst/__core/__service/service_info.h"
        "include/public/rst/__core/__service/sound/parallel_sound_service.h"
        "include/public/rst/__core/__service/sound/sdl_sound_service.h"
        "include/public/rst/__core/__service/sound_service.h"
        "include/public/rst/__core/__service/sound/sound_logger_service.h"

        # include/public/rst/data_type/
        "include/public/rst/data_type/blackboard.h"
        "include/public/rst/data_type/data_structure_error.h"
        "include/public/rst/data_type/deleter.h"
        "include/public/rst/data_type/optional_ref.h"
        "include/public/rst/data_type/ref_proxy.h"
        "include/public/rst/data_type/safe_resource.h"
        "include/public/rst/data_type/sparse_set.h"
        "include/public/rst/data_type/token_generator.h"
        "include/public/rst/data_type/unique_ref.h"

        # include/public/rst/meta/
        "include/public/rst/meta/__type_traits/concepts.h"
        "include/public/rst/meta/__type_traits/constant_traits.h"
        "include/public/rst/meta/__type_traits/function_traits.h"
        "include/public/rst/meta/__type_traits/parameter_pack_traits.h"
        "include/public/rst/meta/__type_traits/tuple_traits.h"
        "include/public/rst/meta/address.h"
        "include/public/rst/meta/algorithm.h"
        "include/public/rst/meta/hash.h"
        "include/public/rst/meta/type_traits.h"

        # include/public/rst/temp/
        "include/public/rst/temp/framework/component/audio_component.h"
        "include/public/rst/temp/framework/component/physics/box_collider_component.h"
        "include/public/rst/temp/framework/component/physics/collider_component.h"
        "include/public/rst/temp/framework/component/physics/physics_component.h"
        "include/public/rst/temp/framework/component/text_component.h"
        "include/public/rst/temp/framework/component/texture_component.h"
        "include/public/rst/temp/singleton/Singleton.h"
        "include/public/rst/temp/singleton/game_instance.h"
        "include/public/rst/temp/singleton/game_time.h"
        "include/public/rst/temp/singleton/input_system.h"
        "include/public/rst/temp/singleton/resource_manager.h"
        "include/public/rst/temp/singleton/scene_pool.h"
        "include/public/rst/__core/__service/service_locator.h"

        # include/public/rst/ (root)
        "include/public/rst/fsm.h"
        "include/public/rst/pch.h"

        # src/
        "src/audio.cpp"
        "src/audio_component.cpp"
        "src/binding_device.cpp"
        "src/box_collider_component.cpp"
        "src/collider_component.cpp"
        "src/earmark.cpp"
        "src/entity_allocator.cpp"
        "src/finite_multi_state_machine.cpp"
        "src/finite_state_machine.cpp"
        "src/font.cpp"
        "src/game_instance.cpp"
        "src/game_time.cpp"
        "src/hare.cpp"
        "src/input_buffer.cpp"
        "src/input_mapping_context.cpp"
        "src/input_merge.cpp"
        "src/input_system.cpp"
        "src/parallel_sound_system.cpp"
        "src/physics_component.cpp"
        "src/player_controller.cpp"
        "src/sdl_erasure.cpp"
        "src/sdl_renderer_service.cpp"
        "src/resource_manager.cpp"
        "src/scene.cpp"
        "src/scene_pool.cpp"
        "src/sdl_audio.cpp"
        "src/sdl_sound_system.cpp"
        "src/service_locator.cpp"
        "src/sound_system_logger.cpp"
        "src/pelt_batch.cpp"
        "src/state_stack.cpp"
        "src/subject.cpp"
        "src/text_component.cpp"
        "src/texture_component.cpp"
        "src/transform.cpp"
        include/public/rst/__core/__system/base_system.h
        include/public/rst/__core/__system/system_scheduler.h
        include/public/rst/__core/__system/system_timing.h
        include/public/rst/meta/__type_traits/enum_traits.h
        include/public/rst/__core/__service/renderer_service.h
        include/public/rst/__core/resource/pelt.h
        include/public/rst/__core/__service/render/sdl_renderer_service.h
        include/public/rst/__core/resource/type_erasure/sdl_erasure.h
        include/private/rst/__internal/resource/sdl_pelt.h
        src/sdl_pelt.cpp
        include/public/rst/__core/__service/render/unified_render_dispatch.h
        include/public/rst/__core/__system/renderer_system.h
        src/renderer_system.cpp
        include/public/rst/__core/component/pelt_frame.h
        include/public/rst/__core/__service/renderer_service.h
        include/public/rst/__core/__ecs/component_constraints.h
        src/transform.cpp
        include/public/rst/core.h
        include/public/rst/__core/ecs.h
        include/public/rst/__core/service.h
        include/public/rst/__core/system.h
)

# include directories
target_include_directories(
        ${PROJECT_NAME}
        PUBLIC "include/public"
        PRIVATE "include/private"
        PRIVATE "src" )

# set maximum warning level and treat warnings as errors
include( set_w4wx_macro )
set_w4wx()

# fetch external libraries
include( fetch_glm_macro )
fetch_glm()

include( fetch_sdl2_macro )
fetch_sdl2()

# include vld
find_package( VLD CONFIG )
target_link_libraries( ${PROJECT_NAME} PUBLIC ${VLD_LIBRARY} )
