#pragma once
#include <SFML/Graphics.hpp>
#include <Box2D/Box2D.h>
#include <Thor/Resources.hpp>
#include "../Joint.h"
#include "../World.h"

class World;
class Joint;
class GearJoint : public Joint
{
	// Si un des deux joints utilisés est supprimé, alors ce joint provoquera un bug

public:
	// Ctor & dtor
	GearJoint(World *world, Body *b1, Body *b2, Joint *j1, Joint *j2, float ratio = 1.f, bool collideconnected = true, sf::Color const& color = sf::Color(80, 0, 200));
	virtual ~GearJoint(void);
	
	// Mets à jour le VertexArray
	void Update();

	// Accesseurs
	float GetRatio() const;
	Joint *GetJoint1(); // NE PAS CHANGER LE JOINT !!! Seulement modifier ses propriétés
	const Joint* GetJoint1() const;
	Joint *GetJoint2();
	const Joint* GetJoint2() const;
	
	sf::Color& GetColor() { return mColor; }
	void SetColor(sf::Color const& color) { mColor = color; }
	
private:
	sf::Color mColor;
};
