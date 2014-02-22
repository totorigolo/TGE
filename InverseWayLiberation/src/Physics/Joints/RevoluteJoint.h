#pragma once
#include "../Joint.h"

struct RevoluteJointDef : public JointDef
{
	// Ctor
	RevoluteJointDef()
	{
		body1 = nullptr;
		body2 = nullptr;
		anchor.SetZero();
		enableLimit = false;
		lowerAngle = 0.f;
		upperAngle = 0.f;
		enableMotor = false;
		motorSpeed = 0.f;
		maxMotorTorque = 10.f;
		collideconnected = false;
	}
	
	// Initialisation
	RevoluteJointDef(b2Body *b1, b2Body *b2, b2Vec2 _anchor,
							bool _enableLimit = false, float _lowerAngle = 0.f, float _upperAngle = 0.f,
							bool _enableMotor = false, float _motorSpeed = 0.f, float _maxMotorTorque = 10.f,
							bool _collideconnected = false)
	{
		body1 = b1;
		body2 = b2;
		anchor = _anchor;
		enableLimit = _enableLimit;
		lowerAngle = _lowerAngle;
		upperAngle = _upperAngle;
		enableMotor = _enableMotor;
		motorSpeed = _motorSpeed;
		maxMotorTorque = _maxMotorTorque;
		collideconnected = _collideconnected;
	}
	
	// Attributs
	b2Body *body1;
	b2Body *body2;
	b2Vec2 anchor;
	bool enableLimit;
	float lowerAngle;
	float upperAngle;
	bool enableMotor;
	float motorSpeed;
	float maxMotorTorque;
	bool collideconnected;
};

class Joint;
class RevoluteJoint : public Joint
{
public:
	// Ctor & dtor
	RevoluteJoint(const RevoluteJointDef &def);
	virtual ~RevoluteJoint(void);
	
	// Accesseurs
	float GetJointAngle() const;
	float GetJointSpeed() const;
	bool IsCollideConnected() const;
	float GetReferenceAngle() const;
	bool IsLimitEnabled() const;
	float GetLowerAngle() const;
	float GetUpperAngle() const;
	float GetLowerAngleRad() const;
	float GetUpperAngleRad() const;
	bool IsMotorEnabled() const;
	float GetMotorTorque(float inv_dt) const;
	float GetMaxMotorTorque() const;
	float GetMotorSpeed() const;
	float GetMotorSpeedRad() const;
	b2Vec2 GetAnchorRelativeToBodyA() const;
	b2Vec2 GetAnchorRelativeToBodyB() const;
	
	void SetLimitEnabled(bool enableLimit);
	void SetLimits(float lowerAngle, float upperAngle);
	void SetMotorEnabled(bool enableMotor);
	void SetMaxMotorTorque(float maxMotorTorque);
	void SetMotorSpeed(float motorSpeed);
};
