#include "DistanceJoint.h"
#include "../../Tools/utils.h"

//Ctor
DistanceJoint::DistanceJoint(World *world, Body *b1, b2Vec2 pt1, Body *b2, b2Vec2 p2, float frequencyHz, float damping, bool collideconnected, sf::Color const& color)
	: Joint(world), mColor(color)
{
	mBodyA = b1;
	mBodyB = b2;

	if (mWorld && mBodyA && mBodyB)
	{
		b2DistanceJointDef jointDef;
		jointDef.Initialize(mBodyA->GetBody(), mBodyB->GetBody(), mBodyA->GetBody()->GetWorldPoint(pt1), mBodyB->GetBody()->GetWorldPoint(p2));
		jointDef.collideConnected = collideconnected;
		jointDef.frequencyHz = frequencyHz;
		jointDef.dampingRatio = damping;
		mJoint = (b2DistanceJoint*) mWorld->CreateJoint(&jointDef, this);
		
		mBodyA->RegisterJoint(this);
		mBodyB->RegisterJoint(this);
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
	mBodyA->GetBody()->SetAwake(true);
	mBodyB->GetBody()->SetAwake(true);
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
