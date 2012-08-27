#pragma once
#include <SFML/Graphics.hpp>
#include <Box2D/Box2D.h>
#include <Thor/Resources.hpp>
#include "Joint.h"
#include "../World.h"

class World;
class Joint;
class PrismaticJoint : public Joint
{
public:
	// Ctor & dtor                             Anchor relative to bodyA
	PrismaticJoint(World *world, Body *b1, Body *b2, b2Vec2 anchor, b2Vec2 axis, bool enableLimit = false, float lowerTranslation = 0.f, float upperTranslation = 0.f
																			   , bool enableMotor = false, float motorSpeed = 0.f, float maxMotorForce = 10.f
																			   , bool collideconnected = false, sf::Color const& color = sf::Color::Green);
	virtual ~PrismaticJoint(void);
	
	// Mets à jour le VertexArray
	void Update();

	// Accesseurs
	float GetJointTranslation() const;
	float GetJointSpeed() const;
	bool IsCollideConnected() const;
	bool IsLimitEnabled() const;
	float GetLowerTranslation() const;
	float GetUpperTranslation() const;
	bool IsMotorEnabled() const;
	float GetMotorSpeed() const;
	float GetMotorForce(float inv_dt) const;
	float GetMaxMotorForce() const;
	b2Vec2 GetAnchorRelativeToBodyA() const;
	b2Vec2 GetAnchorRelativeToBodyB() const;
	
	void SetLimitEnabled(bool enableLimit);
	void SetLimits(float lowerTranslation, float upperTranslation);
	void SetMotorEnabled(bool enableMotor);
	void SetMaxMotorForce(float maxMotorTorque);
	void SetMotorSpeed(float motorSpeed);

	sf::Color& GetColor() { return mColor; }
	void SetColor(sf::Color const& color) { mColor = color; }
	
private:
	sf::Color mColor;
};
