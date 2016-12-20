#pragma once

struct DistanceJointDef;
struct FrictionJointDef;
struct GearJointDef;
struct PrismaticJointDef;
struct PulleyJointDef;
struct RevoluteJointDef;
struct RopeJointDef;
struct WeldJointDef;
struct WheelJointDef;
namespace JointFactory
{
	/* Crée un DistanceJoint */
	// Retourne l'ID du joint
	int CreateDistanceJoint(const DistanceJointDef &def);
	
	/* Crée un FrictionJoint */
	// Retourne l'ID du joint
	int CreateFrictionJoint(const FrictionJointDef &def);

	/* Crée un GearJoint */
	// Retourne l'ID du joint
	int CreateGearJoint(const GearJointDef &def);

	/* Crée un PrismaticJoint */
	// Retourne l'ID du joint
	int CreatePrismaticJoint(const PrismaticJointDef &def);

	/* Crée un PulleyJoint */
	// Retourne l'ID du joint
	int CreatePulleyJoint(const PulleyJointDef &def);

	/* Crée un RevoluteJoint */
	// Retourne l'ID du joint
	int CreateRevoluteJoint(const RevoluteJointDef &def);

	/* Crée un RopeJoint */
	// Retourne l'ID du joint
	int CreateRopeJoint(const RopeJointDef &def);

	/* Crée un WeldJoint */
	// Retourne l'ID du joint
	int CreateWeldJoint(const WeldJointDef &def);

	/* Crée un WheelJoint */
	// Retourne l'ID du joint
	int CreateWheelJoint(const WheelJointDef &def);

};
