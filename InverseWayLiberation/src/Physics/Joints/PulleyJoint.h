#pragma once
#include "../Joint.h"

struct PulleyJointDef : public JointDef
{
	// Ctor
	PulleyJointDef()
	{
		body1 = nullptr;
		point1.SetZero();
		body2 = nullptr;
		point2.SetZero();
		groundP1.SetZero();
		groundP2.SetZero();
		ratio = 1.f;
		collideconnected = true;
	}
	
	// Initialisation
	PulleyJointDef(b2Body *b1, b2Vec2 pt1, b2Body *b2, b2Vec2 pt2, b2Vec2 _groundP1, b2Vec2 _groundP2, float _ratio = 1.f, bool _collideconnected = true)
	{
		body1 = b1;
		point1 = pt1;
		body2 = b2;
		point2 = pt2;
		groundP1 = _groundP1;
		groundP2 = _groundP2;
		ratio = _ratio;
		collideconnected = _collideconnected;
	}
	
	// Attributs
	b2Body *body1;
	b2Vec2 point1;
	b2Body *body2;
	b2Vec2 point2;
	b2Vec2 groundP1;
	b2Vec2 groundP2;
	float ratio;
	bool collideconnected;
};

class Joint;
class PulleyJoint : public Joint
{
public:
	// Ctor & dtor
	PulleyJoint(const PulleyJointDef &def);
	virtual ~PulleyJoint(void);
	
	// Accesseurs
	float GetRatio() const;
	float GetLenghtA() const; // Never let Pulley have lenght == 0 !
	float GetLenghtB() const;
	b2Vec2 GetRelativeAnchorA() const;
	b2Vec2 GetRelativeAnchorB() const;
	b2Vec2 GetGroundAnchorA() const;
	b2Vec2 GetGroundAnchorB() const;
};
