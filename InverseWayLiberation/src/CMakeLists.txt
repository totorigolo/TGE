cmake_minimum_required(VERSION 3.6)
project(inverse_way_liberation)

set(SRC_DIR InverseWayLiberation/src)

set(main_src
        ${SRC_DIR}/main.cpp
        ${SRC_DIR}/stdafx.h
        ${SRC_DIR}/stdafx.cpp

        PARENT_SCOPE
)

set(App_src
        ${SRC_DIR}/App/App.h
        ${SRC_DIR}/App/App.cpp
        ${SRC_DIR}/App/InputManager.h
        ${SRC_DIR}/App/InputManager.cpp
        ${SRC_DIR}/App/SpyedKey.h
        ${SRC_DIR}/App/States/State.h
        ${SRC_DIR}/App/States/StateSystem.h
        ${SRC_DIR}/App/States/StateSystem.cpp
        ${SRC_DIR}/App/States/StartUp_State.h
        ${SRC_DIR}/App/States/StartUp_State.cpp
        ${SRC_DIR}/App/States/MainMenu_State.h
        ${SRC_DIR}/App/States/MainMenu_State.cpp
        ${SRC_DIR}/App/States/Editor_State.h
        ${SRC_DIR}/App/States/Editor_State.cpp
        ${SRC_DIR}/App/States/Game_State.h
        ${SRC_DIR}/App/States/Game_State.cpp
        ${SRC_DIR}/App/States/Configuration_State.h
        ${SRC_DIR}/App/States/Configuration_State.cpp

        PARENT_SCOPE
)

set(Editor_src
        ${SRC_DIR}/Editor/Editor.h
        ${SRC_DIR}/Editor/Editor.cpp
        ${SRC_DIR}/Editor/EditBox.h
        ${SRC_DIR}/Editor/EditBox.cpp
        ${SRC_DIR}/Editor/GUI/EmptyScenario.h
        ${SRC_DIR}/Editor/GUI/EmptyScenario.cpp
        ${SRC_DIR}/Editor/GUI/BodyScenario.h
        ${SRC_DIR}/Editor/GUI/BodyScenario.cpp
        ${SRC_DIR}/Editor/GUI/BasicBodyCreationWindow.h
        ${SRC_DIR}/Editor/GUI/BasicBodyCreationWindow.cpp
        ${SRC_DIR}/Editor/GUI/ColFilteringWindow.h
        ${SRC_DIR}/Editor/GUI/ColFilteringWindow.cpp
        ${SRC_DIR}/Editor/GUI/DecoScenario.h
        ${SRC_DIR}/Editor/GUI/DecoScenario.cpp
        ${SRC_DIR}/Editor/GUI/DecoCreationWindow.h
        ${SRC_DIR}/Editor/GUI/DecoCreationWindow.cpp
        ${SRC_DIR}/Editor/GUI/Window.cpp
        ${SRC_DIR}/Editor/GUI/TexturesWindow.h
        ${SRC_DIR}/Editor/GUI/TexturesWindow.cpp
        ${SRC_DIR}/Editor/GUI/Scenario.h
        ${SRC_DIR}/Editor/GUI/Scenario.cpp
        ${SRC_DIR}/Editor/GUI/PolyCreationWindow.h
        ${SRC_DIR}/Editor/GUI/PolyCreationWindow.cpp
        ${SRC_DIR}/Editor/GUI/PointLightScenario.h
        ${SRC_DIR}/Editor/GUI/PointLightScenario.cpp
        ${SRC_DIR}/Editor/GUI/PointLightCreationWindow.h
        ${SRC_DIR}/Editor/GUI/PointLightCreationWindow.cpp
        ${SRC_DIR}/Editor/GUI/LuaConsoleWindow.h
        ${SRC_DIR}/Editor/GUI/LuaConsoleWindow.cpp
        ${SRC_DIR}/Editor/GUI/LevelWindow.h
        ${SRC_DIR}/Editor/GUI/LevelWindow.cpp
        ${SRC_DIR}/Editor/GUI/HumScenario.h
        ${SRC_DIR}/Editor/GUI/HumScenario.cpp
        ${SRC_DIR}/Editor/GUI/HumCreationWindow.h
        ${SRC_DIR}/Editor/GUI/HumCreationWindow.cpp

        PARENT_SCOPE
)

