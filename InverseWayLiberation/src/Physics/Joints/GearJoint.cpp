#include "GearJoint.h"
#include "../../utils.h"

//Ctor
GearJoint::GearJoint(World *world, Body *b1, Body *b2, Joint *j1, Joint *j2, float ratio, bool collideconnected, sf::Color const& color)
	: Joint(world), mColor(color)
{
	mBodyA = b1;
	mBodyB = b2;

	if (mWorld && mBodyA && mBodyB)
	{
		b2GearJointDef jointDef;
		jointDef.bodyA = mBodyA->GetBody();
		jointDef.bodyB = mBodyB->GetBody();
		jointDef.joint1 = j1->GetJoint();
		jointDef.joint2 = j2->GetJoint();
		jointDef.ratio = ratio;
		jointDef.collideConnected = collideconnected;
		mJoint = (b2GearJoint*) mWorld->CreateJoint(&jointDef, this);
		
		mBodyA->RegisterJoint(this);
		mBodyB->RegisterJoint(this);
		
		j1->RegisterJoint(this);
		j2->RegisterJoint(this);

		mIsNull = false;
	}

	(*this)[0].color = mColor;
	(*this)[1].color = mColor;
}

// Dtor
GearJoint::~GearJoint(void)
{
}

// Mets à jour le VertexArray
void GearJoint::Update()
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
float GearJoint::GetRatio() const
{
	return ((b2GearJoint*) mJoint)->GetRatio();
}
Joint *GearJoint::GetJoint1()
{
	return (Joint*) ((b2GearJoint*) mJoint)->GetJoint1()->GetUserData();
}
Joint const* GearJoint::GetJoint1() const
{
	return (Joint*) ((b2GearJoint*) mJoint)->GetJoint1()->GetUserData();
}
Joint *GearJoint::GetJoint2()
{
	return (Joint*) ((b2GearJoint*) mJoint)->GetJoint2()->GetUserData();
}
Joint const* GearJoint::GetJoint2() const
{
	return (Joint*) ((b2GearJoint*) mJoint)->GetJoint2()->GetUserData();
}
