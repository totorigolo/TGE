#pragma once
#include <lua.hpp>
// Désactive les warning "warning C4251: ...nécessite une interface DLL pour être utilisé(e) par les clients..."
#pragma warning (disable : 4251)
#include <luabind/luabind.hpp>
#include <string>
#include "../Tools/NonCopyable.h"

class LuaMachine : public NonCopyable
{
public:
	// Ctor & dtor
	LuaMachine();
	virtual ~LuaMachine();

	// Réinitialisation
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

	// Exécution
	int DoFile(const std::string &path);
	int LoadFile(const std::string &path);
	int DoString(const std::string &command);

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
	int ReportLuaError(int errorCode);

private:
	// State Lua
	lua_State *mLuaState;
};
