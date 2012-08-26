#include "DistanceJoint.h"
#include "../../utils.h"

//Ctor
DistanceJoint::DistanceJoint(World *world, Body *b1, b2Vec2 p1, Body *b2, b2Vec2 p2, float frequencyHz, float damping, bool collideconnected, sf::Color const& color)
	: Joint(world), mColor(color)
{
	mBodyA = b1;
	mBodyB = b2;
	mAnchorA = mBodyA->GetBody()->GetWorldPoint(p1);
	mAnchorB = mBodyB->GetBody()->GetWorldPoint(p2);

	if (mWorld && mBodyA && mBodyB)
	{
		b2DistanceJointDef jointDef;
		jointDef.Initialize(mBodyA->GetBody(), mBodyB->GetBody(), mAnchorA, mAnchorB);
		jointDef.collideConnected = collideconnected;
		jointDef.frequencyHz = frequencyHz;
		jointDef.dampingRatio = damping;
		mJoint = (b2DistanceJoint*) mWorld->CreateJoint(&jointDef, this);
		
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
DistanceJoint::~DistanceJoint(void)
{
	if (mBodyA)
		if (mBodyA->GetBody())
			mBodyA->GetBody()->SetBullet(false);
	if (mBodyB)
		if (mBodyB->GetBody())
			mBodyB->GetBody()->SetBullet(false);
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

void DistanceJoint::SetLenght(float lenght)
{
	mBodyA->GetBody()->SetAwake(true);
	mBodyB->GetBody()->SetAwake(true);
	((b2DistanceJoint*) mJoint)->SetLength(lenght);
}
void DistanceJoint::SetFrequencyHz(float frequencyHz)
{
	((b2DistanceJoint*) mJoint)->SetFrequency(frequencyHz);
}
void DistanceJoint::SetDampingRatio(float damping)
{
	((b2DistanceJoint*) mJoint)->SetDampingRatio(damping);
}
