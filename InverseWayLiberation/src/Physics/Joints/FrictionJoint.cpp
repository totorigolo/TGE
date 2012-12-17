#include "FrictionJoint.h"
#include "../../Tools/utils.h"

//Ctor
FrictionJoint::FrictionJoint(World *world, Body *b1, b2Vec2 pt1, Body *b2, b2Vec2 p2, float maxForce, float maxTorque, bool collideconnected, sf::Color const& color)
	: Joint(world), mColor(color)
{
	mBodyA = b1;
	mBodyB = b2;

	if (mWorld && mBodyA && mBodyB)
	{
		b2FrictionJointDef jointDef;
		jointDef.bodyA = mBodyA->GetBody();
		jointDef.bodyB = mBodyB->GetBody();
		jointDef.localAnchorA = pt1;
		jointDef.localAnchorB = p2;
		jointDef.maxForce = maxForce;
		jointDef.maxTorque = maxTorque;
		jointDef.collideConnected = collideconnected;
		mJoint = (b2FrictionJoint*) mWorld->CreateJoint(&jointDef, this);
		
		mBodyA->GetBody()->SetBullet(true);
		mBodyB->GetBody()->SetBullet(true);
		
		mBodyA->RegisterJoint(this);
		mBodyB->RegisterJoint(this);
		mIsNull = false;
	}

	(*this)[0].color = mColor;
	(*this)[1].color = mColor;
}

// Dtor
FrictionJoint::~FrictionJoint(void)
{
	if (mBodyA)
		if (mBodyA->GetBody())
			mBodyA->GetBody()->SetBullet(false);
	if (mBodyB)
		if (mBodyB->GetBody())
			mBodyB->GetBody()->SetBullet(false);
}

// Mets à jour le VertexArray
void FrictionJoint::Update()
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
float FrictionJoint::GetMaxForce() const
{
	return ((b2FrictionJoint*) mJoint)->GetMaxForce();
}
float FrictionJoint::GetMaxTorque() const
{
	return ((b2FrictionJoint*) mJoint)->GetMaxTorque();
}
b2Vec2 FrictionJoint::GetAnchorA() const
{
	return ((b2FrictionJoint*) mJoint)->GetAnchorA();
}
b2Vec2 FrictionJoint::GetAnchorB() const
{
	return ((b2FrictionJoint*) mJoint)->GetAnchorB();
}


void FrictionJoint::SetMaxForce(float maxForce)
{
	((b2FrictionJoint*) mJoint)->SetMaxForce(maxForce);
}
void FrictionJoint::SetMaxTorque(float maxTorque)
{
	((b2FrictionJoint*) mJoint)->SetMaxTorque(maxTorque);
}
