#include "JointFactory.h"
#include "Joints\DistanceJoint.h"
#include "Joints\FrictionJoint.h"
#include "Joints\GearJoint.h"
#include "Joints\PrismaticJoint.h"
#include "Joints\PulleyJoint.h"
#include "Joints\RevoluteJoint.h"
#include "Joints\RopeJoint.h"
#include "Joints\WeldJoint.h"
#include "Joints\WheelJoint.h"

namespace JointFactory
{
	/* Crée un DistanceJoint */
	// Retourne l'ID du joint
	int CreateDistanceJoint(const DistanceJointDef &def)
	{
		auto j = new DistanceJoint(def);
		return j->GetID();
	}
	
	/* Crée un FrictionJoint */
	// Retourne l'ID du joint
	int CreateFrictionJoint(const FrictionJointDef &def)
	{
		auto j = new FrictionJoint(def);
		return j->GetID();
	}

	/* Crée un GearJoint */
	// Retourne l'ID du joint
	int CreateGearJoint(const GearJointDef &def)
	{
		auto j = new GearJoint(def);
		return j->GetID();
	}

	/* Crée un PrismaticJoint */
	// Retourne l'ID du joint
	int CreatePrismaticJoint(const PrismaticJointDef &def)
	{
		auto j = new PrismaticJoint(def);
		return j->GetID();
	}

	/* Crée un PulleyJoint */
	// Retourne l'ID du joint
	int CreatePulleyJoint(const PulleyJointDef &def)
	{
		auto j = new PulleyJoint(def);
		return j->GetID();
	}

	/* Crée un RevoluteJoint */
	// Retourne l'ID du joint
	int CreateRevoluteJoint(const RevoluteJointDef &def)
	{
		auto j = new RevoluteJoint(def);
		return j->GetID();
	}

	/* Crée un RopeJoint */
	// Retourne l'ID du joint
	int CreateRopeJoint(const RopeJointDef &def)
	{
		auto j = new RopeJoint(def);
		return j->GetID();
	}

	/* Crée un WeldJoint */
	// Retourne l'ID du joint
	int CreateWeldJoint(const WeldJointDef &def)
	{
		auto j = new WeldJoint(def);
		return j->GetID();
	}

	/* Crée un WheelJoint */
	// Retourne l'ID du joint
	int CreateWheelJoint(const WheelJointDef &def)
	{
		auto j = new WheelJoint(def);
		return j->GetID();
	}

} // namespace JointFactory
