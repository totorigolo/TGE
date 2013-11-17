#include "RevoluteJoint.h"
#include "../PhysicManager.h"
#include "../../Tools/utils.h"
#include "../../Tools/Error.h"

//Ctor
RevoluteJoint::RevoluteJoint(const RevoluteJointDef &def)
{
	myAssert(def.body1, "Le b2Body n'existe pas.");
	myAssert(def.body2, "Le b2Body n'existe pas.");
	
	mPhysicMgr.RegisterJoint(this);

	b2RevoluteJointDef jointDef;
	jointDef.Initialize(def.body1, def.body2, def.body1->GetWorldPoint(def.anchor));
	jointDef.collideConnected = def.collideconnected;
	jointDef.enableLimit = def.enableLimit;
	jointDef.lowerAngle = def.lowerAngle * RPD;
	jointDef.upperAngle = def.upperAngle * RPD;
	jointDef.enableMotor = def.enableMotor;
	jointDef.motorSpeed = def.motorSpeed * RPD;
	jointDef.maxMotorTorque = def.maxMotorTorque;
	mJoint = mPhysicMgr.Createb2Joint(&jointDef);
	mJoint->SetUserData(this);
	
	mIsAlive = true;
	mType = JointType::RevoluteJoint;
	
	def.body1->SetAwake(true);
	def.body2->SetAwake(true);
}

// Dtor
RevoluteJoint::~RevoluteJoint(void)
{
	;
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
float RevoluteJoint::GetReferenceAngle() const
{
	return ((b2RevoluteJoint*) mJoint)->GetReferenceAngle();
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
float RevoluteJoint::GetLowerAngleRad() const
{
	return ((b2RevoluteJoint*)mJoint)->GetLowerLimit();
}
float RevoluteJoint::GetUpperAngleRad() const
{
	return ((b2RevoluteJoint*)mJoint)->GetUpperLimit();
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
	return ((b2RevoluteJoint*) mJoint)->GetMotorSpeed() * DPR;
}
float RevoluteJoint::GetMotorSpeedRad() const
{
	return ((b2RevoluteJoint*)mJoint)->GetMotorSpeed();
}
b2Vec2 RevoluteJoint::GetAnchorRelativeToBodyA() const
{
	return GetBodyA()->GetLocalPoint(((b2RevoluteJoint*) mJoint)->GetAnchorA());
}
b2Vec2 RevoluteJoint::GetAnchorRelativeToBodyB() const
{
	return GetBodyB()->GetLocalPoint(((b2RevoluteJoint*)mJoint)->GetAnchorB());
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
