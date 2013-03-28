#pragma once
#include <SFML/Graphics.hpp>
#include <Box2D/Box2D.h>
#include <Thor/Resources.hpp>
#include "../Joint.h"

class Joint;
class DistanceJoint : public Joint
{
public:
	// Ctor & dtor
	DistanceJoint(b2Body *b1, b2Vec2 pt1, b2Body *b2, b2Vec2 pt2, float frequencyHz = 4.f, float damping = 0.5f, bool collideconnected = true);
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
