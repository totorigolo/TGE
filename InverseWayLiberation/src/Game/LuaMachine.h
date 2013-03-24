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
	// HACK: D�placer �a dans les classes respectives ?
	void RegisterEntityFactory();

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
