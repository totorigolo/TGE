#pragma once
#include <SFML/Graphics.hpp>
#include <Box2D/Box2D.h>
#include <Thor/Resources.hpp>
#include "../Joint.h"

class Joint;
class GearJoint : public Joint
{
public:
	// Ctor & dtor
	GearJoint(b2Body *b1, b2Body *b2, int j1, int j2 , float ratio = 1.f, bool collideconnected = true);
	virtual ~GearJoint(void);
	
	// Accesseurs
	float GetRatio() const;
	b2Joint *GetJoint1(); // NE PAS CHANGER LE JOINT !!! Seulement modifier ses propriétés (TODO...)
	const b2Joint* GetJoint1() const;
	b2Joint *GetJoint2();
	const b2Joint* GetJoint2() const;
};
