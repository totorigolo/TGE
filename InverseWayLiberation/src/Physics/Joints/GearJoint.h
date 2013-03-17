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
	GearJoint(PhysicManager *physicMgr, b2Body *b1, b2Body *b2, int j1, int j2 , float ratio = 1.f
						, bool collideconnected = true, sf::Color const& color = sf::Color(80, 0, 200));
	virtual ~GearJoint(void);
	
	// Mets à jour le VertexArray
	void Update();

	// Accesseurs
	float GetRatio() const;
	b2Joint *GetJoint1(); // NE PAS CHANGER LE JOINT !!! Seulement modifier ses propriétés (TODO...)
	const b2Joint* GetJoint1() const;
	b2Joint *GetJoint2();
	const b2Joint* GetJoint2() const;
	
	sf::Color& GetColor() { return mColor; }
	void SetColor(sf::Color const& color) { mColor = color; }
	
private:
	sf::Color mColor;
};
