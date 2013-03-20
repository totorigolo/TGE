#pragma once
#include <SFML/Graphics.hpp>
#include <Box2D/Box2D.h>
#include <Thor/Resources.hpp>
#include "../Joint.h"

class Joint;
class WeldJoint : public Joint
{
public:
	// Ctor & dtor										   Anchor relative to bodyA
	WeldJoint(PhysicManager *physicMgr, b2Body *b1, b2Body *b2, b2Vec2 anchor, float frequencyHz = 4.f, float damping = 0.5f
											  , bool collideconnected = false);
	virtual ~WeldJoint(void);
	
	// Accesseurs
	float GetReferenceAngle() const;
	float GetFrequencyHz() const;
	float GetDampingRatio() const;
	b2Vec2 GetAnchorA() const;
	b2Vec2 GetAnchorB() const;

	void SetFrequencyHz(float frequencyHz);
	void SetDampingRatio(float damping);
};
