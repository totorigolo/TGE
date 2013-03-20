#pragma once
#include <SFML/Graphics.hpp>
#include <Box2D/Box2D.h>
#include <Thor/Resources.hpp>
#include "../Joint.h"

class Joint;
class RevoluteJoint : public Joint
{
public:
	// Ctor & dtor											 Anchor relative to bodyA
	RevoluteJoint(PhysicManager *physicMgr, b2Body *b1, b2Body *b2, b2Vec2 anchor
																, bool enableLimit = false, float lowerAngle = 0.f, float upperAngle = 0.f
																, bool enableMotor = false, float motorSpeed = 0.f, float maxMotorTorque = 10.f
																, bool collideconnected = false);
	virtual ~RevoluteJoint(void);
	
	// Accesseurs
	float GetJointAngle() const;
	float GetJointSpeed() const;
	bool IsCollideConnected() const;
	bool IsLimitEnabled() const;
	float GetLowerAngle() const;
	float GetUpperAngle() const;
	bool IsMotorEnabled() const;
	float GetMotorTorque(float inv_dt) const;
	float GetMaxMotorTorque() const;
	float GetMotorSpeed() const;
	b2Vec2 GetAnchorRelativeToBodyA() const;
	b2Vec2 GetAnchorRelativeToBodyB() const;
	
	void SetLimitEnabled(bool enableLimit);
	void SetLimits(float lowerAngle, float upperAngle);
	void SetMotorEnabled(bool enableMotor);
	void SetMaxMotorTorque(float maxMotorTorque);
	void SetMotorSpeed(float motorSpeed);
};
