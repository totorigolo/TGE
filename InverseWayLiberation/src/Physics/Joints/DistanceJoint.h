#pragma once
#include <SFML/Graphics.hpp>
#include <Box2D/Box2D.h>
#include <Thor/Resources.hpp>
#include "Joint.h"
#include "../World.h"

class World;
class Joint;
class DistanceJoint : public Joint
{
public:
	// Ctor & dtor
	DistanceJoint(World *world, Body *b1, b2Vec2 pt1, Body *b2, b2Vec2 p2, float frequencyHz = 4.f, float damping = 0.5f, bool collideconnected = true
																		, sf::Color const& color = sf::Color::Magenta);
	virtual ~DistanceJoint(void);
	
	// Mets à jour le VertexArray
	void Update();

	// Accesseurs
	float GetLength() const;
	float GetFrequencyHz() const;
	float GetDampingRatio() const;
	b2Vec2 GetAnchorA() const;
	b2Vec2 GetAnchorB() const;

	void SetLength(float length);
	void SetFrequencyHz(float frequencyHz);
	void SetDampingRatio(float damping);
	
	sf::Color& GetColor() { return mColor; }
	void SetColor(sf::Color const& color) { mColor = color; }
	
private:
	sf::Color mColor;
};
