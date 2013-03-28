#pragma once
#include <SFML/Graphics.hpp>
#include <Box2D/Box2D.h>
#include <Thor/Resources.hpp>
#include "../Joint.h"

class Joint;
class WheelJoint : public Joint
{
public:
	// Ctor & dtor												Anchor relative to wheel
	WheelJoint(b2Body *car, b2Body *wheel, b2Vec2 pWheel, b2Vec2 axis, float frequencyHz = 2.f, float damping = 0.7f
																	 , bool enableMotor = false, float motorSpeed = 0.f, float maxMotorTorque = 10.f
																	 , bool collideconnected = false);
	virtual ~WheelJoint(void);
	
	// Accesseurs
	float GetJointSpeed() const;
	bool IsMotorEnabled() const;
	float GetMotorTorque(float inv_dt) const;
	float GetMaxMotorTorque() const;
	float GetMotorSpeed() const;
	float GetFrequencyHz() const;
	float GetDampingRatio() const;
	b2Vec2 GetAnchorRelativeToBodyA() const;
	b2Vec2 GetAnchorRelativeToBodyB() const;
	
	void SetMotorEnabled(bool enableMotor);
	void SetMaxMotorTorque(float maxMotorTorque);
	void SetMotorSpeed(float motorSpeed);
	void SetFrequencyHz(float frequencyHz);
	void SetDampingRatio(float damping);
};
