# # "cmake-lists": rhaster-engine, "author": alessandromanzini
# rhaster-engine specific logic here.
#
project( "rhaster-engine" )

add_library(
        ${PROJECT_NAME}
        "include/public/rst/__behaviour/fsm/transition/base_transition.h"
        "include/public/rst/__behaviour/fsm/transition/logic/And.h"
        "include/public/rst/__behaviour/fsm/transition/logic/Or.h"
        "include/public/rst/__behaviour/fsm/transition/logic/Nand.h"
        "include/public/rst/__behaviour/fsm/transition/logic/Nor.h"
        "include/public/rst/__behaviour/fsm/transition/logic/Not.h"
        "src/state_stack.cpp"
        "src/finite_multi_state_machine.cpp"
        "src/finite_state_machine.cpp"

        "src/input_merge.cpp"
        "src/binding_device.cpp"
        "src/input_buffer.cpp"
        "src/input_mapping_context.cpp"
        "include/public/rst/__command/command.h"
        "include/public/rst/__command/gameobject_command.h"

        "include/public/rst/__controller/base_controller.h"
        "src/player_controller.cpp"

        "include/public/rst/__event/dispatcher.h"
        "include/public/rst/__event/event_queue.h"
        "src/Subject.cpp"

        "src/gameobject.cpp"
        "src/scene.cpp"
        "src/transform.cpp"
        "src/hare.cpp"

        "src/audio_Component.cpp"
        "src/Component.cpp"
        "src/text_component.cpp"
        "src/texture_component.cpp"
        "src/box_collider_component.cpp"
        "src/collider_component.cpp"
        "src/physics_component.cpp"

        "include/public/rst/type/blackboard.h"
        "include/public/rst/type/deleter.h"
        "include/public/rst/type/optional_ref.h"
        "include/public/rst/type/sparse_set.h"
        "src/font.cpp"
        "src/Sprite2D.cpp"
        "src/Texture2D.cpp"
        "src/Audio.cpp"
        "src/sdl_audio.cpp"

        "include/public/rst/__service/service_info.h"
        "src/sdl_sound_system.cpp"
        "src/sound_system_logger.cpp"
        "src/parallel_sound_system.cpp"

        "src/game_instance.cpp"
        "src/game_time.cpp"
        "src/input_system.cpp"
        "src/Renderer.cpp"
        "src/resource_manager.cpp"
        "src/scene_pool.cpp"
        "src/service_locator.cpp"

        "include/public/rst/pch.h"

        include/public/rst/fsm.h
        include/public/rst/framework/multicast_delegate.h
        include/public/rst/framework/observer.h
        include/public/rst/framework/event_queue.h
        include/public/rst/__core/test.h
        src/earmark.cpp
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
