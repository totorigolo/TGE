#pragma once
#include <SFML/Graphics.hpp>
#include <Box2D/Box2D.h>
#include <Thor/Resources.hpp>
#include "../Joint.h"

struct WheelJointDef : public JointDef
{
	// Ctor
	WheelJointDef()
	{
		car = nullptr;
		wheel = nullptr;
		pWheel.SetZero();
		axis.SetZero();
		frequencyHz = 2.f;
		damping = 0.7f;
		enableMotor = false;
		motorSpeed = 0.f;
		maxMotorTorque = 10.f;
		collideconnected = false;
	}
	
	// Initialisation
	WheelJointDef(b2Body *_car, b2Body *_wheel, b2Vec2 _pWheel, b2Vec2 _axis, float _frequencyHz = 2.f, float _damping = 0.7f,
																  bool _enableMotor = false, float _motorSpeed = 0.f, float _maxMotorTorque = 10.f,
																  bool _collideconnected = false)
	{
		car = _car;
		wheel = _wheel;
		pWheel = _pWheel;
		axis = _axis;
		frequencyHz = _frequencyHz;
		damping = _damping;
		enableMotor = _enableMotor;
		motorSpeed = _motorSpeed;
		maxMotorTorque = _maxMotorTorque;
		collideconnected = _collideconnected;
	}
	
	// Attributs
	b2Body *car;
	b2Body *wheel;
	b2Vec2 pWheel;
	b2Vec2 axis;
	float frequencyHz;
	float damping;
	bool enableMotor;
	float motorSpeed;
	float maxMotorTorque;
	bool collideconnected;
};

class Joint;
class WheelJoint : public Joint
{
public:
	// Ctor & dtor
	WheelJoint(const WheelJointDef &def);
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
