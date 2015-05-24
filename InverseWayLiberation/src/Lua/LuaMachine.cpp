#include "stdafx.h"
#include "LuaMachine.h"
#include "../Game/Game.h"
#include "../App/InputManager.h"
#include "../Level/LevelManager.h"
#include "../Physics/JointFactory.h"
#include "../Physics/PhysicManager.h"
#include "../Entities/EntityFactory.h"
#include "../Entities/Player.h"

// Ctor & dtor
LuaMachine::LuaMachine()
	: mLuaConsoleWindow(nullptr)
{
	// Initialise le Lua
	InitLua();
}
LuaMachine::~LuaMachine()
{
	// Ferme le State Lua
	lua_close(mLuaState);
}

// R�initialisation
void LuaMachine::Reset()
{
	// TODO
}

// Enregistrement des attributs
void LuaMachine::UnregisterGlobalLuaVar(const std::string &name)
{
	try
	{
		luabind::globals(mLuaState)[name] = luabind::nil;
	}
	catch (const std::exception &e)
	{
		std::cerr << e.what() << std::endl;
	}
}

// Ex�cution
int LuaMachine::DoFile(const std::string &path, OutputInterface *_interface)
{
	// Normalise l'interface
	auto output = GetInterface(_interface);

	try
	{
#ifdef _DEBUG
		sf::Clock c;
		int r = ReportLuaError(luaL_dofile(mLuaState, path.c_str()), output);
		*output << "Script \"" << path << "\" ex\x82""cut\x82 en : "
			<< c.getElapsedTime().asSeconds() << " sec" << std::endl << std::endl;
		return r;
#else
		return ReportLuaError(luaL_dofile(mLuaState, path.c_str()), output);
#endif
	}
	catch (const std::exception &e)
	{
		*output << e.what() << std::endl;
	}

	// Si on est l�, c'est � cause d'une erreur
	return -1;
}
int LuaMachine::LoadFile(const std::string &path, OutputInterface *_interface)
{
	// Normalise l'interface
	auto output = GetInterface(_interface);

	try
	{
#ifdef _DEBUG
		sf::Clock c;
		int r = ReportLuaError(luaL_loadfile(mLuaState, path.c_str()), output);
		*output << "Fichier \"" << path << "\" charg\x82 en : "
			<< c.getElapsedTime().asSeconds() << " sec" << std::endl << std::endl;
		return r;
#else
		return ReportLuaError(luaL_loadfile(mLuaState, path.c_str()), output);
#endif
	}
	catch (const std::exception &e)
	{
		*output << e.what() << std::endl;
	}

	// Si on est l�, c'est � cause d'une erreur
	return -1;
}
int LuaMachine::DoString(const std::string &command, OutputInterface *_interface)
{
	// Normalise l'interface
	auto output = GetInterface(_interface);

	try
	{
#ifdef _DEBUG
		sf::Clock c;
		int r = ReportLuaError(luaL_dostring(mLuaState, command.c_str()), output);
		*output << "Commande \"" << command << "\" ex\x82""cut\x82 en : "
			<< c.getElapsedTime().asSeconds() << " sec" << std::endl << std::endl;
		return r;
#else
		return ReportLuaError(luaL_dostring(mLuaState, command.c_str()), output);
#endif
	}
	catch (const std::exception &e)
	{
		*output << e.what() << std::endl;
	}

	// Si on est l�, c'est � cause d'une erreur
	return -1;
}

// Enregistre la console Lua
void LuaMachine::SetLuaConsole(LuaConsoleWindow *window)
{
	myCheckError_v(window, "LuaConsoleWindow pass�e invalide.");
	mLuaConsoleWindow = window;
}
OutputInterface* LuaMachine::GetInterface(OutputInterface *_interface)
{
	auto tmp = _interface;

	// Cr�e une interface si on en a pas
	if (!tmp)
	{
		if (mLuaConsoleWindow)
			tmp = new LuaConsoleInterface(*mLuaConsoleWindow);
		else
			tmp = new ostreamInterface();
	}
	return tmp;
}

// Accesseur
lua_State* LuaMachine::GetLuaState()
{
	return mLuaState;
}

// Initialisation
void LuaMachine::InitLua()
{
	// Cr�e un Lua State
	mLuaState = luaL_newstate();
	luaL_openlibs(mLuaState);

	// Connecte LuaBind � ce Lua State
	luabind::open(mLuaState);

	// Enregistre les classes
	RegisterBox2D();
	RegisterSFML();
}

// Affichage des erreurs
int LuaMachine::ReportLuaError(int errorCode, OutputInterface *_interface)
{
	// Pas d'erreur
	if (errorCode == 0)
	{
		//*_interface << "Aucune erreur." << std::endl;
		return errorCode;
	}

	// Erreurs connus
	else if (errorCode == LUA_ERRSYNTAX)
		*_interface << "Lua: Erreur de syntaxe (LUA_ERRSYNTAX)." << std::endl;

	else if (errorCode == LUA_ERRRUN)
		*_interface << "Lua: Erreur de Runtime (LUA_ERRRUN)." << std::endl;

	else if (errorCode == LUA_ERRMEM)
		*_interface << "Lua: Pas assez de m�moire (LUA_ERRMEM)." << std::endl;
	
	else if (errorCode == LUA_ERRERR)
		*_interface << "Lua: Erreur intraitable (LUA_ERRERR)." << std::endl;

	else if (errorCode == LUA_ERRFILE)
		*_interface << "Lua: Fichier introuvable (LUA_ERRFILE)." << std::endl;

	// Erreur inconnu
	else
		*_interface << "Lua: Erreur inconnue (" << errorCode << ")." << std::endl;
	
	// Affiche l'erreur sur la pile
	lua_Debug debugInfo;
	int r = lua_getstack(mLuaState, -1, &debugInfo);
	if (r == 0)
	{
		*_interface << "Requested stack depth exceeded current stack depth" << std::endl;
	}
	lua_getinfo(mLuaState, "Slun", &debugInfo);
	const char * er = lua_tostring(mLuaState, -1);
	lua_pop(mLuaState, 1);

	std::string error = "Failure to convert error to string";
	if (er)
	{
		error = er;
	}
	*_interface <<
		"Error " << debugInfo.event << " at (line " << debugInfo.currentline << "): " << error << "\n" <<
		"\tIn a " << debugInfo.namewhat << " " << debugInfo.source << " function called " << debugInfo.name << ".\n" <<
		"\tIn " << debugInfo.short_src << " defined at " << debugInfo.linedefined << ".\n";

	return errorCode;
}
