#include "DistanceJoint.h"
#include "../../Tools/utils.h"

//Ctor
DistanceJoint::DistanceJoint(PhysicManager *physicMgr, b2Body *b1, b2Vec2 pt1, b2Body *b2, b2Vec2 p2, float frequencyHz, float damping, bool collideconnected, sf::Color const& color)
	: Joint(physicMgr), mColor(color)
{
	assert(mPhysicMgr && "n'est pas valide.");

	mBodyA = b1;
	mBodyB = b2;

	if (mBodyA && mBodyB)
	{
		b2DistanceJointDef jointDef;
		jointDef.Initialize(mBodyA, mBodyB, mBodyA->GetWorldPoint(pt1), mBodyB->GetWorldPoint(p2));
		jointDef.collideConnected = collideconnected;
		jointDef.frequencyHz = frequencyHz;
		jointDef.dampingRatio = damping;
		mJoint = (b2DistanceJoint*) mPhysicMgr->CreateJoint(&jointDef, this);
		
		//mBodyA->RegisterJoint(this);
		//mBodyB->RegisterJoint(this);
		mIsNull = false;
	}

	(*this)[0].color = mColor;
	(*this)[1].color = mColor;
}

// Dtor
DistanceJoint::~DistanceJoint(void)
{
}

// Mets à jour le VertexArray
void DistanceJoint::Update()
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
float DistanceJoint::GetLength() const
{
	return ((b2DistanceJoint*) mJoint)->GetLength();
}
float DistanceJoint::GetFrequencyHz() const
{
	return ((b2DistanceJoint*) mJoint)->GetFrequency();
}
float DistanceJoint::GetDampingRatio() const
{
	return ((b2DistanceJoint*) mJoint)->GetDampingRatio();
}
b2Vec2 DistanceJoint::GetAnchorA() const
{
	return ((b2DistanceJoint*) mJoint)->GetAnchorA();
}
b2Vec2 DistanceJoint::GetAnchorB() const
{
	return ((b2DistanceJoint*) mJoint)->GetAnchorB();
}

void DistanceJoint::SetLength(float length)
{
	mBodyA->SetAwake(true);
	mBodyB->SetAwake(true);
	((b2DistanceJoint*) mJoint)->SetLength(length);
}
void DistanceJoint::SetFrequencyHz(float frequencyHz)
{
	((b2DistanceJoint*) mJoint)->SetFrequency(frequencyHz);
}
void DistanceJoint::SetDampingRatio(float damping)
{
	((b2DistanceJoint*) mJoint)->SetDampingRatio(damping);
}
