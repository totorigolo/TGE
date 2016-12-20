#pragma once

#include <lua.h>
#include "OutputInterfaces.h"
#include "../Editor/GUI/LuaConsoleWindow.h"

class LuaMachine : public NonCopyable
{
public:
	// Ctor & dtor
	LuaMachine();
	virtual ~LuaMachine();

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
	template <typename T>
	void RegisterGlobalLuaVar(const std::string &name, T *var)
	{
		try
		{
//			luabind::globals(mLuaState)[name] = var;
		}
		catch (const std::exception &e)
		{
			std::cerr << e.what() << std::endl;
		}
	}
	void UnregisterGlobalLuaVar(const std::string &name);

	// Exécution
	int DoFile(const std::string &path, OutputInterface *_interface = nullptr);//new ostreamInterface());
	int LoadFile(const std::string &path, OutputInterface *_interface = nullptr);//new ostreamInterface());
	int DoString(const std::string &command, OutputInterface *_interface = nullptr);//new ostreamInterface());

	// Enregistre la console Lua
	void SetLuaConsole(LuaConsoleWindow *window);
	OutputInterface* GetInterface(OutputInterface *_interface = nullptr);

	// Accesseur
	lua_State* GetLuaState();

protected:
	// Enregistrements privés
	void RegisterBox2D();
	void RegisterSFML();

private:
	// Initialisation
	void InitLua();

	// Affichage des erreurs
	int ReportLuaError(int errorCode, OutputInterface *_interface = new ostreamInterface());

private:
	// State Lua
	lua_State *mLuaState;

	// Console Lua
	LuaConsoleWindow *mLuaConsoleWindow;
};
