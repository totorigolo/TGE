#pragma once
#include <SFML/Graphics.hpp>
#include <Box2D/Box2D.h>
#include <Thor/Resources.hpp>
#include "../Joint.h"

class Joint;
class MouseJoint : public Joint
{
public:
	// Ctor & dtor
	MouseJoint(PhysicManager *physicMgr, b2Body *body, b2Body *ground, b2Vec2 target
					 , float maxForce, float frequencyHz = 4.f, float damping = 0.5f
										, sf::Color const& color = sf::Color::Cyan);
	virtual ~MouseJoint(void);
	
	// Mets à jour le VertexArray
	void Update();

	// Accesseurs
	void SetTarget(b2Vec2 const& target);

	float GetMaxForce() const;
	float GetFrequencyHz() const;
	float GetDampingRatio() const;
	b2Body const* GetAttachedBody() const;
	b2Vec2 GetAnchor() const;
	b2Vec2 GetTarget() const;
	
	sf::Color& GetColor() { return mColor; }
	void SetColor(sf::Color const& color) { mColor = color; }
	
private:
	sf::Color mColor;
};
