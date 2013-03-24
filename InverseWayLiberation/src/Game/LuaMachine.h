#pragma once
#include <lua.hpp>
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
