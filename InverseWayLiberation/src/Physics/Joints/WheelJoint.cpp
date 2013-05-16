#include "WheelJoint.h"
#include "../PhysicManager.h"
#include "../../Tools/utils.h"
#include "../../Tools/Error.h"

//Ctor
WheelJoint::WheelJoint(const WheelJointDef &def)
{
	myAssert(def.car, "Le b2Body n'existe pas.");
	myAssert(def.wheel, "Le b2Body n'existe pas.");
	
	mPhysicMgr.RegisterJoint(this);

	b2WheelJointDef jointDef;
	jointDef.Initialize(def.car, def.wheel, def.wheel->GetWorldPoint(def.pWheel), def.axis);
	jointDef.motorSpeed = def.motorSpeed;
	jointDef.maxMotorTorque = def.maxMotorTorque;
	jointDef.enableMotor = def.enableMotor;
	jointDef.frequencyHz = def.frequencyHz;
	jointDef.dampingRatio = def.damping;
	jointDef.collideConnected = def.collideconnected;
	mJoint = mPhysicMgr.Createb2Joint(&jointDef);
	mJoint->SetUserData(this);
	
	mIsAlive = true;
	
	def.car->SetAwake(true);
	def.wheel->SetAwake(true);
}

// Dtor
WheelJoint::~WheelJoint(void)
{
}

// Accesseurs
float WheelJoint::GetJointSpeed() const
{
	return ((b2WheelJoint*) mJoint)->GetJointSpeed();
}
bool WheelJoint::IsMotorEnabled() const
{
	return ((b2WheelJoint*) mJoint)->IsMotorEnabled();
}
float WheelJoint::GetMotorTorque(float inv_dt) const
{
	return ((b2WheelJoint*) mJoint)->GetMotorTorque(inv_dt);
}
float WheelJoint::GetMaxMotorTorque() const
{
	return ((b2WheelJoint*) mJoint)->GetMaxMotorTorque();
}
float WheelJoint::GetMotorSpeed() const
{
	return ((b2WheelJoint*) mJoint)->GetMotorSpeed();
}
float WheelJoint::GetFrequencyHz() const
{
	return ((b2WheelJoint*) mJoint)->GetSpringFrequencyHz();
}
float WheelJoint::GetDampingRatio() const
{
	return ((b2WheelJoint*) mJoint)->GetSpringDampingRatio();
}
b2Vec2 WheelJoint::GetAnchorRelativeToBodyA() const
{
	return ((b2WheelJoint*) mJoint)->GetAnchorA();
}
b2Vec2 WheelJoint::GetAnchorRelativeToBodyB() const
{
	return ((b2WheelJoint*) mJoint)->GetAnchorB();
}
	
void WheelJoint::SetMotorEnabled(bool enableMotor)
{
	((b2WheelJoint*) mJoint)->EnableMotor(enableMotor);
}
void WheelJoint::SetMaxMotorTorque(float maxMotorTorque)
{
	((b2WheelJoint*) mJoint)->SetMaxMotorTorque(maxMotorTorque);
}
void WheelJoint::SetMotorSpeed(float motorSpeed)
{
	((b2WheelJoint*) mJoint)->SetMotorSpeed(motorSpeed);
}
void WheelJoint::SetFrequencyHz(float frequencyHz)
{
	((b2WheelJoint*) mJoint)->SetSpringFrequencyHz(frequencyHz);
}
void WheelJoint::SetDampingRatio(float damping)
{
	((b2WheelJoint*) mJoint)->SetSpringDampingRatio(damping);
}
