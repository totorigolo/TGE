#include "GearJoint.h"
#include "../../Tools/utils.h"

//Ctor
GearJoint::GearJoint(b2Body *b1, b2Body *b2, int j1, int j2, float ratio, bool collideconnected)
{
	assert(b1 && "n'est pas valide.");
	assert(b2 && "n'est pas valide.");

	Joint *joint1 = mPhysicMgr.GetJoint(j1);
	Joint *joint2 = mPhysicMgr.GetJoint(j2);

	assert(joint1 && "le joint n'existe pas.");
	assert(joint2 && "le joint n'existe pas.");

	mPhysicMgr.RegisterJoint(this);

	b2GearJointDef jointDef;
	jointDef.bodyA = b1;
	jointDef.bodyB = b2;
	jointDef.joint1 = joint1->GetJoint();
	jointDef.joint2 = joint2->GetJoint();
	jointDef.ratio = ratio;
	jointDef.collideConnected = collideconnected;
	mJoint = mPhysicMgr.Createb2Joint(&jointDef);
	mJoint->SetUserData(this);
	
	joint1->RegisterLinkedJoint(mID);
	joint2->RegisterLinkedJoint(mID);

	mIsAlive = false;
	
	b1->SetAwake(true);
	b2->SetAwake(true);
}

// Dtor
GearJoint::~GearJoint(void)
{
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
