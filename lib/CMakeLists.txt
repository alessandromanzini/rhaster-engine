# # "cmake-lists": rhaster-engine, "author": alessandromanzini
# rhaster-engine specific logic here.
#
project( "rhaster-engine" )


# ========================================
# SOURCE FILES
# ========================================

# --- core ecs system ---
set( ECS_HEADERS
     "include/public/rst/__core/__ecs/component_constraints.h"
     "include/public/rst/__core/__ecs/ecs_error.h"
     "include/public/rst/__core/__ecs/entity.h"
     "include/public/rst/__core/__ecs/entity_allocator.h"
     "include/public/rst/__core/__ecs/entity_handle.h"
     "include/public/rst/__core/__ecs/registry.h"
     "include/public/rst/__core/__ecs/registry_pool.h"
     "include/public/rst/__core/__ecs/view.h"
)

set( ECS_SOURCES
     "src/entity_allocator.cpp"
)

# --- core system ---
set( SYSTEM_HEADERS
     "include/public/rst/__core/__system/base_system.h"
     "include/public/rst/__core/__system/renderer_system.h"
     "include/public/rst/__core/__system/system_scheduler.h"
     "include/public/rst/__core/__system/system_timing.h"
)

set( SYSTEM_SOURCES
     "src/renderer_system.cpp"
)

# --- core services ---
set( SERVICE_HEADERS
     "include/public/rst/__core/__service/renderer_service.h"
     "include/public/rst/__core/__service/service_info.h"
     "include/public/rst/__core/__service/service_locator.h"
     "include/public/rst/__core/__service/sound_service.h"

     # render service
     "include/public/rst/__core/__service/render/sdl_renderer_service.h"

     # sound service
     "include/public/rst/__core/__service/sound/parallel_sound_service.h"
     "include/public/rst/__core/__service/sound/sdl_sound_service.h"
     "include/public/rst/__core/__service/sound/sound_logger_service.h"
)

set( SERVICE_SOURCES
     "src/parallel_sound_system.cpp"
     "src/sdl_renderer_service.cpp"
     "src/sdl_sound_system.cpp"
     "src/service_locator.cpp"
     "src/sound_system_logger.cpp"
)

# --- core component ---
set( COMPONENT_HEADERS
     "include/public/rst/__core/component/pelt_frame.h"
     "include/public/rst/__core/component/transform.h"
)

set( COMPONENT_SOURCES
     "src/transform.cpp"
)

# --- core resource ---
set( RESOURCE_HEADERS
     "include/public/rst/__core/resource/audio.h"
     "include/public/rst/__core/resource/font.h"
     "include/public/rst/__core/resource/pelt.h"
     "include/public/rst/__core/resource/pelt_batch.h"
     "include/public/rst/__core/resource/type_erasure/sdl_erasure.h"
)

set( RESOURCE_SOURCES
     "src/audio.cpp"
     "src/font.cpp"
     "src/pelt_batch.cpp"
     "src/sdl_erasure.cpp"
)

# --- core module ---
set( CORE_MODULE_HEADERS
     "include/public/rst/__core/ecs.h"
     "include/public/rst/__core/earmark.h"
     "include/public/rst/__core/hare.h"
     "include/public/rst/__core/scene.h"
     "include/public/rst/__core/service.h"
     "include/public/rst/__core/system.h"
     "include/public/rst/__core/test.h"
     "include/public/rst/core.h"
)

set( CORE_MODULE_SOURCES
     "src/earmark.cpp"
     "src/hare.cpp"
     "src/scene.cpp"
)

# --- behavior ---
set( BEHAVIOR_HEADERS
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
)

set( BEHAVIOR_SOURCES
     "src/finite_multi_state_machine.cpp"
     "src/finite_state_machine.cpp"
     "src/state_stack.cpp"
)

# --- input ---
set( INPUT_HEADERS
     "include/public/rst/__input/binding_device.h"
     "include/public/rst/__input/controller/base_controller.h"
     "include/public/rst/__input/controller/player_controller.h"
     "include/public/rst/__input/input_buffer.h"
     "include/public/rst/__input/input_mapping_context.h"
     "include/public/rst/__input/input_merge.h"
     "include/public/rst/__input/input_type.h"
)

