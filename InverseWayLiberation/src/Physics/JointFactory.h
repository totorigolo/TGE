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
	/* Cr�e un DistanceJoint */
	// Retourne l'ID du joint
	int CreateDistanceJoint(const DistanceJointDef &def);
	
	/* Cr�e un FrictionJoint */
	// Retourne l'ID du joint
	int CreateFrictionJoint(const FrictionJointDef &def);

	/* Cr�e un GearJoint */
	// Retourne l'ID du joint
	int CreateGearJoint(const GearJointDef &def);

	/* Cr�e un PrismaticJoint */
	// Retourne l'ID du joint
	int CreatePrismaticJoint(const PrismaticJointDef &def);

	/* Cr�e un PulleyJoint */
	// Retourne l'ID du joint
	int CreatePulleyJoint(const PulleyJointDef &def);

	/* Cr�e un RevoluteJoint */
	// Retourne l'ID du joint
	int CreateRevoluteJoint(const RevoluteJointDef &def);

	/* Cr�e un RopeJoint */
	// Retourne l'ID du joint
	int CreateRopeJoint(const RopeJointDef &def);

	/* Cr�e un WeldJoint */
	// Retourne l'ID du joint
	int CreateWeldJoint(const WeldJointDef &def);

	/* Cr�e un WheelJoint */
	// Retourne l'ID du joint
	int CreateWheelJoint(const WheelJointDef &def);

};
