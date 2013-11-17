#pragma once
#include "../Joint.h"

#include <SFML/Graphics.hpp>
#include <Box2D/Box2D.h>

struct WeldJointDef : public JointDef
{
	// Ctor
	WeldJointDef()
	{
		body1 = nullptr;
		body2 = nullptr;
		anchor.SetZero();
		frequencyHz = 4.f;
		damping = 0.5f;
		collideconnected = false;
	}
	
	// Initialisation
	WeldJointDef(b2Body *b1, b2Body *b2, b2Vec2 _anchor, float _frequencyHz = 4.f, float _damping = 0.5f, bool _collideconnected = false)
	{
		body1 = b1;
		body2 = b2;
		anchor = _anchor;
		frequencyHz = _frequencyHz;
		damping = _damping;
		collideconnected = _collideconnected;
	}
	
	// Attributs
	b2Body *body1;
	b2Body *body2;
	b2Vec2 anchor;
	float frequencyHz;
	float damping;
	bool collideconnected;
};

class Joint;
class WeldJoint : public Joint
{
public:
	// Ctor & dtor
	WeldJoint(const WeldJointDef &def);
	virtual ~WeldJoint(void);
	
	// Accesseurs
	float GetReferenceAngle() const;
	float GetFrequencyHz() const;
	float GetDampingRatio() const;
	b2Vec2 GetAnchorRelativeToBodyA() const;
	b2Vec2 GetAnchorRelativeToBodyB() const;

	void SetFrequencyHz(float frequencyHz);
	void SetDampingRatio(float damping);
};
