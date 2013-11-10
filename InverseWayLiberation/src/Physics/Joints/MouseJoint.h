#pragma once
#include "../Joint.h"

#include <SFML/Graphics.hpp>
#include <Box2D/Box2D.h>

struct MouseJointDef : public JointDef
{
	// Ctor
	MouseJointDef()
	{
		body = nullptr;
		ground = nullptr;
		target.SetZero();
		maxForce = 0.f;
		frequencyHz = 4.f;
		damping = 0.5f;
	}
	
	// Initialisation
	MouseJointDef(b2Body *_body, b2Body *_ground, b2Vec2 _target, float _maxForce, float _frequencyHz = 4.f, float _damping = 0.5f)
	{
		body = _body;
		ground = _ground;
		target = _target;
		maxForce = _maxForce;
		frequencyHz = _frequencyHz;
		damping = _damping;
	}
	
	// Attributs
	b2Body *body;
	b2Body *ground;
	b2Vec2 target;
	float maxForce;
	float frequencyHz;
	float damping;
};

class Joint;
class MouseJoint : public Joint
{
public:
	// Ctor & dtor
	MouseJoint(const MouseJointDef &def);
	virtual ~MouseJoint(void);

	// Accesseurs
	void SetTarget(b2Vec2 const& target);

	float GetMaxForce() const;
	float GetFrequencyHz() const;
	float GetDampingRatio() const;
	b2Body const* GetAttachedBody() const;
	b2Vec2 GetAnchor() const;
	b2Vec2 GetTarget() const;
};
