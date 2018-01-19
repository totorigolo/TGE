#include "../ScriptMachine.h"
#include "../../Physics/JointFactory.h"
#include "../../Physics/Joints/DistanceJoint.h"
#include "../../Physics/Joints/FrictionJoint.h"
#include "../../Physics/Joints/GearJoint.h"
#include "../../Physics/Joints/PrismaticJoint.h"
#include "../../Physics/Joints/PulleyJoint.h"
#include "../../Physics/Joints/RevoluteJoint.h"
#include "../../Physics/Joints/RopeJoint.h"
#include "../../Physics/Joints/WeldJoint.h"
#include "../../Physics/Joints/WheelJoint.h"

// Enregistrements
void ScriptMachine::RegisterJointFactory()
{
	try
	{/*
		// Enregistre les structs
		luabind::module(mLuaState)
		[
			// DistanceJointDef
			luabind::class_<DistanceJointDef>("DistanceJointDef")
				// Ctors
				.def(luabind::constructor<>())
				.def(luabind::constructor<b2Body*, b2Vec2, b2Body*, b2Vec2, float, float, bool>())
				// Attributs
				.def_readwrite("body1", &DistanceJointDef::body1)
				.def_readwrite("point1", &DistanceJointDef::point1)
				.def_readwrite("body2", &DistanceJointDef::body2)
				.def_readwrite("point2", &DistanceJointDef::point2)
				.def_readwrite("frequencyHz", &DistanceJointDef::frequencyHz)
				.def_readwrite("damping", &DistanceJointDef::damping)
				.def_readwrite("collideconnected", &DistanceJointDef::collideconnected)
			,
			// FrictionJointDef
			luabind::class_<FrictionJointDef>("FrictionJointDef")
				// Ctors
				.def(luabind::constructor<>())
				.def(luabind::constructor<b2Body*, b2Vec2, b2Body*, b2Vec2, float, float, bool>())
				// Attributs
				.def_readwrite("body1", &FrictionJointDef::body1)
				.def_readwrite("point1", &FrictionJointDef::point1)
				.def_readwrite("body2", &FrictionJointDef::body2)
				.def_readwrite("point2", &FrictionJointDef::point2)
				.def_readwrite("maxForce", &FrictionJointDef::maxForce)
				.def_readwrite("maxTorque", &FrictionJointDef::maxTorque)
				.def_readwrite("collideconnected", &FrictionJointDef::collideconnected)
			,
			// GearJointDef
			luabind::class_<GearJointDef>("GearJointDef")
				// Ctors
				.def(luabind::constructor<>())
				.def(luabind::constructor<b2Body*, b2Body*, int, int, float, bool>())
				// Attributs
				.def_readwrite("body1", &GearJointDef::body1)
				.def_readwrite("body2", &GearJointDef::body2)
				.def_readwrite("joint1", &GearJointDef::joint1)
				.def_readwrite("joint2", &GearJointDef::joint2)
				.def_readwrite("ratio", &GearJointDef::ratio)
				.def_readwrite("collideconnected", &GearJointDef::collideconnected)
			,
			// PrismaticJointDef
			// Pas tous les paramètres du ctor sont fournis car il y en a trop
			luabind::class_<PrismaticJointDef>("PrismaticJointDef")
				// Ctors
				.def(luabind::constructor<>())
				.def(luabind::constructor<b2Body*, b2Body*, b2Vec2, b2Vec2>())
				// Attributs
				.def_readwrite("body1", &PrismaticJointDef::body1)
				.def_readwrite("body2", &PrismaticJointDef::body2)
				.def_readwrite("anchor", &PrismaticJointDef::anchor)
				.def_readwrite("axis", &PrismaticJointDef::axis)
				.def_readwrite("enableLimit", &PrismaticJointDef::enableLimit)
				.def_readwrite("lowerTranslation", &PrismaticJointDef::lowerTranslation)
				.def_readwrite("upperTranslation", &PrismaticJointDef::upperTranslation)
				.def_readwrite("enableMotor", &PrismaticJointDef::enableMotor)
				.def_readwrite("motorSpeed", &PrismaticJointDef::motorSpeed)
				.def_readwrite("maxMotorForce", &PrismaticJointDef::maxMotorForce)
				.def_readwrite("collideconnected", &PrismaticJointDef::collideconnected)
			,
			// PulleyJointDef
			luabind::class_<PulleyJointDef>("PulleyJointDef")
				// Ctors
				.def(luabind::constructor<>())
				.def(luabind::constructor<b2Body*, b2Vec2, b2Body*, b2Vec2, b2Vec2, b2Vec2, float, bool>())
				// Attributs
				.def_readwrite("body1", &PulleyJointDef::body1)
				.def_readwrite("point1", &PulleyJointDef::point1)
				.def_readwrite("body2", &PulleyJointDef::body2)
				.def_readwrite("point2", &PulleyJointDef::point2)
				.def_readwrite("groundP1", &PulleyJointDef::groundP1)
				.def_readwrite("groundP2", &PulleyJointDef::groundP2)
				.def_readwrite("ratio", &PulleyJointDef::ratio)
				.def_readwrite("collideconnected", &PulleyJointDef::collideconnected)
			,
			// RevoluteJointDef
			// Pas tous les paramètres du ctor sont fournis car il y en a trop
			luabind::class_<RevoluteJointDef>("RevoluteJointDef")
				// Ctors
				.def(luabind::constructor<>())
				.def(luabind::constructor<b2Body*, b2Body*, b2Vec2>())
				// Attributs
				.def_readwrite("body1", &RevoluteJointDef::body1)
				.def_readwrite("body2", &RevoluteJointDef::body2)
				.def_readwrite("anchor", &RevoluteJointDef::anchor)
				.def_readwrite("enableLimit", &RevoluteJointDef::enableLimit)
				.def_readwrite("lowerAngle", &RevoluteJointDef::lowerAngle)
				.def_readwrite("upperAngle", &RevoluteJointDef::upperAngle)
				.def_readwrite("enableMotor", &RevoluteJointDef::enableMotor)
				.def_readwrite("motorSpeed", &RevoluteJointDef::motorSpeed)
				.def_readwrite("maxMotorTorque", &RevoluteJointDef::maxMotorTorque)
				.def_readwrite("collideconnected", &RevoluteJointDef::collideconnected)
			,
			// RopeJointDef
			luabind::class_<RopeJointDef>("RopeJointDef")
				// Ctors
				.def(luabind::constructor<>())
				.def(luabind::constructor<b2Body*, b2Vec2, b2Body*, b2Vec2, float, bool>())
				// Attributs
				.def_readwrite("body1", &RopeJointDef::body1)
				.def_readwrite("point1", &RopeJointDef::point1)
				.def_readwrite("body2", &RopeJointDef::body2)
				.def_readwrite("point2", &RopeJointDef::point2)
				.def_readwrite("maxLength", &RopeJointDef::maxLength)
				.def_readwrite("collideconnected", &RopeJointDef::collideconnected)
			,
			// WeldJointDef
			luabind::class_<WeldJointDef>("WeldJointDef")
				// Ctors
				.def(luabind::constructor<>())
				.def(luabind::constructor<b2Body*, b2Body*, b2Vec2, float, float, bool>())
				// Attributs
				.def_readwrite("body1", &WeldJointDef::body1)
				.def_readwrite("body2", &WeldJointDef::body2)
				.def_readwrite("anchor", &WeldJointDef::anchor)
				.def_readwrite("frequencyHz", &WeldJointDef::frequencyHz)
				.def_readwrite("damping", &WeldJointDef::damping)
				.def_readwrite("collideconnected", &WeldJointDef::collideconnected)
			,
			// WheelJointDef
			luabind::class_<WheelJointDef>("WheelJointDef")
				// Ctors
				.def(luabind::constructor<>())
				.def(luabind::constructor<b2Body*, b2Body*, b2Vec2, b2Vec2>())
				// Attributs
				.def_readwrite("car", &WheelJointDef::car)
				.def_readwrite("wheel", &WheelJointDef::wheel)
				.def_readwrite("pWheel", &WheelJointDef::pWheel)
				.def_readwrite("axis", &WheelJointDef::axis)
				.def_readwrite("frequencyHz", &WheelJointDef::frequencyHz)
				.def_readwrite("damping", &WheelJointDef::damping)
				.def_readwrite("enableMotor", &WheelJointDef::enableMotor)
				.def_readwrite("motorSpeed", &WheelJointDef::motorSpeed)
				.def_readwrite("maxMotorTorque", &WheelJointDef::maxMotorTorque)
				.def_readwrite("collideconnected", &WheelJointDef::collideconnected)
		];

		// Enregistre les fonctions de la Factory
		luabind::module(mLuaState) [
			luabind::namespace_("JointFactory") [
				// CreateDistanceJoint
				luabind::def("CreateDistanceJoint", JointFactory::CreateDistanceJoint),
				// CreateFrictionJoint
				luabind::def("CreateFrictionJoint", JointFactory::CreateFrictionJoint),
				// CreateGearJoint
				luabind::def("CreateGearJoint", JointFactory::CreateGearJoint),
				// CreatePrismaticJoint
				luabind::def("CreatePrismaticJoint", JointFactory::CreatePrismaticJoint),
				// CreatePulleyJoint
				luabind::def("CreatePulleyJoint", JointFactory::CreatePulleyJoint),
				// CreateRevoluteJoint
				luabind::def("CreateRevoluteJoint", JointFactory::CreateRevoluteJoint),
				// CreateRopeJoint
				luabind::def("CreateRopeJoint", JointFactory::CreateRopeJoint),
				// CreateWeldJoint
				luabind::def("CreateWeldJoint", JointFactory::CreateWeldJoint),
				// CreateWheelJoint
				luabind::def("CreateWeldJoint", JointFactory::CreateWeldJoint)
			]
		];*/
	}
	catch (const std::exception &e)
	{
		std::cerr << e.what() << std::endl;
	}
}