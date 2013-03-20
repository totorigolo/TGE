#include "RevoluteJoint.h"
#include "../../Tools/utils.h"

//Ctor
RevoluteJoint::RevoluteJoint(PhysicManager *physicMgr, b2Body *b1, b2Body *b2, b2Vec2 anchor, bool enableLimit, float lowerAngle, float upperAngle
																			, bool enableMotor, float motorSpeed, float maxMotorTorque
																			, bool collideconnected)
	: Joint(physicMgr)
{
	assert(mPhysicMgr && "n'est pas valide.");
	assert(b1 && "n'est pas valide.");
	assert(b2 && "n'est pas valide.");
	
	mPhysicMgr->RegisterJoint(this);

	b2RevoluteJointDef jointDef;
	jointDef.Initialize(b1, b2, b1->GetWorldPoint(anchor));
	jointDef.collideConnected = collideconnected;
	jointDef.enableLimit = enableLimit;
	jointDef.lowerAngle = lowerAngle * RPD;
	jointDef.upperAngle = upperAngle * RPD;
	jointDef.enableMotor = enableMotor;
	jointDef.motorSpeed = motorSpeed * RPD;
	jointDef.maxMotorTorque = maxMotorTorque;
	mJoint = mPhysicMgr->Createb2Joint(&jointDef);
	mJoint->SetUserData(this);
		
	mIsAlive = true;
	
	b1->SetAwake(true);
	b2->SetAwake(true);
}

// Dtor
RevoluteJoint::~RevoluteJoint(void)
{
}

// Accesseurs
float RevoluteJoint::GetJointAngle() const
{
	return ((b2RevoluteJoint*) mJoint)->GetJointAngle() * DPR;
}
float RevoluteJoint::GetJointSpeed() const
{
	return ((b2RevoluteJoint*) mJoint)->GetJointSpeed();
}
bool RevoluteJoint::IsCollideConnected() const
{
	return ((b2RevoluteJoint*) mJoint)->GetCollideConnected();
}
bool RevoluteJoint::IsLimitEnabled() const
{
	return ((b2RevoluteJoint*) mJoint)->IsLimitEnabled();
}
float RevoluteJoint::GetLowerAngle() const
{
	return ((b2RevoluteJoint*) mJoint)->GetLowerLimit() * DPR;
}
float RevoluteJoint::GetUpperAngle() const
{
	return ((b2RevoluteJoint*) mJoint)->GetUpperLimit() * DPR;
}
bool RevoluteJoint::IsMotorEnabled() const
{
	return ((b2RevoluteJoint*) mJoint)->IsMotorEnabled();
}
float RevoluteJoint::GetMotorTorque(float inv_dt) const
{
	return ((b2RevoluteJoint*) mJoint)->GetMotorTorque(inv_dt);
}
float RevoluteJoint::GetMaxMotorTorque() const
{
	return ((b2RevoluteJoint*) mJoint)->GetMaxMotorTorque();
}
float RevoluteJoint::GetMotorSpeed() const
{
	return ((b2RevoluteJoint*) mJoint)->GetMotorSpeed();
}
b2Vec2 RevoluteJoint::GetAnchorRelativeToBodyA() const
{
	return ((b2RevoluteJoint*) mJoint)->GetAnchorA();
}
b2Vec2 RevoluteJoint::GetAnchorRelativeToBodyB() const
{
	return ((b2RevoluteJoint*) mJoint)->GetAnchorB();
}

void RevoluteJoint::SetLimitEnabled(bool enableLimit)
{
	((b2RevoluteJoint*) mJoint)->EnableLimit(enableLimit);
}
void RevoluteJoint::SetLimits(float lowerAngle, float upperAngle)
{
	((b2RevoluteJoint*) mJoint)->SetLimits(lowerAngle * RPD, upperAngle * RPD);
}
void RevoluteJoint::SetMotorEnabled(bool enableMotor)
{
	((b2RevoluteJoint*) mJoint)->EnableMotor(enableMotor);
}
void RevoluteJoint::SetMaxMotorTorque(float maxMotorTorque)
{
	((b2RevoluteJoint*) mJoint)->SetMaxMotorTorque(maxMotorTorque);
}
void RevoluteJoint::SetMotorSpeed(float motorSpeed)
{
	((b2RevoluteJoint*) mJoint)->SetMotorSpeed(motorSpeed);
}
