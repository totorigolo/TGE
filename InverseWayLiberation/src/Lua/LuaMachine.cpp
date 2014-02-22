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
{
	// Initialise le Lua
	InitLua();
}
LuaMachine::~LuaMachine()
{
	// Ferme le State Lua
	lua_close(mLuaState);
}

// Réinitialisation
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

// Exécution
int LuaMachine::DoFile(const std::string &path, OutputInterface *interface)
{
	try
	{
#ifdef _DEBUG
		sf::Clock c;
		int r = ReportLuaError(luaL_dofile(mLuaState, path.c_str()), interface);
		*interface << "Script \"" << path << "\" ex\x82""cut\x82 en : "
			<< c.getElapsedTime().asSeconds() << " sec" << std::endl << std::endl;
		return r;
#else
		return ReportLuaError(luaL_dofile(mLuaState, path.c_str()), interface);
#endif
	}
	catch (const std::exception &e)
	{
		*interface << e.what() << std::endl;
	}

	// Si on est là, c'est à cause d'une erreur
	return -1;
}
int LuaMachine::LoadFile(const std::string &path, OutputInterface *interface)
{
	try
	{
#ifdef _DEBUG
		sf::Clock c;
		int r = ReportLuaError(luaL_loadfile(mLuaState, path.c_str()), interface);
		*interface << "Fichier \"" << path << "\" charg\x82 en : "
			<< c.getElapsedTime().asSeconds() << " sec" << std::endl << std::endl;
		return r;
#else
		return ReportLuaError(luaL_loadfile(mLuaState, path.c_str()), interface);
#endif
	}
	catch (const std::exception &e)
	{
		*interface << e.what() << std::endl;
	}

	// Si on est là, c'est à cause d'une erreur
	return -1;
}
int LuaMachine::DoString(const std::string &command, OutputInterface *interface)
{
	try
	{
#ifdef _DEBUG
		sf::Clock c;
		int r = ReportLuaError(luaL_dostring(mLuaState, command.c_str()), interface);
		*interface << "Commande \"" << command << "\" ex\x82""cut\x82 en : "
			<< c.getElapsedTime().asSeconds() << " sec" << std::endl << std::endl;
		return r;
#else
		return ReportLuaError(luaL_dostring(mLuaState, command.c_str()), interface);
#endif
	}
	catch (const std::exception &e)
	{
		*interface << e.what() << std::endl;
	}

	// Si on est là, c'est à cause d'une erreur
	return -1;
}

// Accesseur
lua_State* LuaMachine::GetLuaState()
{
	return mLuaState;
}

// Initialisation
void LuaMachine::InitLua()
{
	// Crée un Lua State
	mLuaState = luaL_newstate();
	luaL_openlibs(mLuaState);

	// Connecte LuaBind à ce Lua State
	luabind::open(mLuaState);

	// Enregistre les classes
	RegisterBox2D();
	RegisterSFML();
}

// Affichage des erreurs
int LuaMachine::ReportLuaError(int errorCode, OutputInterface *interface)
{
	// Pas d'erreur
	if (errorCode == 0)
	{
		*interface << "Aucune erreur." << std::endl;
		return errorCode;
	}

	// Erreurs connus
	else if (errorCode == LUA_ERRSYNTAX)
		*interface << "Lua: Erreur de syntaxe (LUA_ERRSYNTAX)." << std::endl;

	else if (errorCode == LUA_ERRRUN)
		*interface << "Lua: Erreur de Runtime (LUA_ERRRUN)." << std::endl;

	else if (errorCode == LUA_ERRMEM)
		*interface << "Lua: Pas assez de mémoire (LUA_ERRMEM)." << std::endl;
	
	else if (errorCode == LUA_ERRERR)
		*interface << "Lua: Erreur intraitable (LUA_ERRERR)." << std::endl;

	else if (errorCode == LUA_ERRFILE)
		*interface << "Lua: Fichier introuvable (LUA_ERRFILE)." << std::endl;

	// Erreur inconnu
	else
		*interface << "Lua: Erreur inconnue (" << errorCode << ")." << std::endl;
	
	// Affiche l'erreur sur la pile
	*interface << lua_tostring(mLuaState, -1) << std::endl;

	return errorCode;
}

/*
JointFactory.CreatePulleyJoint(1, b2Vec2(0.f, 0.f), 2, b2Vec2(0.f, 0.f), b2Vec2(-2.f, 0.5f), b2Vec2(1.9f, 4.f), 1.5f)
JointFactory.CreateRopeJoint(3, b2Vec2(0.f, 0.f), 4, b2Vec2(0.f, 0.f), 1.f, true, (0, 255, 255))
*/
