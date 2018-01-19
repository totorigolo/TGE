#pragma once

#include <lua.h>
#include "OutputInterfaces.h"
#include "../Editor/GUI/LuaConsoleWindow.h"

class ScriptMachine : public NonCopyable {
public:
    // Ctor & dtor
    ScriptMachine();

    virtual ~ScriptMachine();

    // Réinitialisation
    void Reset();

    // Enregistrements
    void RegisterJoint();

// TODO: RegisterEntity
//	void RegisterEntity();
    void RegisterEntityFactory();

    void RegisterJointFactory();

    void RegisterLevelManager();

    void RegisterPhysicManager();

    void RegisterInputManager();

    void RegisterResourceManager();

    // Enregistrement des attributs
    template<typename T>
    void RegisterGlobalVar(const std::string &name, T *var) {
        try {
//			luabind::globals(mLuaState)[name] = var;
        }
        catch (const std::exception &e) {
            std::cerr << e.what() << std::endl;
        }
    }

    void UnregisterGlobalVar(const std::string &name);

    // Exécution
    int DoFile(const std::string &path, OutputInterface *_interface = nullptr);//new ostreamInterface());
    int LoadFile(const std::string &path, OutputInterface *_interface = nullptr);//new ostreamInterface());
    int DoString(const std::string &command, OutputInterface *_interface = nullptr);//new ostreamInterface());

    // Enregistre la console de scripts
    void SetConsole(LuaConsoleWindow *window);

    OutputInterface *GetInterface(OutputInterface *_interface = nullptr);

protected:
    // Enregistrements privés
    void RegisterBox2D();

    void RegisterSFML();

private:
    // Initialisation
    void Init();

    // Affichage des erreurs
    int ReportError(int errorCode, OutputInterface *_interface = new ostreamInterface());

private:
    // State Lua
    lua_State *mLuaState;

    // Console Lua
    LuaConsoleWindow *mLuaConsoleWindow;
};
