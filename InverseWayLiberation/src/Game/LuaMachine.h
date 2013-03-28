#pragma once
#include <lua.hpp>
// D�sactive les warning "warning C4251: ...n�cessite une interface DLL pour �tre utilis�(e) par les clients..."
#pragma warning (disable : 4251)
#include <luabind/luabind.hpp>
#include <string>

class LuaMachine
{
public:
	// Ctor & dtor
	LuaMachine();
	virtual ~LuaMachine();

	// Enregistrements
	// HACK: D�placer �a dans les classes respectives ?
	void RegisterEntityFactory();
	void RegisterJointFactory();
	void RegisterLevelManager();
	void RegisterPhysicManager();

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
	int DoFile(const std::string &path);
	int LoadFile(const std::string &path);
	int DoString(const std::string &command);

protected:
	// Enregistrements priv�s
	void RegisterBox2D();
	void RegisterSFML();

private:
	// Affichage des erreurs
	int ReportLuaError(int errorCode);

private:
	// State Lua
	lua_State *mLuaState;
};
