#include "JointFactory.h"
#include "PhysicManager.h"
#include "Joint.h"
#include "Joints\DistanceJoint.h"

namespace JointFactory
{
	// Variables privés
	namespace
	{
		//...
	}
	
	// Crée un DistanceJoint
	void CreateDistanceJoint(b2Body *b1, b2Vec2 pt1, b2Body *b2, b2Vec2 pt2, float frequencyHz, float damping, bool collideconnected)
	{
		new DistanceJoint(b1, pt1, b2, pt2, frequencyHz, damping, collideconnected);
	}

} // namespace JointFactory
