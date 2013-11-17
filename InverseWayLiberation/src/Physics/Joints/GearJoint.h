#pragma once
#include "../Joint.h"

#include <SFML/Graphics.hpp>
#include <Box2D/Box2D.h>

struct GearJointDef : public JointDef
{
	// Ctor
	GearJointDef()
	{
		body1 = nullptr;
		body2 = nullptr;
		joint1 = -1;
		joint2 = -1;
		ratio = 1.f;
		collideconnected = true;
	}
	
	// Initialisation
	GearJointDef(b2Body *b1, b2Body *b2, int j1, int j2, float _ratio = 1.f, bool _collideconnected = true)
	{
		body1 = b1;
		body2 = b2;
		joint1 = j1;
		joint2 = j2;
		ratio = _ratio;
		collideconnected = _collideconnected;
	}
	
	// Attributs
	b2Body *body1;
	b2Body *body2;
	int joint1;
	int joint2;
	float ratio;
	bool collideconnected;
};

class Joint;
class GearJoint : public Joint
{
public:
	// Ctor & dtor
	GearJoint(const GearJointDef &def);
	virtual ~GearJoint(void);
	
	// Accesseurs
	float GetRatio() const;
	b2Joint *const GetJoint1();
	const b2Joint* GetJoint1() const;
	b2Joint *const GetJoint2();
	const b2Joint* GetJoint2() const;
};