set( INPUT_SOURCES
     "src/binding_device.cpp"
     "src/input_buffer.cpp"
     "src/input_mapping_context.cpp"
     "src/input_merge.cpp"
     "src/player_controller.cpp"
)

# --- event ---
set( EVENT_HEADERS
     "include/public/rst/data_type/event/event_queue.h"
     "include/public/rst/data_type/event/multicast_delegate.h"
     "include/public/rst/data_type/event/observer.h"
     "include/public/rst/data_type/event/scoped_binding.h"
     "include/public/rst/data_type/event/subject.h"
     "include/public/rst/data_type/event/token.h"
)

set( EVENT_SOURCES
     "src/subject.cpp"
)

# --- data structure ---
set( DATA_STRUCTURE_HEADERS
     "include/public/rst/data_type/blackboard.h"
     "include/public/rst/data_type/data_structure_error.h"
     "include/public/rst/data_type/deleter.h"
     "include/public/rst/data_type/optional_ref.h"
     "include/public/rst/data_type/ref_proxy.h"
     "include/public/rst/data_type/safe_resource.h"
     "include/public/rst/data_type/sparse_set.h"
     "include/public/rst/data_type/token_generator.h"
     "include/public/rst/data_type/unique_ref.h"
)

# --- meta ---
set( META_HEADERS
     "include/public/rst/meta/__type_traits/concepts.h"
     "include/public/rst/meta/__type_traits/constant_traits.h"
     "include/public/rst/meta/__type_traits/enum_traits.h"
     "include/public/rst/meta/__type_traits/function_traits.h"
     "include/public/rst/meta/__type_traits/parameter_pack_traits.h"
     "include/public/rst/meta/__type_traits/tuple_traits.h"
     "include/public/rst/meta/address.h"
     "include/public/rst/meta/algorithm.h"
     "include/public/rst/meta/hash.h"
     "include/public/rst/meta/type_traits.h"
)

# --- command ---
set( COMMAND_HEADERS
     "include/public/rst/__command/command.h"
)

# --- legacy (being phased out) ---
set( TEMP_HEADERS
     # Framework components
     "include/public/rst/temp/framework/component/audio_component.h"
     "include/public/rst/temp/framework/component/physics/box_collider_component.h"
     "include/public/rst/temp/framework/component/physics/collider_component.h"
     "include/public/rst/temp/framework/component/physics/physics_component.h"
     "include/public/rst/temp/framework/component/text_component.h"
     "include/public/rst/temp/framework/component/texture_component.h"

     # Singletons
     "include/public/rst/temp/singleton/Singleton.h"
     "include/public/rst/temp/singleton/game_instance.h"
     "include/public/rst/temp/singleton/game_time.h"
     "include/public/rst/temp/singleton/input_system.h"
     "include/public/rst/temp/singleton/resource_manager.h"
     "include/public/rst/temp/singleton/scene_pool.h"
)

set( TEMP_SOURCES
     "src/audio_component.cpp"
     "src/box_collider_component.cpp"
     "src/collider_component.cpp"
     "src/game_instance.cpp"
     "src/game_time.cpp"
     "src/input_system.cpp"
     "src/physics_component.cpp"
     "src/resource_manager.cpp"
     "src/scene_pool.cpp"
     "src/text_component.cpp"
     "src/texture_component.cpp"
)

# --- internal/private ---
set( PRIVATE_HEADERS
     "include/private/rst/__internal/resource/sdl_audio.h"
     "include/private/rst/__internal/resource/sdl_pelt.h"
)

set( PRIVATE_SOURCES
     "src/sdl_audio.cpp"
     "src/sdl_pelt.cpp"
)

# --- root ---
set( ROOT_HEADERS
     "include/public/rst/fsm.h"
     "include/public/rst/pch.h"
)


