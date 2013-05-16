#pragma once
#include <SFML/Graphics.hpp>
#include <Box2D/Box2D.h>
#include <Thor/Resources.hpp>
#include "../Joint.h"

struct PrismaticJointDef : public JointDef
{
	// Ctor
	PrismaticJointDef()
	{
		body1 = nullptr;
		body2 = nullptr;
		anchor.SetZero();
		axis.SetZero();
		enableLimit = false;
		lowerTranslation = 0.f;
		upperTranslation = 0.f;
		enableMotor = false;
		motorSpeed = 0.f;
		maxMotorForce = 10.f;
		collideconnected = false;
	}
	
	// Initialisation
	PrismaticJointDef(b2Body *b1, b2Body *b2, b2Vec2 _anchor, b2Vec2 _axis, bool _enableLimit = false, float _lowerTranslation = 0.f, float _upperTranslation = 0.f,
																		  bool _enableMotor = false, float _motorSpeed = 0.f, float _maxMotorForce = 10.f,
																		  bool _collideconnected = false)
	{
		body1 = b1;
		body2 = b2;
		anchor = _anchor;
		axis = _axis;
		enableLimit = _enableLimit;
		lowerTranslation = _lowerTranslation;
		upperTranslation = _upperTranslation;
		enableMotor = _enableMotor;
		motorSpeed = _motorSpeed;
		maxMotorForce = _maxMotorForce;
		collideconnected = _collideconnected;
	}
	
	// Attributs
	b2Body *body1;
	b2Body *body2;
	b2Vec2 anchor;
	b2Vec2 axis;
	bool enableLimit;
	float lowerTranslation;
	float upperTranslation;
	bool enableMotor;
	float motorSpeed;
	float maxMotorForce;
	bool collideconnected;
};

class Joint;
class PrismaticJoint : public Joint
{
public:
	// Ctor & dtor
	PrismaticJoint(const PrismaticJointDef &def);
	virtual ~PrismaticJoint(void);
	
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
};
