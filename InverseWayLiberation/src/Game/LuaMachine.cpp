#include "LuaMachine.h"
#include "Box2DGame.h"
#include "InputManager.h"
#include "../Level/LevelManager.h"
#include "../Physics/JointFactory.h"
#include "../Physics/PhysicManager.h"
#include "../Entities/EntityFactory.h"
#include "../Entities/Player.h"
#include <iostream>
#include <exception>
#include <Box2D/Box2D.h>
#include <SFML/Graphics.hpp>
#include <luabind/operator.hpp>

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
			// Fxs
			luabind::def("LoadTexture", EntityFactory::LoadTexture),

			// Fxs dans EntityFactory
			luabind::namespace_("EntityFactory") [
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
				luabind::def("CreateRagdoll", (void(*)(const b2Vec2&)) EntityFactory::CreateRagdoll),
				luabind::def("CreateRagdoll", (void(*)(const b2Vec2&, int)) EntityFactory::CreateRagdoll)
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
void LuaMachine::RegisterLevelManager()
{
	try
	{
		// Enregistrement
		luabind::module(mLuaState) [
			// Level
			luabind::class_<LevelManager>("LevelManager")
				// Fxs
				.def("LoadFromFile", &LevelManager::LoadFromFile)
				.def("GetPlayer", &LevelManager::GetPlayer, luabind::dependency(luabind::result, _1))
				.def("PrepareForGame", &LevelManager::PrepareForGame)
				.def("Clear", &LevelManager::Clear)
				// Attributs
				.property("charged", &LevelManager::IsCharged, &LevelManager::SetCharged)
				.property("bckgcolor", &LevelManager::GetBckgColor, &LevelManager::SetBckgColor)
		];
	}
	catch (const std::exception &e)
	{
		std::cerr << e.what() << std::endl;
	}
}
void LuaMachine::RegisterPhysicManager()
{
	try
	{
		// Enregistrement
		luabind::module(mLuaState) [
			// Level
			luabind::class_<PhysicManager>("PhysicManager")
				// Fxs
				.def("DestroyAllBody", &PhysicManager::DestroyAllBody)
				.def("DestroyBodiesOut", &PhysicManager::DestroyBodiesOut)
				.def("DestroyAllJoints", &PhysicManager::DestroyAllJoints)
				.def("GetBodyCount", &PhysicManager::GetBodyCount)
				.def("GetJointCount", &PhysicManager::GetJointCount)
				// Attributs
				.property("timestep", &PhysicManager::GetTimeStep, &PhysicManager::SetTimeStep)
				.property("gravity", &PhysicManager::GetGravity, &PhysicManager::SetGravity)
				.property("PPM", &PhysicManager::GetPPM, &PhysicManager::SetPPM)
		];
	}
	catch (const std::exception &e)
	{
		std::cerr << e.what() << std::endl;
	}
}
void LuaMachine::RegisterInputManager()
{
	try
	{
		// Enregistrement
		luabind::module(mLuaState) [
			// Level
			luabind::class_<InputManager>("InputManager")
				// Enum
				.enum_("KeyState")
				[
					luabind::value("None", KeyState::None),
					luabind::value("Pressed", KeyState::Pressed),
					luabind::value("Released", KeyState::Released)
				]
				/* Fxs */
				// Vue et zoom
				.def("SetZoom", &InputManager::SetZoom)
				.def("SetCenter", &InputManager::SetCenter)
				.def("GetCurrentZoom", &InputManager::GetCurrentZoom)
				// Evènements
				.def("IsKeyPressed", &InputManager::IsKeyPressed)
				.def("GetMousePos", &InputManager::GetMousePos)
				.def("GetMousePosRV", &InputManager::GetMousePosRV)
				.def("GetLastMousePos", &InputManager::GetLastMousePos)
				.def("GetLastMousePosRV", &InputManager::GetLastMousePosRV)
				.def("GetLMBState", &InputManager::GetLMBState)
				.def("GetRMBState", &InputManager::GetRMBState)
				.def("GetMMBState", &InputManager::GetMMBState)
				// Espionnage des touches
				.def("AddSpyedKey", &InputManager::AddSpyedKey)
				.def("RemoveSpyedKey", &InputManager::RemoveSpyedKey)
				.def("KeyPressed", &InputManager::KeyPressed)
				.def("KeyReleased", &InputManager::KeyReleased)
				.def("GetSpyedKeyState", &InputManager::GetSpyedKeyState)
				// Attributs
				.property("defaultZoom", &InputManager::GetDefaultZoom, &InputManager::SetDefaultZoom)
				.property("defaultCenter", &InputManager::GetDefaultCenter, &InputManager::SetDefaultCenter)
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
#ifdef _DEBUG
	sf::Clock c;
	int r = ReportLuaError(luaL_dofile(mLuaState, path.c_str()));
	std::cout << "Script \"" << path << "\" ex\x82""cut\x82 en : "
		<< c.getElapsedTime().asSeconds() << " sec" << std::endl;
	return r;
#else
	return ReportLuaError(luaL_dofile(mLuaState, path.c_str()));
#endif
}
int LuaMachine::LoadFile(const std::string &path)
{
#ifdef _DEBUG
	sf::Clock c;
	int r = ReportLuaError(luaL_loadfile(mLuaState, path.c_str()));
	std::cout << "Script \"" << path << "\" ex\x82""cut\x82 en : "
		<< c.getElapsedTime().asSeconds() << " sec" << std::endl;
	return r;
#else
	return ReportLuaError(luaL_loadfile(mLuaState, path.c_str()));
#endif
}
int LuaMachine::DoString(const std::string &command)
{
#ifdef _DEBUG
	sf::Clock c;
	int r = ReportLuaError(luaL_dostring(mLuaState, command.c_str()));
	std::cout << "Commande \"" << command << "\" ex\x82""cut\x82 en : "
		<< c.getElapsedTime().asSeconds() << " sec" << std::endl;
	return r;
#else
	return ReportLuaError(luaL_dostring(mLuaState, command.c_str()));
#endif
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

		// sf::Keyboard::Key
		luabind::module(mLuaState) [
			// La classe
			luabind::class_<sf::Keyboard>("Keyboard")
				// Enum
				.enum_("Key")
				[
					luabind::value("None", 1),
					luabind::value("Unknown", sf::Keyboard::Unknown), ///< Unhandled key
					luabind::value("A", sf::Keyboard::A), ///< The A key
					luabind::value("B", sf::Keyboard::B), ///< The B key
					luabind::value("C", sf::Keyboard::C), ///< The C key
					luabind::value("D", sf::Keyboard::D), ///< The D key
					luabind::value("E", sf::Keyboard::E), ///< The E key
					luabind::value("F", sf::Keyboard::F), ///< The F key
					luabind::value("G", sf::Keyboard::G), ///< The G key
					luabind::value("H", sf::Keyboard::H), ///< The H key
					luabind::value("I", sf::Keyboard::I), ///< The I key
					luabind::value("J", sf::Keyboard::J), ///< The J key
					luabind::value("K", sf::Keyboard::K), ///< The K key
					luabind::value("L", sf::Keyboard::L), ///< The L key
					luabind::value("M", sf::Keyboard::M), ///< The M key
					luabind::value("N", sf::Keyboard::N), ///< The N key
					luabind::value("O", sf::Keyboard::O), ///< The O key
					luabind::value("P", sf::Keyboard::P), ///< The P key
					luabind::value("Q", sf::Keyboard::Q), ///< The Q key
					luabind::value("R", sf::Keyboard::R), ///< The R key
					luabind::value("S", sf::Keyboard::S), ///< The S key
					luabind::value("T", sf::Keyboard::T), ///< The T key
					luabind::value("U", sf::Keyboard::U), ///< The U key
					luabind::value("V", sf::Keyboard::V), ///< The V key
					luabind::value("W", sf::Keyboard::W), ///< The W key
					luabind::value("X", sf::Keyboard::X), ///< The X key
					luabind::value("Y", sf::Keyboard::Y), ///< The Y key
					luabind::value("Z", sf::Keyboard::Z), ///< The Z key
					luabind::value("Num0", sf::Keyboard::Num0), ///< The 0 key
					luabind::value("Num1", sf::Keyboard::Num1), ///< The 1 key
					luabind::value("Num2", sf::Keyboard::Num2), ///< The 2 key
					luabind::value("Num3", sf::Keyboard::Num3), ///< The 3 key
					luabind::value("Num4", sf::Keyboard::Num4), ///< The 4 key
					luabind::value("Num5", sf::Keyboard::Num5), ///< The 5 key
					luabind::value("Num6", sf::Keyboard::Num6), ///< The 6 key
					luabind::value("Num7", sf::Keyboard::Num7), ///< The 7 key
					luabind::value("Num8", sf::Keyboard::Num8), ///< The 8 key
					luabind::value("Num9", sf::Keyboard::Num9), ///< The 9 key
					luabind::value("Escap", sf::Keyboard::Escape), ///< The Escape key
					luabind::value("LControl", sf::Keyboard::LControl), ///< The left Control key
					luabind::value("LShift", sf::Keyboard::LShift), ///< The left Shift key
					luabind::value("LAlt", sf::Keyboard::LAlt), ///< The left Alt key
					luabind::value("LSystem", sf::Keyboard::LSystem), ///< The left OS specific key: window (Windows and Linux), apple (MacOS X), ...
					luabind::value("RControl", sf::Keyboard::RControl), ///< The right Control key
					luabind::value("RShift", sf::Keyboard::RShift), ///< The right Shift key
					luabind::value("RAlt", sf::Keyboard::RAlt), ///< The right Alt key
					luabind::value("RSystem", sf::Keyboard::RSystem), ///< The right OS specific key: window (Windows and Linux), apple (MacOS X), ...
					luabind::value("Menu", sf::Keyboard::Menu), ///< The Menu key
					luabind::value("LBracket", sf::Keyboard::LBracket), ///< The [ key
					luabind::value("RBracket", sf::Keyboard::RBracket), ///< The ] key
					luabind::value("SemiColon", sf::Keyboard::SemiColon), ///< The ; key
					luabind::value("Comma", sf::Keyboard::Comma), ///< The , key
					luabind::value("Period", sf::Keyboard::Period), ///< The . key
					luabind::value("Quote", sf::Keyboard::Quote), ///< The ' key
					luabind::value("Slash", sf::Keyboard::Slash), ///< The / key
					luabind::value("BackSlash", sf::Keyboard::BackSlash), ///< The \ key
					luabind::value("Tilde", sf::Keyboard::Tilde), ///< The ~ key
					luabind::value("Equal", sf::Keyboard::Equal), ///< The = key
					luabind::value("Dash", sf::Keyboard::Dash), ///< The - key
					luabind::value("Space", sf::Keyboard::Space), ///< The Space key
					luabind::value("Return", sf::Keyboard::Return), ///< The Return key
					luabind::value("BackSpace", sf::Keyboard::BackSpace), ///< The Backspace key
					luabind::value("Tab", sf::Keyboard::Tab), ///< The Tabulation key
					luabind::value("PageUp", sf::Keyboard::PageUp), ///< The Page up key
					luabind::value("PageDown", sf::Keyboard::PageDown), ///< The Page down key
					luabind::value("End", sf::Keyboard::End), ///< The End key
					luabind::value("Home", sf::Keyboard::Home), ///< The Home key
					luabind::value("Insert", sf::Keyboard::Insert), ///< The Insert key
					luabind::value("Delete", sf::Keyboard::Delete), ///< The Delete key
					luabind::value("Add", sf::Keyboard::Add), ///< +
					luabind::value("Subtract", sf::Keyboard::Subtract), ///< -
					luabind::value("Multiply", sf::Keyboard::Multiply), ///< *
					luabind::value("Divide", sf::Keyboard::Divide), ///< /
					luabind::value("Left", sf::Keyboard::Left), ///< Left arrow
					luabind::value("Right", sf::Keyboard::Right), ///< Right arrow
					luabind::value("Up", sf::Keyboard::Up), ///< Up arrow
					luabind::value("Down", sf::Keyboard::Down), ///< Down arrow
					luabind::value("Numpad0", sf::Keyboard::Numpad0), ///< The numpad 0 key
					luabind::value("Numpad1", sf::Keyboard::Numpad1), ///< The numpad 1 key
					luabind::value("Numpad2", sf::Keyboard::Numpad2), ///< The numpad 2 key
					luabind::value("Numpad3", sf::Keyboard::Numpad3), ///< The numpad 3 key
					luabind::value("Numpad4", sf::Keyboard::Numpad4), ///< The numpad 4 key
					luabind::value("Numpad5", sf::Keyboard::Numpad5), ///< The numpad 5 key
					luabind::value("Numpad6", sf::Keyboard::Numpad6), ///< The numpad 6 key
					luabind::value("Numpad7", sf::Keyboard::Numpad7), ///< The numpad 7 key
					luabind::value("Numpad8", sf::Keyboard::Numpad8), ///< The numpad 8 key
					luabind::value("Numpad9", sf::Keyboard::Numpad9), ///< The numpad 9 key
					luabind::value("F1", sf::Keyboard::F1), ///< The F1 key
					luabind::value("F2", sf::Keyboard::F2), ///< The F2 key
					luabind::value("F3", sf::Keyboard::F3), ///< The F3 key
					luabind::value("F4", sf::Keyboard::F4), ///< The F4 key
					luabind::value("F5", sf::Keyboard::F5), ///< The F5 key
					luabind::value("F6", sf::Keyboard::F6), ///< The F6 key
					luabind::value("F7", sf::Keyboard::F7), ///< The F7 key
					luabind::value("F8", sf::Keyboard::F8), ///< The F8 key
					luabind::value("F9", sf::Keyboard::F9), ///< The F9 key
					luabind::value("F10", sf::Keyboard::F10), ///< The F10 key
					luabind::value("F11", sf::Keyboard::F11), ///< The F11 key
					luabind::value("F12", sf::Keyboard::F12), ///< The F12 key
					luabind::value("F13", sf::Keyboard::F13), ///< The F13 key
					luabind::value("F14", sf::Keyboard::F14), ///< The F14 key
					luabind::value("F15", sf::Keyboard::F15), ///< The F15 key
					luabind::value("Pause", sf::Keyboard::Pause), ///< The Pause key

					luabind::value("KeyCount", sf::Keyboard::KeyCount) ///< Keep last -- the total number of keyboard keys
				]
				// Fxs
			.def("isKeyPressed", &sf::Keyboard::isKeyPressed)
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
JointFactory.CreatePulleyJoint(1, b2Vec2(0.f, 0.f), 2, b2Vec2(0.f, 0.f), b2Vec2(-2.f, 0.5f), b2Vec2(1.9f, 4.f), 1.5f)
JointFactory.CreateRopeJoint(3, b2Vec2(0.f, 0.f), 4, b2Vec2(0.f, 0.f), 1.f, true, (0, 255, 255))
*/
