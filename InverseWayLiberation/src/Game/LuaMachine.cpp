#include "LuaMachine.h"
#include "Box2DGame.h"
#include "../Level/Level.h"
#include "../Entities/EntityFactory.h"
#include "../Physics/JointFactory.h"
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
				luabind::def("LoadTexture", EntityFactory::LoadTexture),
				luabind::def("CreateDeco", (void(*)(const b2Vec3&, const std::string&)) EntityFactory::CreateDeco),
				luabind::def("CreateDeco", (void(*)(const b2Vec3&, const std::string&, int)) EntityFactory::CreateDeco),
				luabind::def("CreateDynamicBox", (void(*)(const b2Vec3&, const std::string&)) EntityFactory::CreateDynamicBox),
				luabind::def("CreateDynamicBox", (void(*)(const b2Vec3&, const std::string&, int)) EntityFactory::CreateDynamicBox),
				luabind::def("CreateStaticBox", (void(*)(const b2Vec3&, const std::string&)) EntityFactory::CreateStaticBox),
				luabind::def("CreateStaticBox", (void(*)(const b2Vec3&, const std::string&, int)) EntityFactory::CreateStaticBox),
				luabind::def("CreateLamp", (void(*)(const b2Vec3&, const std::string&)) EntityFactory::CreateLamp),
				luabind::def("CreateLamp", (void(*)(const b2Vec3&, const std::string&, int)) EntityFactory::CreateLamp),
				luabind::def("CreateDynamicCircle", (void(*)(const b2Vec3&, const std::string&)) EntityFactory::CreateDynamicCircle),
				luabind::def("CreateDynamicCircle", (void(*)(const b2Vec3&, const std::string&, int)) EntityFactory::CreateDynamicCircle),
				luabind::def("CreateRagdoll", (void(*)(const b2Vec3&)) EntityFactory::CreateRagdoll),
				luabind::def("CreateRagdoll", (void(*)(const b2Vec3&, int)) EntityFactory::CreateRagdoll)
			]
		];
	}
	catch (const std::exception &e)
	{
		std::cerr << e.what() << std::endl;
	}
}
void LuaMachine::RegisterJointFactory()
{
	/*
	// Crée un DistanceJoint
	void CreateDistanceJoint(b2Body *b1, b2Vec2 pt1, b2Body *b2, b2Vec2 pt2, float frequencyHz = 4.f, float damping = 0.5f, bool collideconnected = true);
	*/
	/*
	try
	{
		// Enregistre les fonctions
		luabind::module(mLuaState) [
			luabind::namespace_("JointFactory") [
				luabind::def("CreateDistanceJoint", (void(*)(const b2Vec3&, const std::string&)) EntityFactory::CreateDistanceJoint),
				luabind::def("CreateDistanceJoint", (void(*)(const b2Vec3&, const std::string&, int)) EntityFactory::CreateDistanceJoint),
			]
		];
	}
	catch (const std::exception &e)
	{
		std::cerr << e.what() << std::endl;
	}*/
}
void LuaMachine::RegisterLevel()
{
	try
	{
		// Enregistrement
		luabind::module(mLuaState) [
			// Level
			luabind::class_<Level>("Level")
				// Fxs
				.def("LoadFromFile", &Level::LoadFromFile)
				.def("SetViewPosition", &Level::SetViewPosition)
				.def("SetZoom", &Level::SetZoom)
				.def("GetPlayer", &Level::GetPlayer)
				// Attributs
				.property("charged", &Level::IsCharged, &Level::SetCharged)
				.property("gravity", &Level::GetGravity, &Level::SetGravity)
				.property("PPM", &Level::GetPPM, &Level::SetPPM)
				.property("bckgcolor", &Level::GetBckgColor, &Level::SetBckgColor)
				.property("originview", &Level::GetOriginView, &Level::SetOriginView)
				.property("defaultzoom", &Level::GetDefaultZoom, &Level::SetDefaultZoom)
		];
	}
	catch (const std::exception &e)
	{
		std::cerr << e.what() << std::endl;
	}
}

// Enregistrement des attributs
void LuaMachine::UnregisterGlobalLuaVar(const std::string &name)
{
	luabind::globals(mLuaState)[name] = luabind::nil;
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
	/* Enregistre les classes */
	try
	{
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
	/* Enregistre les classes */
	try
	{
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

		// sf::Color
		luabind::module(mLuaState) [
			// La classe
			luabind::class_<sf::Color>("Color")
				// Ctors
				.def(luabind::constructor<>())
				.def(luabind::constructor<sf::Uint8, sf::Uint8, sf::Uint8>())
				.def(luabind::constructor<sf::Uint8, sf::Uint8, sf::Uint8, sf::Uint8>())
				// Ops
				.def(luabind::const_self + sf::Color())
				.def(luabind::const_self * sf::Color())
				.def(luabind::const_self == sf::Color())
				// Attributs
				.def_readwrite("r", &sf::Color::r)
				.def_readwrite("g", &sf::Color::g)
				.def_readwrite("b", &sf::Color::b)
				.def_readwrite("a", &sf::Color::a)
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

/*
LoadTexture("way",			"tex/way.png")
JointFactory.CreatePulleyJoint(1, b2Vec2(0.f, 0.f), 2, b2Vec2(0.f, 0.f), b2Vec2(-2.f, 0.5f), b2Vec2(1.9f, 4.f), 1.5f)
JointFactory.CreateRopeJoint(3, b2Vec2(0.f, 0.f), 4, b2Vec2(0.f, 0.f), 1.f, true, (0, 255, 255))
*/