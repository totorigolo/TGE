#pragma once
#include <SFML/Graphics.hpp>
#include <Box2D/Box2D.h>
#include <Thor/Resources.hpp>
#include "Joint.h"

class Joint;
class DistanceJoint : public Joint
{
public:
	// Ctor & dtor
	DistanceJoint(World *world, Body *b1, b2Vec2 p1, Body *b2, b2Vec2 p2, float frequencyHz = 4.f, float damping = 0.5f, sf::Color const& color = sf::Color::Magenta);
	virtual ~DistanceJoint(void);
	
	// Mets à jour le VertexArray
	void Update();

	// Accesseurs
	float GetLenght() const;
	float GetFrequencyHz() const;
	float GetDampingRatio() const;
	
	sf::Color& GetColor() { return mColor; }
	void SetColor(sf::Color const& color) { mColor = color; }
	
private:
	sf::Color mColor;
};
