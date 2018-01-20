#include "../ScriptMachine.h"
#include "../../Entities/EntityFactory.h"

using namespace luabind;

void ScriptMachine::RegisterEntityFactory()
{
	try
	{
		// Enregistre les fonctions
		module(mLuaState)[
			// PolyChain::Type
			class_<PolyChain>("PolyChain")
				.enum_("Type")
				[
					value("None", PolyChain::Type::None),
					value("Chain", PolyChain::Type::Chain),
					value("Loop", PolyChain::Type::Loop)
				],

			// Fxs dans EntityFactory
			namespace_("EntityFactory") [
				def("CreateDeco", (void(*)(const b2Vec3&, const std::string&)) EntityFactory::CreateDeco),
				def("CreateDeco", (void(*)(const b2Vec3&, const std::string&, int)) EntityFactory::CreateDeco),
				def("CreateBox", (void(*)(const b2Vec3&, b2BodyType, const std::string&)) EntityFactory::CreateBox),
				def("CreateBox", (void(*)(const b2Vec3&, b2BodyType, const std::string&, int)) EntityFactory::CreateBox),
				def("CreateCircle", (void(*)(const b2Vec3&, b2BodyType, const std::string&)) EntityFactory::CreateCircle),
				def("CreateCircle", (void(*)(const b2Vec3&, b2BodyType, const std::string&, int)) EntityFactory::CreateCircle),
				def("CreatePoly", (void(*)(const vector_b2Vec2&, b2BodyType, const std::string&)) EntityFactory::CreatePolyBody),
				def("CreatePoly", (void(*)(const vector_b2Vec2&, b2BodyType, const std::string&, int)) EntityFactory::CreatePolyBody),
				def("CreatePolyChain", (void(*)(const vector_b2Vec2&, PolyChain::Type, const std::string&)) EntityFactory::CreatePolyChain),
				def("CreatePolyChain", (void(*)(const vector_b2Vec2&, PolyChain::Type, const std::string&, int)) EntityFactory::CreatePolyChain),
				def("CreatePointLight", (void(*)(const b2Vec2&)) EntityFactory::CreatePointLight),
				def("CreatePointLight", (void(*)(const b2Vec2&, int)) EntityFactory::CreatePointLight),
				def("CreatePointLight", (void(*)(const b2Vec2&, int, const sf::Color&)) EntityFactory::CreatePointLight),
				def("CreatePointLight", (void(*)(const b2Vec2&, int, const sf::Color&, int)) EntityFactory::CreatePointLight),
				def("CreateRagdoll", (void(*)(const b2Vec2&)) EntityFactory::CreateRagdoll),
				def("CreateRagdoll", (void(*)(const b2Vec2&, int)) EntityFactory::CreateRagdoll),
				def("CreateHum", (void(*)(const b2Vec2&)) EntityFactory::CreateHum),
				def("CreateHum", (void(*)(const b2Vec2&, unsigned int)) EntityFactory::CreateHum),
				def("CreateHum", (void(*)(const b2Vec2&, unsigned int, int)) EntityFactory::CreateHum),
				def("CreateHum", (void(*)(const b2Vec2&, unsigned int, int, int)) EntityFactory::CreateHum),
				def("CreatePlayer", (void(*)(const b2Vec2&)) EntityFactory::CreatePlayer),
				def("CreatePlayer", (void(*)(const b2Vec2&, unsigned int)) EntityFactory::CreatePlayer),
				def("CreatePlayer", (void(*)(const b2Vec2&, unsigned int, int)) EntityFactory::CreatePlayer)
			]
		];
	}
	catch (const std::exception &e)
	{
		std::cerr << e.what() << std::endl;
	}
}
