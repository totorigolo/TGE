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
	int CreateDistanceJoint(b2Body *b1, b2Vec2 pt1, b2Body *b2, b2Vec2 pt2, float frequencyHz, float damping, bool collideconnected)
	{
		auto j = new DistanceJoint(b1, pt1, b2, pt2, frequencyHz, damping, collideconnected);
		return j->GetID();
	}
	
	/* Crée un FrictionJoint */
	// Retourne l'ID du joint
	int CreateFrictionJoint(b2Body *b1, b2Vec2 pt1, b2Body *b2, b2Vec2 pt2, float maxForce, float maxTorque, bool collideconnected)
	{
		auto j = new FrictionJoint(b1, pt1, b2, pt2, maxForce, maxTorque, collideconnected);
		return j->GetID();
	}

	/* Crée un GearJoint */
	// Retourne l'ID du joint
	int CreateGearJoint(b2Body *b1, b2Body *b2, int j1, int j2, float ratio, bool collideconnected)
	{
		auto j = new GearJoint(b1, b2, j1, j2, ratio, collideconnected);
		return j->GetID();
	}

	/* Crée un PrismaticJoint */
	// Retourne l'ID du joint
	int CreatePrismaticJoint(b2Body *b1, b2Body *b2, b2Vec2 anchor, b2Vec2 axis, bool enableLimit, float lowerTranslation, float upperTranslation,
																				 bool enableMotor, float motorSpeed, float maxMotorForce,
																				 bool collideconnected)
	{
		auto j = new PrismaticJoint(b1, b2, anchor, axis, enableLimit, lowerTranslation, upperTranslation,
														  enableMotor, motorSpeed, maxMotorForce,
														  collideconnected);
		return j->GetID();
	}

	/* Crée un PulleyJoint */
	// Retourne l'ID du joint
	int CreatePulleyJoint(b2Body *b1, b2Vec2 pt1, b2Body *b2, b2Vec2 pt2, b2Vec2 groundPt1, b2Vec2 groundPt2, float ratio, bool collideconnected)
	{
		auto j = new PulleyJoint(b1, pt1, b2, pt2, groundPt1, groundPt2, ratio, collideconnected);
		return j->GetID();
	}

	/* Crée un RevoluteJoint */
	// Retourne l'ID du joint
	int CreateRevoluteJoint(b2Body *b1, b2Body *b2, b2Vec2 anchor, bool enableLimit, float lowerAngle, float upperAngle,
																   bool enableMotor, float motorSpeed, float maxMotorTorque,
																   bool collideconnected)
	{
		auto j = new RevoluteJoint(b1, b2, anchor, enableLimit, lowerAngle, upperAngle, enableMotor, motorSpeed, maxMotorTorque, collideconnected);
		return j->GetID();
	}

	/* Crée un RopeJoint */
	// Retourne l'ID du joint
	int CreateRopeJoint(b2Body *b1, b2Vec2 pt1, b2Body *b2, b2Vec2 pt2, float maxLength, bool collideconnected)
	{
		auto j = new RopeJoint(b1, pt1, b2, pt2, maxLength, collideconnected);
		return j->GetID();
	}

	/* Crée un WeldJoint */
	// Retourne l'ID du joint
	int CreateWeldJoint(b2Body *b1, b2Body *b2, b2Vec2 anchor, float frequencyHz, float damping, bool collideconnected)
	{
		auto j = new WeldJoint(b1, b2, anchor, frequencyHz, damping, collideconnected);
		return j->GetID();
	}

	/* Crée un WheelJoint */
	// Retourne l'ID du joint
	int CreateWheelJoint(b2Body *car, b2Body *wheel, b2Vec2 pWheel, b2Vec2 axis, float frequencyHz, float damping,
																				 bool enableMotor, float motorSpeed, float maxMotorTorque,
																				 bool collideconnected)
	{
		auto j = new WheelJoint(car, wheel, pWheel, axis, frequencyHz, damping, enableMotor, motorSpeed, maxMotorTorque, collideconnected);
		return j->GetID();
	}

} // namespace JointFactory
