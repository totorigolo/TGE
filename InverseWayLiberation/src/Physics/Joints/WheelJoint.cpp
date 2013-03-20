#include "WheelJoint.h"
#include "../../Tools/utils.h"

//Ctor
WheelJoint::WheelJoint(PhysicManager *physicMgr, b2Body *car, b2Body *wheel, b2Vec2 pWheel, b2Vec2 axis, float frequencyHz, float damping
																					   , bool enableMotor, float motorSpeed, float maxMotorTorque
																					   , bool collideconnected)
	: Joint(physicMgr)
{
	assert(mPhysicMgr && "n'est pas valide.");
	assert(car && "n'est pas valide.");
	assert(wheel && "n'est pas valide.");
	
	mPhysicMgr->RegisterJoint(this);

	b2WheelJointDef jointDef;
	jointDef.Initialize(car, wheel, wheel->GetWorldPoint(pWheel), axis);
	jointDef.motorSpeed = motorSpeed;
	jointDef.maxMotorTorque = maxMotorTorque;
	jointDef.enableMotor = enableMotor;
	jointDef.frequencyHz = frequencyHz;
	jointDef.dampingRatio = damping;
	jointDef.collideConnected = collideconnected;
	mJoint = mPhysicMgr->Createb2Joint(&jointDef);
	mJoint->SetUserData(this);
	
	mIsAlive = true;
	
	car->SetAwake(true);
	wheel->SetAwake(true);
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
