#include "PrismaticJoint.h"
#include "../PhysicManager.h"
#include "../../Tools/utils.h"
#include "../../Tools/Error.h"

//Ctor
PrismaticJoint::PrismaticJoint(const PrismaticJointDef &def)
{
	myAssert(def.body1, "Le b2Body n'existe pas.");
	myAssert(def.body2, "Le b2Body n'existe pas.");
	
	mPhysicMgr.RegisterJoint(this);

	b2PrismaticJointDef jointDef;
	jointDef.Initialize(def.body1, def.body2, def.body1->GetWorldPoint(def.anchor), def.axis);
	jointDef.collideConnected = def.collideconnected;
	jointDef.enableLimit = def.enableLimit;
	jointDef.lowerTranslation = def.lowerTranslation;
	jointDef.upperTranslation = def.upperTranslation;
	jointDef.enableMotor = def.enableMotor;
	jointDef.motorSpeed = def.motorSpeed * RPD;
	jointDef.maxMotorForce = def.maxMotorForce;
	mJoint = mPhysicMgr.Createb2Joint(&jointDef);
	mJoint->SetUserData(this);
	
	mIsAlive = true;
	
	def.body1->SetAwake(true);
	def.body2->SetAwake(true);
}

// Dtor
PrismaticJoint::~PrismaticJoint(void)
{
}

// Accesseurs
float PrismaticJoint::GetJointTranslation() const
{
	return ((b2PrismaticJoint*) mJoint)->GetJointTranslation() * DPR;
}
float PrismaticJoint::GetJointSpeed() const
{
	return ((b2PrismaticJoint*) mJoint)->GetJointSpeed();
}
bool PrismaticJoint::IsCollideConnected() const
{
	return ((b2PrismaticJoint*) mJoint)->GetCollideConnected();
}
bool PrismaticJoint::IsLimitEnabled() const
{
	return ((b2PrismaticJoint*) mJoint)->IsLimitEnabled();
}
float PrismaticJoint::GetLowerTranslation() const
{
	return ((b2PrismaticJoint*) mJoint)->GetLowerLimit();
}
float PrismaticJoint::GetUpperTranslation() const
{
	return ((b2PrismaticJoint*) mJoint)->GetUpperLimit();
}
bool PrismaticJoint::IsMotorEnabled() const
{
	return ((b2PrismaticJoint*) mJoint)->IsMotorEnabled();
}
float PrismaticJoint::GetMotorForce(float inv_dt) const
{
	return ((b2PrismaticJoint*) mJoint)->GetMotorForce(inv_dt);
}
float PrismaticJoint::GetMaxMotorForce() const
{
	return ((b2PrismaticJoint*) mJoint)->GetMaxMotorForce();
}
float PrismaticJoint::GetMotorSpeed() const
{
	return ((b2PrismaticJoint*) mJoint)->GetMotorSpeed();
}
b2Vec2 PrismaticJoint::GetAnchorRelativeToBodyA() const
{
	return ((b2PrismaticJoint*) mJoint)->GetAnchorA();
}
b2Vec2 PrismaticJoint::GetAnchorRelativeToBodyB() const
{
	return ((b2PrismaticJoint*) mJoint)->GetAnchorB();
}


void PrismaticJoint::SetLimitEnabled(bool enableLimit)
{
	((b2PrismaticJoint*) mJoint)->EnableLimit(enableLimit);
}
void PrismaticJoint::SetLimits(float lowerTranslation, float upperTranslation)
{
	((b2PrismaticJoint*) mJoint)->SetLimits(lowerTranslation, upperTranslation);
}
void PrismaticJoint::SetMotorEnabled(bool enableMotor)
{
	((b2PrismaticJoint*) mJoint)->EnableMotor(enableMotor);
}
void PrismaticJoint::SetMaxMotorForce(float maxMotorForce)
{
	((b2PrismaticJoint*) mJoint)->SetMaxMotorForce(maxMotorForce);
}
void PrismaticJoint::SetMotorSpeed(float motorSpeed)
{
	((b2PrismaticJoint*) mJoint)->SetMotorSpeed(motorSpeed);
}