set(Entities_src
        ${SRC_DIR}/Entities/Entity.h
        ${SRC_DIR}/Entities/Entity.cpp
        ${SRC_DIR}/Entities/EntityManager.h
        ${SRC_DIR}/Entities/EntityManager.cpp
        ${SRC_DIR}/Entities/EntityFactory.h
        ${SRC_DIR}/Entities/EntityFactory.cpp
        ${SRC_DIR}/Entities/PartitioningTree.h
        ${SRC_DIR}/Entities/PartitioningTree.cpp
        ${SRC_DIR}/Entities/BaseBody.h
        ${SRC_DIR}/Entities/BaseBody.cpp
        ${SRC_DIR}/Entities/BasicBody.h
        ${SRC_DIR}/Entities/BasicBody.cpp
        ${SRC_DIR}/Entities/PolyBody.h
        ${SRC_DIR}/Entities/PolyBody.cpp
        ${SRC_DIR}/Entities/PolyChain.h
        ${SRC_DIR}/Entities/PolyChain.cpp
        ${SRC_DIR}/Entities/Deco.h
        ${SRC_DIR}/Entities/Deco.cpp
        ${SRC_DIR}/Entities/Hull.h
        ${SRC_DIR}/Entities/Hull.cpp
        ${SRC_DIR}/Entities/Hum.h
        ${SRC_DIR}/Entities/Hum.cpp
        ${SRC_DIR}/Entities/Player.h
        ${SRC_DIR}/Entities/Player.cpp
        ${SRC_DIR}/Entities/PointLight.h
        ${SRC_DIR}/Entities/PointLight.cpp

        PARENT_SCOPE
)

set(Game_src
        ${SRC_DIR}/Game/Game.h
        ${SRC_DIR}/Game/Game.cpp

        PARENT_SCOPE
)

set(Graphics_src
        ${SRC_DIR}/Graphics/LightEngine.h
        ${SRC_DIR}/Graphics/LightEngine.cpp

        PARENT_SCOPE
)

set(Level_src
        ${SRC_DIR}/Level/LevelManager.h
        ${SRC_DIR}/Level/LevelManager.cpp
        ${SRC_DIR}/Level/LevelLoader.h
        ${SRC_DIR}/Level/LevelLoader.cpp
        ${SRC_DIR}/Level/LevelSaver.h
        ${SRC_DIR}/Level/LevelSaver.cpp

        PARENT_SCOPE
)

set(Lua_src
        ${SRC_DIR}/Lua/LuaMachine.h
        ${SRC_DIR}/Lua/LuaMachine.cpp
        ${SRC_DIR}/Lua/LuaArea.h
        ${SRC_DIR}/Lua/LuaArea.cpp
        ${SRC_DIR}/Lua/LuaAction.h
        ${SRC_DIR}/Lua/LuaAction.cpp
        ${SRC_DIR}/Lua/OutputInterfaces.h
        ${SRC_DIR}/Lua/TriggersManager.h
        ${SRC_DIR}/Lua/TriggersManager.cpp
        ${SRC_DIR}/Lua/Registrations/RegisterSFML.cpp
        ${SRC_DIR}/Lua/Registrations/RegisterBox2D.cpp
        ${SRC_DIR}/Lua/Registrations/RegisterEntityFactory.cpp
        ${SRC_DIR}/Lua/Registrations/RegisterInputManager.cpp
        ${SRC_DIR}/Lua/Registrations/RegisterJoint.cpp
        ${SRC_DIR}/Lua/Registrations/RegisterJointFactory.cpp
        ${SRC_DIR}/Lua/Registrations/RegisterLevelManager.cpp
        ${SRC_DIR}/Lua/Registrations/RegisterPhysicManager.cpp
        ${SRC_DIR}/Lua/Registrations/RegisterResourceManager.cpp

        PARENT_SCOPE
)

