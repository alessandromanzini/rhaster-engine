# # "cmake-lists": rhaster-engine, "author": alessandromanzini
# rhaster-engine specific logic here.
#
project( "rhaster-engine" )

add_library(
        ${PROJECT_NAME}
        "include/public/framework/behaviour/fsm/__transition/base_transitions.h"
        "include/public/framework/behaviour/fsm/__transition/logic/And.h"
        "include/public/framework/behaviour/fsm/__transition/logic/Or.h"
        "include/public/framework/behaviour/fsm/__transition/logic/Nand.h"
        "include/public/framework/behaviour/fsm/__transition/logic/Nor.h"
        "include/public/framework/behaviour/fsm/__transition/logic/Not.h"
        "include/public/framework/behaviour/fsm/transitions.h"
        "src/framework/behaviour/fsm/__state_machine/StateStack.cpp"
        "src/framework/behaviour/fsm/FiniteMultiStateMachine.cpp"
        "src/framework/behaviour/fsm/FiniteStateMachine.cpp"

        "src/framework/binding/binding_controls.cpp"
        "src/framework/binding/binding_device.cpp"
        "src/framework/binding/InputBuffer.cpp"
        "src/framework/binding/InputMappingContext.cpp"
        "include/public/framework/binding/command/Command.h"
        "include/public/framework/binding/command/GameObjectCommand.h"

        "include/public/framework/controller/BaseController.h"
        "src/framework/controller/PlayerController.cpp"

        "include/public/framework/event/Dispatcher.h"
        "include/public/framework/event/EventQueue.h"
        "src/framework/event/Subject.cpp"

        "src/framework/GameObject.cpp"
        "src/framework/GameObjectView.cpp"
        "src/framework/Scene.cpp"
        "src/framework/Transform.cpp"

        "src/framework/component/AudioComponent.cpp"
        "src/framework/component/Component.cpp"
        "src/framework/component/TextComponent.cpp"
        "src/framework/component/TextureComponent.cpp"
        "src/framework/component/physics/BoxColliderComponent.cpp"
        "src/framework/component/physics/ColliderComponent.cpp"
        "src/framework/component/physics/PhysicsComponent.cpp"
        "src/framework/ui_component/InputDisplayUIComponent.cpp"
        "src/framework/ui_component/UIComponent.cpp"

        "include/public/framework/resource/data/Blackboard.h"
        "include/public/framework/resource/data/Deleter.h"
        "include/public/framework/resource/data/OptionalRef.h"
        "include/public/framework/resource/data/SparseSet.h"
        "src/framework/resource/Font.cpp"
        "src/framework/resource/texture/Sprite2D.cpp"
        "src/framework/resource/texture/Texture2D.cpp"
        "src/framework/resource/sound/Audio.cpp"
        "src/framework/resource/sound/SdlAudio.cpp"

        "src/core/Minigin.cpp"

        "include/public/service/service_info.h"
        "src/service/sound/SdlSoundSystem.cpp"
        "src/service/sound/SoundSystemLogger.cpp"
        "src/service/sound/ParallelSoundSystem.cpp"

        "src/singleton/GameInstance.cpp"
        "src/singleton/GameTime.cpp"
        "src/singleton/InputSystem.cpp"
        "src/singleton/Renderer.cpp"
        "src/singleton/ResourceManager.cpp"
        "src/singleton/ScenePool.cpp"
        "src/singleton/UIController.cpp"
        "src/singleton/ServiceLocator.cpp"
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
