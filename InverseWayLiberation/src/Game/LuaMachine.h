#pragma once
#include <lua.hpp>
// Désactive les warning "warning C4251: ...nécessite une interface DLL pour être utilisé(e) par les clients..."
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
	// HACK: Déplacer ça dans les classes respectives ?
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

	// Exécution
	int DoFile(const std::string &path);
	int LoadFile(const std::string &path);
	int DoString(const std::string &command);

protected:
	// Enregistrements privés
	void RegisterBox2D();
	void RegisterSFML();

private:
	// Affichage des erreurs
	int ReportLuaError(int errorCode);

private:
	// State Lua
	lua_State *mLuaState;
};
