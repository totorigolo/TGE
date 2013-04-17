#pragma once
#include <Box2D\Box2D.h>

namespace JointFactory
{
	/* Crée un DistanceJoint */
	// Retourne l'ID du joint
	int CreateDistanceJoint(b2Body *b1, b2Vec2 pt1, b2Body *b2, b2Vec2 pt2, float frequencyHz = 4.f, float damping = 0.5f, bool collideconnected = true);
	
	/* Crée un FrictionJoint */
	// Retourne l'ID du joint
	int CreateFrictionJoint(b2Body *b1, b2Vec2 pt1, b2Body *b2, b2Vec2 p2, float maxForce = 0.f, float maxTorque = 0.f, bool collideconnected = true);

	/* Crée un GearJoint */
	// Retourne l'ID du joint
	int CreateGearJoint(b2Body *b1, b2Body *b2, int j1, int j2 , float ratio = 1.f, bool collideconnected = true);

	/* Crée un PrismaticJoint */
	// Retourne l'ID du joint
	int CreatePrismaticJoint(b2Body *b1, b2Body *b2, b2Vec2 anchor, b2Vec2 axis,
							bool enableLimit = false, float lowerTranslation = 0.f, float upperTranslation = 0.f,
							bool enableMotor = false, float motorSpeed = 0.f, float maxMotorForce = 10.f,
							bool collideconnected = false);

	/* Crée un PulleyJoint */
	// Retourne l'ID du joint
	int CreatePulleyJoint(b2Body *b1, b2Vec2 pt1, b2Body *b2, b2Vec2 p2, b2Vec2 groundP1, b2Vec2 groundP2, float ratio = 1.f, bool collideconnected = true);

	/* Crée un RevoluteJoint */
	// Retourne l'ID du joint
	int CreateRevoluteJoint(b2Body *b1, b2Body *b2, b2Vec2 anchor, bool enableLimit = false, float lowerAngle = 0.f, float upperAngle = 0.f,
																   bool enableMotor = false, float motorSpeed = 0.f, float maxMotorTorque = 10.f,
																   bool collideconnected = false);

	/* Crée un RopeJoint */
	// Retourne l'ID du joint
	int CreateRopeJoint(b2Body *b1, b2Vec2 pt1, b2Body *b2, b2Vec2 p2, float maxLength, bool collideconnected = true);

	/* Crée un WeldJoint */
	// Retourne l'ID du joint
	int CreateWeldJoint(b2Body *b1, b2Body *b2, b2Vec2 anchor, float frequencyHz = 4.f, float damping = 0.5f, bool collideconnected = false);

	/* Crée un WheelJoint */
	// Retourne l'ID du joint
	int CreateWheelJoint(b2Body *car, b2Body *wheel, b2Vec2 pWheel, b2Vec2 axis, float frequencyHz = 2.f, float damping = 0.7f,
																				 bool enableMotor = false, float motorSpeed = 0.f, float maxMotorTorque = 10.f,
																				 bool collideconnected = false);

};
