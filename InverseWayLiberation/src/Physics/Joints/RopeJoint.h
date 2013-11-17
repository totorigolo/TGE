#pragma once
#include "../Joint.h"

#include <SFML/Graphics.hpp>
#include <Box2D/Box2D.h>

struct RopeJointDef : public JointDef
{
	// Ctor
	RopeJointDef()
	{
		body1 = nullptr;
		point1.SetZero();
		body2 = nullptr;
		point2.SetZero();
		maxLength = 1.f;
		collideconnected = true;
	}
	
	// Initialisation
	RopeJointDef(b2Body *b1, b2Vec2 pt1, b2Body *b2, b2Vec2 pt2, float _maxLength, bool _collideconnected = true)
	{
		body1 = b1;
		point1 = pt1;
		body2 = b2;
		point2 = pt2;
		maxLength = _maxLength;
		collideconnected = _collideconnected;
	}
	
	// Attributs
	b2Body *body1;
	b2Vec2 point1;
	b2Body *body2;
	b2Vec2 point2;
	float maxLength;
	bool collideconnected;
};

class Joint;
class RopeJoint : public Joint
{
public:
	// Ctor & dtor
	RopeJoint(const RopeJointDef &def);
	virtual ~RopeJoint(void);
	
	// Accesseurs
	float GetMaxLength() const;
	void SetMaxLength(float maxLenght);
	b2Vec2 GetRelativeAnchorA() const;
	b2Vec2 GetRelativeAnchorB() const;
};
