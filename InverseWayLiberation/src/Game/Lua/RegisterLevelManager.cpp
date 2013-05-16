#include "../LuaMachine.h"
#include "../../Entities/Player.h"
#include "../../Level/LevelManager.h"
#include <iostream>
#include <exception>

// Enregistrements
void LuaMachine::RegisterLevelManager()
{
	try
	{
		// Enregistrement
		luabind::module(mLuaState) [
			// Level
			luabind::class_<LevelManager>("LevelManager")
				// Fxs
				.def("LoadFromFile", &LevelManager::LoadFromFile)
				.def("GetPlayer", &LevelManager::GetPlayer, luabind::dependency(luabind::result, _1))
				.def("PrepareForGame", &LevelManager::PrepareForGame)
				.def("Clear", &LevelManager::Clear)
				// Attributs
				.property("charged", &LevelManager::IsCharged, &LevelManager::SetCharged)
				.property("bckgcolor", &LevelManager::GetBckgColor, &LevelManager::SetBckgColor)
		];
	}
	catch (const std::exception &e)
	{
		std::cerr << e.what() << std::endl;
	}
}