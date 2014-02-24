#include "stdafx.h"
#include "../LuaMachine.h"
#include "../../Entities/EntityFactory.h"

using namespace luabind;

void LuaMachine::RegisterEntityFactory()
{
	try
	{
		// Enregistre les fonctions
		module(mLuaState) [
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
				def("CreateLamp", (void(*)(const b2Vec3&, const std::string&)) EntityFactory::CreateLamp),
				def("CreateLamp", (void(*)(const b2Vec3&, const std::string&, int)) EntityFactory::CreateLamp),
				def("CreateRagdoll", (void(*)(const b2Vec2&)) EntityFactory::CreateRagdoll),
				def("CreateRagdoll", (void(*)(const b2Vec2&, int)) EntityFactory::CreateRagdoll)
			]
		];
	}
	catch (const std::exception &e)
	{
		std::cerr << e.what() << std::endl;
	}
}
