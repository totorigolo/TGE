#include "../LuaMachine.h"
#include "../../Entities/EntityFactory.h"

#include <Box2D/Box2D.h>
#include <exception>

void LuaMachine::RegisterEntityFactory()
{
	try
	{
		// Enregistre les fonctions
		luabind::module(mLuaState) [
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