# ========================================
# LIBRARY TARGET
# ========================================
add_library( ${PROJECT_NAME}
             # core ecs
             ${ECS_HEADERS}
             ${ECS_SOURCES}

             # core system
             ${SYSTEM_HEADERS}
             ${SYSTEM_SOURCES}

             # core services
             ${SERVICE_HEADERS}
             ${SERVICE_SOURCES}

             # core component
             ${COMPONENT_HEADERS}
             ${COMPONENT_SOURCES}

             # core resource
             ${RESOURCE_HEADERS}
             ${RESOURCE_SOURCES}

             # core module
             ${CORE_MODULE_HEADERS}
             ${CORE_MODULE_SOURCES}

             # behavior
             ${BEHAVIOR_HEADERS}
             ${BEHAVIOR_SOURCES}

             # input
             ${INPUT_HEADERS}
             ${INPUT_SOURCES}

             # events
             ${EVENT_HEADERS}
             ${EVENT_SOURCES}

             # data structure
             ${DATA_STRUCTURE_HEADERS}

             # meta
             ${META_HEADERS}

             # command
             ${COMMAND_HEADERS}

             # legacy (being phased out)
             ${TEMP_HEADERS}
             ${TEMP_SOURCES}

             # private/internal
             ${PRIVATE_HEADERS}
             ${PRIVATE_SOURCES}

             # root
             ${ROOT_HEADERS}
)


# ========================================
# TARGET CONFIGURATION
# ========================================

# include directories
target_include_directories( ${PROJECT_NAME}
                            PUBLIC
                            $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/public>
                            $<INSTALL_INTERFACE:include>
                            PRIVATE
                            ${CMAKE_CURRENT_SOURCE_DIR}/include/private
                            ${CMAKE_CURRENT_SOURCE_DIR}/src )

# set maximum warning level and treat warnings as errors
include( set_w4wx_macro )
set_w4wx()


# ========================================
# EXTERNAL DEPENDENCIES
# ========================================

# GLM (math library)
include( fetch_glm_macro )
fetch_glm()

# SDL2 (graphics/input)
include( fetch_sdl2_macro )
fetch_sdl2()

# ========================================
# SOURCE GROUPS (for IDEs)
# ========================================

source_group( "Header Files/Core/ECS" FILES ${ECS_HEADERS} )
source_group( "Header Files/Core/Systems" FILES ${SYSTEM_HEADERS} )
source_group( "Header Files/Core/Services" FILES ${SERVICE_HEADERS} )
source_group( "Header Files/Core/Components" FILES ${COMPONENT_HEADERS} )
source_group( "Header Files/Core/Resources" FILES ${RESOURCE_HEADERS} )
source_group( "Header Files/Core" FILES ${CORE_MODULE_HEADERS} )
source_group( "Header Files/Behavior" FILES ${BEHAVIOR_HEADERS} )
source_group( "Header Files/Input" FILES ${INPUT_HEADERS} )
source_group( "Header Files/Events" FILES ${EVENT_HEADERS} )
source_group( "Header Files/Data Structures" FILES ${DATA_STRUCTURE_HEADERS} )
source_group( "Header Files/Meta" FILES ${META_HEADERS} )
source_group( "Header Files/Commands" FILES ${COMMAND_HEADERS} )
source_group( "Header Files/Temp (Legacy)" FILES ${TEMP_HEADERS} )
source_group( "Header Files/Private" FILES ${PRIVATE_HEADERS} )
source_group( "Header Files" FILES ${ROOT_HEADERS} )

source_group( "Source Files/Core/ECS" FILES ${ECS_SOURCES} )
source_group( "Source Files/Core/Systems" FILES ${SYSTEM_SOURCES} )
source_group( "Source Files/Core/Services" FILES ${SERVICE_SOURCES} )
source_group( "Source Files/Core/Components" FILES ${COMPONENT_SOURCES} )
source_group( "Source Files/Core/Resources" FILES ${RESOURCE_SOURCES} )
source_group( "Source Files/Core" FILES ${CORE_MODULE_SOURCES} )
source_group( "Source Files/Behavior" FILES ${BEHAVIOR_SOURCES} )
source_group( "Source Files/Input" FILES ${INPUT_SOURCES} )
source_group( "Source Files/Events" FILES ${EVENT_SOURCES} )
source_group( "Source Files/Temp (Legacy)" FILES ${TEMP_SOURCES} )
source_group( "Source Files/Private" FILES ${PRIVATE_SOURCES} )