#include "PulleyJoint.h"
#include "../../utils.h"

//Ctor
PulleyJoint::PulleyJoint(World *world, Body *b1, b2Vec2 p1, Body *b2, b2Vec2 p2, b2Vec2 groundP1, b2Vec2 groundP2, float ratio, bool collideconnected, sf::Color const& color)
	: Joint(world), mColor(color)
{
	mBodyA = b1;
	mBodyB = b2;

	if (mWorld && mBodyA && mBodyB)
	{
		b2PulleyJointDef jointDef;
		jointDef.Initialize(mBodyA->GetBody(), mBodyB->GetBody(), groundP1, groundP2, mBodyA->GetBody()->GetWorldPoint(p1), mBodyB->GetBody()->GetWorldPoint(p2), ratio);
		jointDef.collideConnected = collideconnected;
		mJoint = (b2PulleyJoint*) mWorld->CreateJoint(&jointDef, this);
		
		mBodyA->RegisterJoint(this);
		mBodyB->RegisterJoint(this);
		mIsNull = false;
	}

	this->resize(4U);
	this->setPrimitiveType(sf::Lines);
	(*this)[0].color = mColor;
	(*this)[1].color = mColor;
	(*this)[2].color = mColor;
	(*this)[3].color = mColor;
}

// Dtor
PulleyJoint::~PulleyJoint(void)
{
}

// Mets à jour le VertexArray
void PulleyJoint::Update()
{
	if (mBodyA && mBodyB)
	{
		if (!mBodyA->IsNull() && !mBodyB->IsNull())
		{
			(*this)[0].position = b22sfVec(GetGroundAnchorA(), mWorld->GetPPM());
			(*this)[1].position = b22sfVec(mJoint->GetAnchorA(), mWorld->GetPPM());

			(*this)[2].position = b22sfVec(GetGroundAnchorB(), mWorld->GetPPM());
			(*this)[3].position = b22sfVec(mJoint->GetAnchorB(), mWorld->GetPPM());
		}
		else
		{
			mWorld->DestroyJoint(this, false);
			delete this;
		}
	}
}

// Accesseurs
float PulleyJoint::GetRatio() const
{
	return ((b2PulleyJoint*) mJoint)->GetRatio();
}
float PulleyJoint::GetLenghtA() const
{
	return ((b2PulleyJoint*) mJoint)->GetLengthA();
}
float PulleyJoint::GetLenghtB() const
{
	return ((b2PulleyJoint*) mJoint)->GetLengthB();
}
b2Vec2 PulleyJoint::GetAnchorA() const
{
	return ((b2PulleyJoint*) mJoint)->GetAnchorA();
}
b2Vec2 PulleyJoint::GetAnchorB() const
{
	return ((b2PulleyJoint*) mJoint)->GetAnchorB();
}
b2Vec2 PulleyJoint::GetGroundAnchorA() const
{
	return ((b2PulleyJoint*) mJoint)->GetGroundAnchorA();
}
b2Vec2 PulleyJoint::GetGroundAnchorB() const
{
	return ((b2PulleyJoint*) mJoint)->GetGroundAnchorB();
}
