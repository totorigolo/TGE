#include <lua.hpp>
#include "ScriptMachine.h"
#include "../Game/Game.h"
#include "../App/InputManager.h"
#include "../Level/LevelManager.h"
#include "../Physics/JointFactory.h"
#include "../Physics/PhysicManager.h"
#include "../Entities/EntityFactory.h"
#include "../Entities/Player.h"

// Ctor & dtor
ScriptMachine::ScriptMachine()
	: mLuaConsoleWindow(nullptr)
{
	Init();
}
ScriptMachine::~ScriptMachine()
{
}

// Réinitialisation
void ScriptMachine::Reset()
{
	// TODO
}

// Enregistrement des attributs
void ScriptMachine::UnregisterGlobalVar(const std::string &name)
{
	try
	{
//		luabind::globals(mLuaState)[name] = luabind::nil;
	}
	catch (const std::exception &e)
	{
		std::cerr << e.what() << std::endl;
	}
}

// Exécution
int ScriptMachine::DoFile(const std::string &path, OutputInterface *_interface)
{
	// Normalise l'interface
	auto output = GetInterface(_interface);

	try
	{
#ifdef _DEBUG
		sf::Clock c;
		int r = ReportError(luaL_dofile(mLuaState, path.c_str()), output);
		*output << "Script \"" << path << "\" ex\x82""cut\x82 en : "
			<< c.getElapsedTime().asSeconds() << " sec" << std::endl << std::endl;
		return r;
#else
		return ReportError(luaL_dofile(mLuaState, path.c_str()), output);
#endif
	}
	catch (const std::exception &e)
	{
		*output << e.what() << std::endl;
	}

	// Si on est là, c'est à cause d'une erreur
	return -1;
}
int ScriptMachine::LoadFile(const std::string &path, OutputInterface *_interface)
{
	// Normalise l'interface
	auto output = GetInterface(_interface);

	try
	{
#ifdef _DEBUG
		sf::Clock c;
		int r = ReportError(luaL_loadfile(mLuaState, path.c_str()), output);
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

	// Si on est là, c'est à cause d'une erreur
	return -1;
}
int ScriptMachine::DoString(const std::string &command, OutputInterface *_interface)
{
	// Normalise l'interface
	auto output = GetInterface(_interface);

	try
	{
#ifdef _DEBUG
		sf::Clock c;
		int r = ReportError(luaL_dostring(mLuaState, command.c_str()), output);
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

	// Si on est là, c'est à cause d'une erreur
	return -1;
}

// Enregistre la console de l'IHM
void ScriptMachine::SetConsole(LuaConsoleWindow *window)
{
	myCheckError_v(window, "LuaConsoleWindow passée invalide.");
	mLuaConsoleWindow = window;
}
OutputInterface* ScriptMachine::GetInterface(OutputInterface *_interface)
{
	auto tmp = _interface;

	// Crée une interface si on en a pas
	if (!tmp)
	{
		if (mLuaConsoleWindow)
			tmp = new LuaConsoleInterface(*mLuaConsoleWindow);
		else
			tmp = new ostreamInterface();
	}
	return tmp;
}

// Initialisation
void ScriptMachine::Init()
{
	// Crée un Lua State
	mLuaState = luaL_newstate();
	luaL_openlibs(mLuaState);

	// Connecte LuaBind à ce Lua State
//	luabind::open(mLuaState);

	// Enregistre les classes
	RegisterBox2D();
	RegisterSFML();
}

// Affichage des erreurs
int ScriptMachine::ReportError(int errorCode, OutputInterface *_interface)
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
		*_interface << "Lua: Pas assez de mémoire (LUA_ERRMEM)." << std::endl;
	
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
