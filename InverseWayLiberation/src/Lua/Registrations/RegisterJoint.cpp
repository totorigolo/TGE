#include "../LuaMachine.h"
#include "../../Physics/Joint.h"

// Enregistrements privés
void LuaMachine::RegisterJoint()
{
	/* Enregistre les classes */
	try
	{/*
		// ForceType
		luabind::globals(mLuaState)["JointForceType_Null"] = ForceType::Null;
		luabind::globals(mLuaState)["JointForceType_Float"] = ForceType::Float;
		luabind::globals(mLuaState)["JointForceType_Vector"] = ForceType::Vector;
		
		// JointType
		luabind::globals(mLuaState)["JointType_Null"] = JointType::Null;
		luabind::globals(mLuaState)["JointType_DistanceJoint"] = JointType::DistanceJoint;
		luabind::globals(mLuaState)["JointType_FrictionJoint"] = JointType::FrictionJoint;
		luabind::globals(mLuaState)["JointType_GearJoint"] = JointType::GearJoint;
		luabind::globals(mLuaState)["JointType_MouseJoint"] = JointType::MouseJoint;
		luabind::globals(mLuaState)["JointType_PrismaticJoint"] = JointType::PrismaticJoint;
		luabind::globals(mLuaState)["JointType_PulleyJoint"] = JointType::PulleyJoint;
		luabind::globals(mLuaState)["JointType_RevoluteJoint"] = JointType::RevoluteJoint;
		luabind::globals(mLuaState)["JointType_RopeJoint"] = JointType::RopeJoint;
		luabind::globals(mLuaState)["JointType_WeldJoint"] = JointType::WeldJoint;
		luabind::globals(mLuaState)["JointType_WheelJoint"] = JointType::WheelJoint;

		// JointDef
		luabind::module(mLuaState) [
			// La classe
			luabind::class_<JointDef>("JointDef")
				// Attributs
				.def_readwrite("isBreakableMaxForce", &JointDef::isBreakableMaxForce)
				.def_readwrite("maxForceType", &JointDef::maxForceType)
				.def_readwrite("maxForce", &JointDef::maxForce)
				.def_readwrite("maxVecForce", &JointDef::maxVecForce)
				.def_readwrite("isBreakableMaxTorque", &JointDef::isBreakableMaxTorque)
				.def_readwrite("maxTorque", &JointDef::maxTorque)
		];

		// Joint
		luabind::module(mLuaState)[
			// La classe
			luabind::class_<Joint>("Joint")
				// Ctors
				.def(luabind::constructor<>())
				.def(luabind::constructor<JointDef>())
				// Fxs
				.def("Destroy", &Joint::Destroy)
				.def("Update", &Joint::Update)
				.def("RegisterLinkedJoint", &Joint::RegisterLinkedJoint)
				.def("RemoveLinkedJoint", &Joint::RemoveLinkedJoint)
				.def("DestroyLinkedJoint", &Joint::DestroyLinkedJoint)
				.def("DestroyAllLinkedJoints", &Joint::DestroyAllLinkedJoints)
				.def("GetLinkedJoints", &Joint::GetLinkedJoints)
				.def("IsAlive", &Joint::IsAlive)
				.def("ToDestroy", &Joint::ToDestroy)
				.def("GetType", &Joint::GetType)
				.def("GetID", &Joint::GetID)
				.def("IsCollideConnected", &Joint::IsCollideConnected)
				.def("GetReactionForce", &Joint::GetReactionForce)
				.def("GetReactionTorque", &Joint::GetReactionTorque)
				// Attributs
				.property("BreakableByForce", &Joint::IsBreakableMaxForce, &Joint::SetBreakableByForce)
				.property("MaxForce", &Joint::GetMaxForce, (void(Joint::*)(float))&Joint::SetMaxForce)
				.property("BreakableMaxTorque", &Joint::IsBreakableMaxTorque, &Joint::SetBreakableByTorque)
				.property("MaxTorque", &Joint::GetMaxTorque, &Joint::SetMaxTorque)
		];*/
	}
	catch (const std::exception &e)
	{
		std::cerr << e.what() << std::endl;
	}
}
