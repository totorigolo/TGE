#include "../ScriptMachine.h"
#include "../../App/InputManager.h"

// Enregistrements
void ScriptMachine::RegisterInputManager()
{
	try
	{
		// Enregistrement
		luabind::module(mLuaState) [
			// Level
			luabind::class_<InputManager>("InputManager")
				// Enum
				.enum_("KeyState")
				[
					luabind::value("None", KeyState::None),
					luabind::value("Pressed", KeyState::Pressed),
					luabind::value("Released", KeyState::Released)
				]
				/* Fxs */
				// Vue et zoom
				.def("SetZoom", &InputManager::SetZoom)
				.def("SetCenter", &InputManager::SetCenter)
				.def("GetCurrentZoom", &InputManager::GetCurrentZoom)
				.def("GetCurrentCenter", &InputManager::GetCurrentCenter)
				.property("zoom", &InputManager::GetCurrentZoom, &InputManager::SetZoom)
				.property("center", &InputManager::GetCurrentCenter, &InputManager::SetCenter)
				// Evènements
				.def("IsKeyPressed", &InputManager::IsKeyPressed)
				.def("GetMousePos", &InputManager::GetMousePos)
				.def("GetMousePosRV", &InputManager::GetMousePosRV)
				.def("GetLastMousePos", &InputManager::GetLastMousePos)
				.def("GetLastMousePosRV", &InputManager::GetLastMousePosRV)
				.def("GetLMBState", &InputManager::GetLMBState)
				.def("GetRMBState", &InputManager::GetRMBState)
				.def("GetMMBState", &InputManager::GetMMBState)
				// Espionnage des touches
				.def("AddSpyedKey", &InputManager::AddSpyedKey)
				.def("RemoveSpyedKey", &InputManager::RemoveSpyedKey)
				.def("KeyPressed", &InputManager::KeyPressed)
				.def("KeyReleased", &InputManager::KeyReleased)
				.def("GetSpyedKeyState", &InputManager::GetSpyedKeyState)
		];
	}
	catch (const std::exception &e)
	{
		std::cerr << e.what() << std::endl;
	}
}
