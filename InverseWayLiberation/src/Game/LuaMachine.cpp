#include "LuaMachine.h"
#include "../Tools/utils.h"
#include "../Entities/EntityFactory.h"
#include <luabind/luabind.hpp>
#include <luabind/operator.hpp>
#include <iostream>
#include <exception>
#include <Box2D/Box2D.h>

// Ctor & dtor
LuaMachine::LuaMachine()
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
LuaMachine::~LuaMachine()
{
	// Ferme le State Lua
	lua_close(mLuaState);
}

// Enregistrements
void LuaMachine::RegisterEntityFactory()
{
	try
	{
		// Enregistre les fonctions
		luabind::module(mLuaState) [
			luabind::namespace_("EntityFactory") [
				luabind::def("CreateBox", EntityFactory::CreateBox),
				luabind::def("CreateLamp", EntityFactory::CreateLamp),
				luabind::def("CreateCircle", EntityFactory::CreateCircle),
				luabind::def("CreateRagdoll", EntityFactory::CreateRagdoll)
			]
		];
	}
	catch (const std::exception &e)
	{
		std::cerr << e.what() << std::endl;
	}
}

// Exécution
int LuaMachine::DoFile(const std::string &path)
{
	return ReportLuaError(luaL_dofile(mLuaState, path.c_str()));
}
int LuaMachine::LoadFile(const std::string &path)
{
	return ReportLuaError(luaL_loadfile(mLuaState, path.c_str()));
}
int LuaMachine::DoString(const std::string &command)
{
	return ReportLuaError(luaL_dostring(mLuaState, command.c_str()));
}

// Enregistrements privés
void LuaMachine::RegisterBox2D()
{
	try
	{
		/* Enregistre les classes */
		// b2Vec2
		luabind::module(mLuaState) [
			// La classe
			luabind::class_<b2Vec2>("b2Vec2")
				// Ctors
				.def(luabind::constructor<>())
				.def(luabind::constructor<float32, float32>())
				// Fxs
				.def("SetZero", &b2Vec2::SetZero)
				.def("Set", &b2Vec2::Set)
				.def("Length", &b2Vec2::Length)
				.def("LengthSquared", &b2Vec2::LengthSquared)
				.def("Normalize", &b2Vec2::Normalize)
				.def("IsValid", &b2Vec2::IsValid)
				.def("Skew", &b2Vec2::Skew)
				// Ops
				.def(- luabind::const_self)
				.def(float32() * luabind::const_self)
				.def(luabind::const_self + b2Vec2())
				.def(luabind::const_self - b2Vec2())
				.def(luabind::const_self == b2Vec2())
				// Attributs
				.def_readwrite("x", &b2Vec2::x)
				.def_readwrite("y", &b2Vec2::y)
				,
			// Fxs
			luabind::def("b2Dot", (float32(*)(const b2Vec2&, const b2Vec2&)) b2Dot),
			luabind::def("b2Cross", (float32(*)(const b2Vec2&, const b2Vec2&)) b2Cross),
			luabind::def("b2Cross", (b2Vec2(*)(const b2Vec2&, float32)) b2Cross),
			luabind::def("b2Cross", (b2Vec2(*)(float32, const b2Vec2&)) b2Cross),
			luabind::def("b2Distance", (float32(*)(const b2Vec2&, const b2Vec2&)) b2Distance),
			luabind::def("b2DistanceSquared", (float32(*)(const b2Vec2&, const b2Vec2&)) b2DistanceSquared)
		];
	
		// b2Vec3
		luabind::module(mLuaState) [
			// La classe
			luabind::class_<b2Vec3>("b2Vec3")
				// Ctors
				.def(luabind::constructor<>())
				.def(luabind::constructor<float32, float32, float32>())
				// Fxs
				.def("SetZero", &b2Vec3::SetZero)
				.def("Set", &b2Vec3::Set)
				// Ops
				.def(- luabind::const_self)
				.def(float32() * luabind::const_self)
				.def(luabind::const_self + b2Vec3())
				.def(luabind::const_self - b2Vec3())
				// Attributs
				.def_readwrite("x", &b2Vec3::x)
				.def_readwrite("y", &b2Vec3::y)
				.def_readwrite("z", &b2Vec3::y)
				,
			// Fxs
			luabind::def("b2Dot", (float32(*)(const b2Vec3&, const b2Vec3&)) b2Dot),
			luabind::def("b2Cross", (b2Vec3(*)(const b2Vec3&, const b2Vec3&)) b2Cross)
		];
	}
	catch (const std::exception &e)
	{
		std::cerr << e.what() << std::endl;
	}
}
void LuaMachine::RegisterSFML()
{
	try
	{
		/* Enregistre les classes */
		// sf::Vector2f
		luabind::module(mLuaState) [
			// La classe
			luabind::class_<sf::Vector2f>("Vector2f")
				// Ctors
				.def(luabind::constructor<>())
				.def(luabind::constructor<float, float>())
				// Ops
				.def(- luabind::const_self)
				.def(luabind::const_self + sf::Vector2f())
				.def(luabind::const_self - sf::Vector2f())
				.def(luabind::const_self == sf::Vector2f())
				.def(luabind::const_self * float())
				.def(luabind::const_self / float())
				.def(float() * luabind::const_self)
				// Attributs
				.def_readwrite("x", &sf::Vector2f::x)
				.def_readwrite("y", &sf::Vector2f::y)
		];
	
		// sf::Vector2i
		luabind::module(mLuaState) [
			// La classe
			luabind::class_<sf::Vector2i>("Vector2i")
				// Ctors
				.def(luabind::constructor<>())
				.def(luabind::constructor<int, int>())
				// Ops
				.def(- luabind::const_self)
				.def(luabind::const_self + sf::Vector2i())
				.def(luabind::const_self - sf::Vector2i())
				.def(luabind::const_self == sf::Vector2i())
				.def(luabind::const_self * int())
				.def(luabind::const_self / int())
				.def(int() * luabind::const_self)
				// Attributs
				.def_readwrite("x", &sf::Vector2i::x)
				.def_readwrite("y", &sf::Vector2i::y)
		];
	}
	catch (const std::exception &e)
	{
		std::cerr << e.what() << std::endl;
	}
}

// Affichage des erreurs
int LuaMachine::ReportLuaError(int errorCode)
{
	// Pas d'erreur
	if (errorCode == 0)
		return errorCode;

	// Erreurs connus
	else if (errorCode == LUA_ERRSYNTAX)
		std::cerr << "Lua: Erreur de syntaxe (LUA_ERRSYNTAX)." << std::endl;

	else if (errorCode == LUA_ERRRUN)
		std::cerr << "Lua: Erreur de Runtime (LUA_ERRRUN)." << std::endl;

	else if (errorCode == LUA_ERRMEM)
		std::cerr << "Lua: Pas assez de mémoire (LUA_ERRMEM)." << std::endl;

	else if (errorCode == LUA_ERRERR)
		std::cerr << "Lua: Erreur intraitable (LUA_ERRERR)." << std::endl;

	else if (errorCode == LUA_ERRFILE)
		std::cerr << "Lua: Fichier introuvable (LUA_ERRFILE)." << std::endl;

	// Erreur inconnu
	else
		std::cerr << "Lua: Erreur inconnue (" << errorCode << ")." << std::endl;
	
	// Affiche l'erreur sur la pile
	std::cout << lua_tostring(mLuaState, -1) << std::endl;

	return errorCode;
}
