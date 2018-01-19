#include "../ScriptMachine.h"
#include "../../Physics/PhysicManager.h"
#include "../../Physics/Joint.h"

// Enregistrements
void ScriptMachine::RegisterPhysicManager()
{
	try
	{/*
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
				.property("MPP", &PhysicManager::GetMPP, &PhysicManager::SetMPP)
				// Joints
				.def("NameJoint", &PhysicManager::Name)
				.def("AnonymizeJoint", &PhysicManager::Anonymize)
				.def("GetJointName", &PhysicManager::GetName)
				.def("JointExists", (bool(PhysicManager::*)(int) const)&PhysicManager::JointExists)
				.def("JointExists", (bool(PhysicManager::*)(const std::string&) const)&PhysicManager::JointExists)
				.def("GetJoint", (Joint*(PhysicManager::*)(int))&PhysicManager::GetJoint, luabind::dependency(_1, luabind::result))
				.def("GetJoint", (Joint*(PhysicManager::*)(const std::string&))&PhysicManager::GetJoint)
		];*/
	}
	catch (const std::exception &e)
	{
		std::cerr << e.what() << std::endl;
	}
}
