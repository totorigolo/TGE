#include "GearJoint.h"
#include "../../Tools/utils.h"

//Ctor
GearJoint::GearJoint(PhysicManager *physicMgr, b2Body *b1, b2Body *b2, int j1, int j2, float ratio, bool collideconnected, sf::Color const& color)
	: Joint(physicMgr), mColor(color)
{
	assert(mPhysicMgr && "n'est pas valide.");
	assert(b1 && "n'est pas valide.");
	assert(b2 && "n'est pas valide.");
	assert(mPhysicMgr->JointExists(j1) && "le joint n'existe pas.");
	assert(mPhysicMgr->JointExists(j2) && "le joint n'existe pas.");

	mPhysicMgr->RegisterJoint(this);

	b2GearJointDef jointDef;
	jointDef.bodyA = b1;
	jointDef.bodyB = b2;
	jointDef.joint1 = mPhysicMgr->GetJoint(j1)->GetJoint();
	jointDef.joint2 = mPhysicMgr->GetJoint(j2)->GetJoint();
	jointDef.ratio = ratio;
	jointDef.collideConnected = collideconnected;
	mJoint = mPhysicMgr->Createb2Joint(&jointDef);
	mJoint->SetUserData(this);
	
	mPhysicMgr->GetJoint(j1)->RegisterLinkedJoint(mID);
	mPhysicMgr->GetJoint(j2)->RegisterLinkedJoint(mID);

	mIsAlive = false;
	
	b1->SetAwake(true);
	b2->SetAwake(true);

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

	if (mIsAlive)
	{
		(*this)[0].position = b22sfVec(mJoint->GetAnchorA(), mPhysicMgr->GetPPM());
		(*this)[1].position = b22sfVec(mJoint->GetAnchorB(), mPhysicMgr->GetPPM());
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
