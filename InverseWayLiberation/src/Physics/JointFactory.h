#pragma once
#include <Box2D\Box2D.h>

class Joint;
namespace JointFactory
{
	// Crée un DistanceJoint
	void CreateDistanceJoint(b2Body *b1, b2Vec2 pt1, b2Body *b2, b2Vec2 pt2, float frequencyHz = 4.f, float damping = 0.5f, bool collideconnected = true);

};
