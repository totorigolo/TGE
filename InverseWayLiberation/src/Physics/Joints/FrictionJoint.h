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
		maxForce = 0.f;
		maxTorque = 0.f;
		collideconnected = true;
	}
	
	// Initialisation
	FrictionJointDef(b2Body *b1, b2Vec2 pt1, b2Body *b2, b2Vec2 pt2, float _maxForce = 0.f, float _maxTorque = 0.f, bool _collideconnected = true)
	{
		body1 = b1;
		point1 = pt1;
		body2 = b2;
		point2 = pt2;
		maxForce = _maxForce;
		maxTorque = _maxTorque;
		collideconnected = _collideconnected;
	}
	
	// Attributs
	b2Body *body1;
	b2Vec2 point1;
	b2Body *body2;
	b2Vec2 point2;
	float maxForce;
	float maxTorque;
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
	float GetMaxForce() const;
	float GetMaxTorque() const;
	b2Vec2 GetAnchorA() const;
	b2Vec2 GetAnchorB() const;
	
	void SetMaxForce(float maxForce);
	void SetMaxTorque(float maxTorque);
};
