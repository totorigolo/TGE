#include "WeldJoint.h"
#include "../../Tools/utils.h"

//Ctor
WeldJoint::WeldJoint(PhysicManager *physicMgr, b2Body *b1, b2Body *b2, b2Vec2 anchor, float frequencyHz, float damping, bool collideconnected, sf::Color const& color)
	: Joint(physicMgr), mColor(color)
{
	assert(mPhysicMgr && "n'est pas valide.");

	mBodyA = b1;
	mBodyB = b2;

	if (mBodyA && mBodyB)
	{
		b2WeldJointDef jointDef;
		jointDef.Initialize(mBodyA, mBodyB, mBodyA->GetWorldPoint(anchor));
		jointDef.frequencyHz = frequencyHz;
		jointDef.dampingRatio = damping;
		jointDef.collideConnected = collideconnected;
		mJoint = (b2WeldJoint*) mPhysicMgr->CreateJoint(&jointDef, this);
		
		//mBodyA->RegisterJoint(this);
		//mBodyB->RegisterJoint(this);
		mIsNull = false;
	}

	(*this)[0].color = mColor;
	(*this)[1].color = mColor;
}

// Dtor
WeldJoint::~WeldJoint(void)
{
}

// Mets à jour le VertexArray
void WeldJoint::Update()
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
float WeldJoint::GetReferenceAngle() const
{
	return ((b2WeldJoint*) mJoint)->GetReferenceAngle();
}
float WeldJoint::GetFrequencyHz() const
{
	return ((b2WeldJoint*) mJoint)->GetFrequency();
}
float WeldJoint::GetDampingRatio() const
{
	return ((b2WeldJoint*) mJoint)->GetDampingRatio();
}
b2Vec2 WeldJoint::GetAnchorA() const
{
	return ((b2WeldJoint*) mJoint)->GetAnchorA();
}
b2Vec2 WeldJoint::GetAnchorB() const
{
	return ((b2WeldJoint*) mJoint)->GetAnchorB();
}

void WeldJoint::SetFrequencyHz(float frequencyHz)
{
	((b2WeldJoint*) mJoint)->SetFrequency(frequencyHz);
}
void WeldJoint::SetDampingRatio(float damping)
{
	((b2WeldJoint*) mJoint)->SetDampingRatio(damping);
}
