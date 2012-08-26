#pragma once
#include <SFML/Graphics.hpp>
#include <Box2D/Box2D.h>
#include <Thor/Resources.hpp>
#include "Joint.h"
#include "../World.h"

class World;
class Joint;
class FrictionJoint : public Joint
{
public:
	// Ctor & dtor
	FrictionJoint(World *world, Body *b1, b2Vec2 p1, Body *b2, b2Vec2 p2, float maxForce = 0.f, float maxTorque = 0.f, bool collideconnected = true
																		, sf::Color const& color = sf::Color::Magenta);
	virtual ~FrictionJoint(void);
	
	// Mets à jour le VertexArray
	void Update();

	// Accesseurs
	float GetMaxForce() const;
	float GetMaxTorque() const;
	b2Vec2 GetAnchorA() const;
	b2Vec2 GetAnchorB() const;
	
	void SetMaxForce(float maxForce);
	void SetMaxTorque(float maxTorque);
	
	sf::Color& GetColor() { return mColor; }
	void SetColor(sf::Color const& color) { mColor = color; }
	
private:
	sf::Color mColor;
};
