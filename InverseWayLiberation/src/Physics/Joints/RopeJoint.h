#pragma once
#include <SFML/Graphics.hpp>
#include <Box2D/Box2D.h>
#include <Thor/Resources.hpp>
#include "../Joint.h"

class Joint;
class RopeJoint : public Joint
{
public:
	// Ctor & dtor
	RopeJoint(b2Body *b1, b2Vec2 pt1, b2Body *b2, b2Vec2 p2, float maxLength, bool collideconnected = true);
	virtual ~RopeJoint(void);
	
	// Accesseurs
	float GetMaxLength() const;

	void SetMaxLength(float maxLenght);
};
