#pragma once
#include <SFML/Graphics.hpp>
#include <Box2D/Box2D.h>
#include <Thor/Resources.hpp>
#include "../Joint.h"

class Joint;
class PulleyJoint : public Joint
{
public:
	// Ctor & dtor
	PulleyJoint(PhysicManager *physicMgr, b2Body *b1, b2Vec2 pt1, b2Body *b2, b2Vec2 p2, b2Vec2 groundP1, b2Vec2 groundP2
																		, float ratio = 1.f, bool collideconnected = true
																		, sf::Color const& color = sf::Color(170, 80, 0));
	virtual ~PulleyJoint(void);
	
	// Mets à jour le VertexArray
	void Update();

	// Accesseurs
	float GetRatio() const;
	float GetLenghtA() const; // Never let Pulley have lenght == 0 !
	float GetLenghtB() const;
	b2Vec2 GetAnchorA() const;
	b2Vec2 GetAnchorB() const;
	b2Vec2 GetGroundAnchorA() const;
	b2Vec2 GetGroundAnchorB() const;
	
	sf::Color& GetColor() { return mColor; }
	void SetColor(sf::Color const& color) { mColor = color; }
	
private:
	sf::Color mColor;
};
