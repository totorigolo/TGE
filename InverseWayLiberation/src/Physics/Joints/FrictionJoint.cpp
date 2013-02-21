#include "FrictionJoint.h"
#include "../../Tools/utils.h"

//Ctor
FrictionJoint::FrictionJoint(PhysicManager *physicMgr, b2Body *b1, b2Vec2 pt1, b2Body *b2, b2Vec2 p2, float maxForce, float maxTorque, bool collideconnected, sf::Color const& color)
	: Joint(physicMgr), mColor(color)
{
	assert(mPhysicMgr && "n'est pas valide.");

	mBodyA = b1;
	mBodyB = b2;

	if (mBodyA && mBodyB)
	{
		b2FrictionJointDef jointDef;
		jointDef.bodyA = mBodyA;
		jointDef.bodyB = mBodyB;
		jointDef.localAnchorA = pt1;
		jointDef.localAnchorB = p2;
		jointDef.maxForce = maxForce;
		jointDef.maxTorque = maxTorque;
		jointDef.collideConnected = collideconnected;
		mJoint = (b2FrictionJoint*) mPhysicMgr->CreateJoint(&jointDef, this);
		
		mBodyA->SetBullet(true);
		mBodyB->SetBullet(true);
		
		//mBodyA->RegisterJoint(this);
		//mBodyB->RegisterJoint(this);
		mIsNull = false;
	}

	(*this)[0].color = mColor;
	(*this)[1].color = mColor;
}

// Dtor
FrictionJoint::~FrictionJoint(void)
{
	if (mBodyA)
		mBodyA->SetBullet(false);
	if (mBodyB)
		mBodyB->SetBullet(false);
}

// Mets � jour le VertexArray
void FrictionJoint::Update()
{
	Joint::Update();

	if (mBodyA && mBodyB && !mIsNull)
	{
		(*this)[0].position = b22sfVec(mJoint->GetAnchorA(), mPhysicMgr->GetPPM());
		(*this)[1].position = b22sfVec(mJoint->GetAnchorB(), mPhysicMgr->GetPPM());
	}
	else
	{
		//mPhysicMgr->DestroyJoint(this, false);
		//delete this;
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
