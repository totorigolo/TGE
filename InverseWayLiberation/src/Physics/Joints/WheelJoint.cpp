#include "WheelJoint.h"
#include "../../Tools/utils.h"

//Ctor
WheelJoint::WheelJoint(World *world, Body *car, Body *wheel, b2Vec2 pWheel, b2Vec2 axis, float frequencyHz, float damping
																					   , bool enableMotor, float motorSpeed, float maxMotorTorque
																					   , bool collideconnected, sf::Color const& color)
	: Joint(world), mColor(color)
{
	mBodyA = car;
	mBodyB = wheel;

	if (mWorld && mBodyA && mBodyB)
	{
		b2WheelJointDef jointDef;
		jointDef.Initialize(mBodyA->GetBody(), mBodyB->GetBody(), mBodyB->GetBody()->GetWorldPoint(pWheel), axis);
		jointDef.motorSpeed = motorSpeed;
		jointDef.maxMotorTorque = maxMotorTorque;
		jointDef.enableMotor = enableMotor;
		jointDef.frequencyHz = frequencyHz;
		jointDef.dampingRatio = damping;
		jointDef.collideConnected = collideconnected;
		mJoint = (b2WheelJoint*) mWorld->CreateJoint(&jointDef, this);
		
		mBodyA->RegisterJoint(this);
		mBodyB->RegisterJoint(this);
		mIsNull = false;
	}

	(*this)[0].color = mColor;
	(*this)[1].color = mColor;
}

// Dtor
WheelJoint::~WheelJoint(void)
{
}

// Mets à jour le VertexArray
void WheelJoint::Update()
{
	if (mBodyA && mBodyB)
	{
		if (!mBodyA->IsNull() && !mBodyB->IsNull())
		{
			(*this)[0].position = b22sfVec(mJoint->GetAnchorA(), mWorld->GetPPM());
			(*this)[1].position = b22sfVec(mJoint->GetAnchorB(), mWorld->GetPPM());
		}
		else
		{
			mWorld->DestroyJoint(this, false);
			delete this;
		}
	}
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
