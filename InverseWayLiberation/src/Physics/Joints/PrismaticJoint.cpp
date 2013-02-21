#include "PrismaticJoint.h"
#include "../../Tools/utils.h"

//Ctor
PrismaticJoint::PrismaticJoint(PhysicManager *physicMgr, b2Body *b1, b2Body *b2, b2Vec2 anchor, b2Vec2 axis, bool enableLimit, float lowerTranslation, float upperTranslation
																						   , bool enableMotor, float motorSpeed, float maxMotorForce
																						   , bool collideconnected, sf::Color const& color)
	: Joint(physicMgr), mColor(color)
{
	assert(mPhysicMgr && "n'est pas valide.");

	mBodyA = b1;
	mBodyB = b2;

	if (mBodyA && mBodyB)
	{
		b2PrismaticJointDef jointDef;
		jointDef.Initialize(mBodyA, mBodyB, mBodyA->GetWorldPoint(anchor), axis);
		jointDef.collideConnected = collideconnected;
		jointDef.enableLimit = enableLimit;
		jointDef.lowerTranslation = lowerTranslation;
		jointDef.upperTranslation = upperTranslation;
		jointDef.enableMotor = enableMotor;
		jointDef.motorSpeed = motorSpeed * RPD;
		jointDef.maxMotorForce = maxMotorForce;
		mJoint = (b2PrismaticJoint*) mPhysicMgr->CreateJoint(&jointDef, this);
		
		//mBodyA->RegisterJoint(this);
		//mBodyB->RegisterJoint(this);
		mIsNull = false;
	}

	// R�gle le VertexArray
	(*this)[0].color = mColor;
	(*this)[1].color = mColor;
}

// Dtor
PrismaticJoint::~PrismaticJoint(void)
{
}

// Mets � jour le VertexArray
void PrismaticJoint::Update()
{
	Joint::Update();

	if (mBodyA && mBodyB && !mIsNull)
	{
		(*this)[0].position = b22sfVec(mJoint->GetAnchorA(), mPhysicMgr->GetPPM());
		(*this)[1].position = b22sfVec(mJoint->GetBodyB()->GetPosition(), mPhysicMgr->GetPPM());
	}
	else
	{
		//mPhysicMgr->DestroyJoint(this, false);
		//delete this;
	}
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
