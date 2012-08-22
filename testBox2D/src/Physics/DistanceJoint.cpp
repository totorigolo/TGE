#include "DistanceJoint.h"
#include "../utils.h"

//Ctor
DistanceJoint::DistanceJoint(World *world, Body *b1, b2Vec2 p1, Body *b2, b2Vec2 p2, float frequencyHz, float damping, sf::Color const& color)
	: Joint(world), mColor(color)
{
	mBodyA = b1;
	mBodyB = b2;
	mAnchorA = b2Mul(b2Rot(mBodyA->GetBody()->GetAngle()), p1) + mBodyA->GetBody()->GetPosition();
	mAnchorB = b2Mul(b2Rot(mBodyB->GetBody()->GetAngle()), p2) + mBodyB->GetBody()->GetPosition();

	if (mWorld && mBodyA && mBodyB)
	{
		b2DistanceJointDef jointDef;
		jointDef.Initialize(mBodyA->GetBody(), mBodyB->GetBody(), mAnchorA, mAnchorB);
		jointDef.collideConnected = true;
		jointDef.frequencyHz = frequencyHz;
		jointDef.dampingRatio = damping;
		mJoint = (b2DistanceJoint*) mWorld->CreateJoint(&jointDef, this);
		
		mBodyA->RegisterJoint(this);
		mBodyB->RegisterJoint(this);
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
		(*this)[0].position = b22sfVec(mJoint->GetAnchorA(), mWorld->GetPPM());
		(*this)[1].position = b22sfVec(mJoint->GetAnchorB(), mWorld->GetPPM());
	}
}

// Accesseurs
float DistanceJoint::GetLenght() const
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
