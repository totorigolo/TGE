#pragma once
#include "../Joint.h"

#include <SFML/Graphics.hpp>
#include <Box2D/Box2D.h>

struct FrictionJointDef : public JointDef
{
	// Ctor
	FrictionJointDef()
	{
		body1 = nullptr;
		point1.SetZero();
		body2 = nullptr;
		point2.SetZero();
		maxFrictionForce = 0.f;
		maxFrictionTorque = 0.f;
		collideconnected = true;
	}
	
	// Initialisation
	FrictionJointDef(b2Body *b1, b2Vec2 pt1, b2Body *b2, b2Vec2 pt2, float _maxFrictionForce = 0.f, float _maxFrictionTorque = 0.f,
					 bool _collideconnected = true)
	{
		body1 = b1;
		point1 = pt1;
		body2 = b2;
		point2 = pt2;
		maxFrictionForce = _maxFrictionForce;
		maxFrictionTorque = _maxFrictionTorque;
		collideconnected = _collideconnected;
	}
	
	// Attributs
	b2Body *body1;
	b2Vec2 point1;
	b2Body *body2;
	b2Vec2 point2;
	float maxFrictionForce;
	float maxFrictionTorque;
	bool collideconnected;
};

class Joint;
class FrictionJoint : public Joint
{
public:
	// Ctor & dtor
	FrictionJoint(const FrictionJointDef &def);
	virtual ~FrictionJoint(void);
	
	// Accesseurs
	float GetMaxFrictionForce() const;
	float GetMaxFrictionTorque() const;
	b2Vec2 GetRelativeAnchorA() const;
	b2Vec2 GetRelativeAnchorB() const;
	
	void SetMaxFrictionForce(float maxForce);
	void SetMaxFrictionTorque(float maxTorque);
};
