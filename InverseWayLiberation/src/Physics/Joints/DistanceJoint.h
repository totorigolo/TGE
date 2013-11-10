#pragma once
#include "../Joint.h"

#include <SFML/Graphics.hpp>
#include <Box2D/Box2D.h>

struct DistanceJointDef : public JointDef
{
	// Ctor
	DistanceJointDef()
	{
		body1 = nullptr;
		point1.SetZero();
		body2 = nullptr;
		point2.SetZero();
		frequencyHz = 4.f;
		damping = 0.5f;
		collideconnected = true;
	}

	// Initialisation
	DistanceJointDef(b2Body *b1, b2Vec2 pt1, b2Body *b2, b2Vec2 pt2, float _frequencyHz = 4.f, float _damping = 0.5f, bool _collideconnected = true)
	{
		body1 = b1;
		point1 = pt1;
		body2 = b2;
		point2 = pt2;
		frequencyHz = _frequencyHz;
		damping = _damping;
		collideconnected = _collideconnected;
	}

	// Attributs
	b2Body *body1;
	b2Vec2 point1;
	b2Body *body2;
	b2Vec2 point2;
	float frequencyHz;
	float damping;
	bool collideconnected;
};

class Joint;
class DistanceJoint : public Joint
{
public:
	// Ctor & dtor
	DistanceJoint(const DistanceJointDef &def);
	virtual ~DistanceJoint(void);
	
	// Accesseurs
	float GetLength() const;
	float GetFrequencyHz() const;
	float GetDampingRatio() const;
	b2Vec2 GetAnchorA() const;
	b2Vec2 GetAnchorB() const;

	void SetLength(float length);
	void SetFrequencyHz(float frequencyHz);
	void SetDampingRatio(float damping);
};
