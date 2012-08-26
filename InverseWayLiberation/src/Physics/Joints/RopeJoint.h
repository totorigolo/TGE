#pragma once
#include <SFML/Graphics.hpp>
#include <Box2D/Box2D.h>
#include <Thor/Resources.hpp>
#include "Joint.h"
#include "../World.h"

class World;
class Joint;
class RopeJoint : public Joint
{
public:
	// Ctor & dtor
	RopeJoint(World *world, Body *b1, b2Vec2 p1, Body *b2, b2Vec2 p2, float maxLength, bool collideconnected = true, sf::Color const& color = sf::Color::Magenta);
	virtual ~RopeJoint(void);
	
	// Mets � jour le VertexArray
	void Update();

	// Accesseurs
	float GetMaxLength() const;

	void SetMaxLength(float maxLenght);
	
	sf::Color& GetColor() { return mColor; }
	void SetColor(sf::Color const& color) { mColor = color; }
	
private:
	sf::Color mColor;
};
