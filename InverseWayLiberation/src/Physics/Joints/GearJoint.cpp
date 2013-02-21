#include "GearJoint.h"
#include "../../Tools/utils.h"

//Ctor
GearJoint::GearJoint(PhysicManager *physicMgr, b2Body *b1, b2Body *b2, b2Joint *j1, b2Joint *j2, float ratio, bool collideconnected, sf::Color const& color)
	: Joint(physicMgr), mColor(color)
{
	assert(mPhysicMgr && "n'est pas valide.");

	mBodyA = b1;
	mBodyB = b2;

	if (mBodyA && mBodyB)
	{
		b2GearJointDef jointDef;
		jointDef.bodyA = mBodyA;
		jointDef.bodyB = mBodyB;
		jointDef.joint1 = j1;
		jointDef.joint2 = j2;
		jointDef.ratio = ratio;
		jointDef.collideConnected = collideconnected;
		mJoint = (b2GearJoint*) mPhysicMgr->CreateJoint(&jointDef, this);
		
		//mBodyA->RegisterJoint(this);
		//mBodyB->RegisterJoint(this);
		
		//j1->RegisterJoint(this);
		//j2->RegisterJoint(this);

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
float GearJoint::GetRatio() const
{
	return ((b2GearJoint*) mJoint)->GetRatio();
}
b2Joint *GearJoint::GetJoint1()
{
	return ((b2GearJoint*) mJoint)->GetJoint1();
}
b2Joint const* GearJoint::GetJoint1() const
{
	return ((b2GearJoint*) mJoint)->GetJoint1();
}
b2Joint *GearJoint::GetJoint2()
{
	return ((b2GearJoint*) mJoint)->GetJoint2();
}
b2Joint const* GearJoint::GetJoint2() const
{
	return ((b2GearJoint*) mJoint)->GetJoint2();
}