set(Physics_src
        ${SRC_DIR}/Physics/PhysicManager.h
        ${SRC_DIR}/Physics/PhysicManager.cpp
        ${SRC_DIR}/Physics/Joint.h
        ${SRC_DIR}/Physics/Joint.cpp
        ${SRC_DIR}/Physics/JointFactory.h
        ${SRC_DIR}/Physics/JointFactory.cpp
        ${SRC_DIR}/Physics/ContactListener.h
        ${SRC_DIR}/Physics/ContactListener.cpp
        ${SRC_DIR}/Physics/DebugDraw.h
        ${SRC_DIR}/Physics/DebugDraw.cpp
        ${SRC_DIR}/Physics/Callback/AABBCallback.h
        ${SRC_DIR}/Physics/Callback/AABBListCallback.h
        ${SRC_DIR}/Physics/Callback/FirstBodyRaycastCallback.h
        ${SRC_DIR}/Physics/Callback/PointCallback.h
        ${SRC_DIR}/Physics/Joints/DistanceJoint.h
        ${SRC_DIR}/Physics/Joints/DistanceJoint.cpp
        ${SRC_DIR}/Physics/Joints/FrictionJoint.h
        ${SRC_DIR}/Physics/Joints/FrictionJoint.cpp
        ${SRC_DIR}/Physics/Joints/GearJoint.h
        ${SRC_DIR}/Physics/Joints/GearJoint.cpp
        ${SRC_DIR}/Physics/Joints/MouseJoint.h
        ${SRC_DIR}/Physics/Joints/MouseJoint.cpp
        ${SRC_DIR}/Physics/Joints/PrismaticJoint.h
        ${SRC_DIR}/Physics/Joints/PrismaticJoint.cpp
        ${SRC_DIR}/Physics/Joints/PulleyJoint.h
        ${SRC_DIR}/Physics/Joints/PulleyJoint.cpp
        ${SRC_DIR}/Physics/Joints/RevoluteJoint.h
        ${SRC_DIR}/Physics/Joints/RevoluteJoint.cpp
        ${SRC_DIR}/Physics/Joints/RopeJoint.h
        ${SRC_DIR}/Physics/Joints/RopeJoint.cpp
        ${SRC_DIR}/Physics/Joints/WeldJoint.h
        ${SRC_DIR}/Physics/Joints/WeldJoint.cpp
        ${SRC_DIR}/Physics/Joints/WheelJoint.h
        ${SRC_DIR}/Physics/Joints/WheelJoint.cpp

        PARENT_SCOPE
)

set(Resources_src
        ${SRC_DIR}/Resources/ResourceManager.h
        ${SRC_DIR}/Resources/ResourceManager.cpp
        ${SRC_DIR}/Resources/Font.h
        ${SRC_DIR}/Resources/Texture.h

        PARENT_SCOPE
)

set(Sounds_src
        ${SRC_DIR}/Sounds/SoundManager.h
        ${SRC_DIR}/Sounds/SoundManager.cpp

        PARENT_SCOPE
)

set(Tools_src
        ${SRC_DIR}/Tools/Dialog.h
        ${SRC_DIR}/Tools/Dialog.cpp
        ${SRC_DIR}/Tools/Error.h
        ${SRC_DIR}/Tools/Error.cpp
        ${SRC_DIR}/Tools/Loader.h
        ${SRC_DIR}/Tools/Loader.cpp
        ${SRC_DIR}/Tools/NonCopyable.h
        ${SRC_DIR}/Tools/Parser.h
        ${SRC_DIR}/Tools/Parser.cpp
        ${SRC_DIR}/Tools/PromptEntry.h
        ${SRC_DIR}/Tools/PromptEntry.cpp
        ${SRC_DIR}/Tools/Saver.h
        ${SRC_DIR}/Tools/Saver.cpp
        ${SRC_DIR}/Tools/Segment.h
        ${SRC_DIR}/Tools/Singleton.h
        ${SRC_DIR}/Tools/utils.h
        ${SRC_DIR}/Tools/vector_wrappers.h

        PARENT_SCOPE
)
