#include "PrismaticJoint.h"
#include "../../Tools/utils.h"

//Ctor
PrismaticJoint::PrismaticJoint(PhysicManager *physicMgr, b2Body *b1, b2Body *b2, b2Vec2 anchor, b2Vec2 axis, bool enableLimit, float lowerTranslation, float upperTranslation
																						   , bool enableMotor, float motorSpeed, float maxMotorForce
																						   , bool collideconnected)
	: Joint(physicMgr)
{
	assert(mPhysicMgr && "n'est pas valide.");
	assert(b1 && "n'est pas valide.");
	assert(b2 && "n'est pas valide.");
	
	mPhysicMgr->RegisterJoint(this);

	b2PrismaticJointDef jointDef;
	jointDef.Initialize(b1, b2, b1->GetWorldPoint(anchor), axis);
	jointDef.collideConnected = collideconnected;
	jointDef.enableLimit = enableLimit;
	jointDef.lowerTranslation = lowerTranslation;
	jointDef.upperTranslation = upperTranslation;
	jointDef.enableMotor = enableMotor;
	jointDef.motorSpeed = motorSpeed * RPD;
	jointDef.maxMotorForce = maxMotorForce;
	mJoint = mPhysicMgr->Createb2Joint(&jointDef);
	mJoint->SetUserData(this);
	
	mIsAlive = true;
	
	b1->SetAwake(true);
	b2->SetAwake(true);
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
