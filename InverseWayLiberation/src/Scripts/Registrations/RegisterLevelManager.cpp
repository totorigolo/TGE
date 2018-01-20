#include "../ScriptMachine.h"
#include "../../Entities/Player.h"
#include "../../Level/LevelManager.h"
#include "../../Level/LevelLoader.h"

// Enregistrements
void ScriptMachine::RegisterLevelManager() {
    try {
        // Enregistrement
        luabind::module(mLuaState)[
                // Level
                luabind::class_<LevelManager>("LevelManager")
                        // Fxs
                        .def("Load", &LevelManager::LoadFromFile)
                        .def("LoadFromFile", &LevelManager::LoadFromFile)
                        .def("GetPlayer", &LevelManager::GetPlayer,
                             luabind::dependency_policy<0, 1>())//(luabind::result, _1))
                        .def("PrepareForGame", &LevelManager::PrepareForGame)
                        .def("Clear", &LevelManager::Clear)
                                // Attributs
                        .property("player", &LevelManager::GetPlayer, &LevelManager::SetPlayer)
                        .property("charged", &LevelManager::IsCharged, &LevelManager::SetCharged)
                        .property("bckgcolor", &LevelManager::GetBckgColor, &LevelManager::SetBckgColor)
                        .property("defaultZoom", &LevelManager::GetDefaultZoom, &LevelManager::SetDefaultZoom)
                        .property("defaultCenter", &LevelManager::GetDefaultCenter, &LevelManager::SetDefaultCenter)
        ];
    }
    catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
    }
}