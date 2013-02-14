#pragma once
#include <SFML/Graphics.hpp>
#include <Box2D/Box2D.h>
#include <Thor/Resources.hpp>
#include "../Joint.h"
#include "../World.h"

class World;
class Joint;
class WeldJoint : public Joint
{
public:
	// Ctor & dtor                             Anchor relative to bodyA
	WeldJoint(World *world, Body *b1, Body *b2, b2Vec2 anchor, float frequencyHz = 4.f, float damping = 0.5f, bool collideconnected = false
															 , sf::Color const& color = sf::Color::Magenta);
	virtual ~WeldJoint(void);
	
	// Mets à jour le VertexArray
	void Update();

	// Accesseurs
	float GetReferenceAngle() const;
	float GetFrequencyHz() const;
	float GetDampingRatio() const;
	b2Vec2 GetAnchorA() const;
	b2Vec2 GetAnchorB() const;

	void SetFrequencyHz(float frequencyHz);
	void SetDampingRatio(float damping);
	
	sf::Color& GetColor() { return mColor; }
	void SetColor(sf::Color const& color) { mColor = color; }
	
private:
	sf::Color mColor;
};
