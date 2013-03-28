#pragma once
#include <SFML/Graphics.hpp>
#include <Box2D/Box2D.h>
#include <Thor/Resources.hpp>
#include "../Joint.h"

class Joint;
class FrictionJoint : public Joint
{
public:
	// Ctor & dtor
	FrictionJoint(b2Body *b1, b2Vec2 pt1, b2Body *b2, b2Vec2 p2, float maxForce = 0.f, float maxTorque = 0.f, bool collideconnected = true);
	virtual ~FrictionJoint(void);
	
	// Accesseurs
	float GetMaxForce() const;
	float GetMaxTorque() const;
	b2Vec2 GetAnchorA() const;
	b2Vec2 GetAnchorB() const;
	
	void SetMaxForce(float maxForce);
	void SetMaxTorque(float maxTorque);
};
