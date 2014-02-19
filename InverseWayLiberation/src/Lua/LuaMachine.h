#pragma once
#include "OutputInterfaces.h"
#include "../Tools/NonCopyable.h"

#include <lua.hpp>
// D�sactive les warning "warning C4251: ...n�cessite une interface DLL pour �tre utilis�(e) par les clients..."
#pragma warning (disable : 4251)
#include <luabind/luabind.hpp>
#include <memory>
#include <string>

class LuaMachine : public NonCopyable
{
public:
	// Ctor & dtor
	LuaMachine();
	virtual ~LuaMachine();

	// R�initialisation
	void Reset();

	// Enregistrements
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
			luabind::globals(mLuaState)[name] = var;
		}
		catch (const std::exception &e)
		{
			std::cerr << e.what() << std::endl;
		}
	}
	void UnregisterGlobalLuaVar(const std::string &name);

	// Ex�cution
	int DoFile(const std::string &path, OutputInterface *interface = new ostreamInterface());
	int LoadFile(const std::string &path, OutputInterface *interface = new ostreamInterface());
	int DoString(const std::string &command, OutputInterface *interface = new ostreamInterface());

	// Accesseur
	lua_State* GetLuaState();

protected:
	// Enregistrements priv�s
	void RegisterBox2D();
	void RegisterSFML();

private:
	// Initialisation
	void InitLua();

	// Affichage des erreurs
	int ReportLuaError(int errorCode, OutputInterface *interface = new ostreamInterface());

private:
	// State Lua
	lua_State *mLuaState;
};