#include "RevoluteJoint.h"
#include "../../utils.h"

//Ctor
RevoluteJoint::RevoluteJoint(World *world, Body *b1, Body *b2, b2Vec2 anchor, bool enableLimit, float lowerAngle, float upperAngle
																			, bool enableMotor, float motorSpeed, float maxMotorTorque
																			, bool collideconnected, sf::Color const& color)
	: Joint(world), mColor(color)
{
	mBodyA = b1;
	mBodyB = b2;

	if (mWorld && mBodyA && mBodyB)
	{
		b2RevoluteJointDef jointDef;
		jointDef.Initialize(mBodyA->GetBody(), mBodyB->GetBody(), mBodyA->GetBody()->GetWorldPoint(anchor));
		jointDef.collideConnected = collideconnected;
		jointDef.enableLimit = enableLimit;
		jointDef.lowerAngle = lowerAngle * RPD;
		jointDef.upperAngle = upperAngle * RPD;
		jointDef.enableMotor = enableMotor;
		jointDef.motorSpeed = motorSpeed * RPD;
		jointDef.maxMotorTorque = maxMotorTorque;
		mJoint = (b2RevoluteJoint*) mWorld->CreateJoint(&jointDef, this);
		
		mBodyA->RegisterJoint(this);
		mBodyB->RegisterJoint(this);
		mIsNull = false;
	}

	// Règle le VertexArray
	(*this)[0].color = mColor;
	(*this)[1].color = mColor;
}

// Dtor
RevoluteJoint::~RevoluteJoint(void)
{
}

// Mets à jour le VertexArray
void RevoluteJoint::Update()
{
	if (mBodyA && mBodyB)
	{
		if (!mBodyA->IsNull() && !mBodyB->IsNull())
		{
			(*this)[0].position = b22sfVec(mJoint->GetAnchorA(), mWorld->GetPPM());
			(*this)[1].position = b22sfVec(mJoint->GetBodyB()->GetPosition(), mWorld->GetPPM());
		}
		else
		{
			mWorld->DestroyJoint(this, false);
			delete this;
		}
	}
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
