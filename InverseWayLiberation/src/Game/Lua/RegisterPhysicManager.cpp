#include "../LuaMachine.h"
#include "../../Physics/PhysicManager.h"
#include <iostream>
#include <exception>

// Enregistrements
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
