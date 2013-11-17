#include "GearJoint.h"
#include "../PhysicManager.h"
#include "../../Tools/utils.h"
#include "../../Tools/Error.h"

//Ctor
GearJoint::GearJoint(const GearJointDef &def)
{
	myAssert(def.body1, "Le b2Body n'existe pas.");
	myAssert(def.body2, "Le b2Body n'existe pas.");

	Joint *joint1 = mPhysicMgr.GetJoint(def.joint1);
	Joint *joint2 = mPhysicMgr.GetJoint(def.joint2);
	
	myAssert(joint1, "Le joint n'existe pas.");
	myAssert(joint2, "Le joint n'existe pas.");

	mPhysicMgr.RegisterJoint(this);

	b2GearJointDef jointDef;
	jointDef.bodyA = def.body1;
	jointDef.bodyB = def.body2;
	jointDef.joint1 = joint1->GetJoint();
	jointDef.joint2 = joint2->GetJoint();
	jointDef.ratio = def.ratio;
	jointDef.collideConnected = def.collideconnected;
	mJoint = mPhysicMgr.Createb2Joint(&jointDef);
	mJoint->SetUserData(this);
	
	joint1->RegisterLinkedJoint(mID);
	joint2->RegisterLinkedJoint(mID);

	mIsAlive = true;
	mType = JointType::GearJoint;
	
	def.body1->SetAwake(true);
	def.body2->SetAwake(true);
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
b2Joint *const GearJoint::GetJoint1()
{
	return ((b2GearJoint*) mJoint)->GetJoint1();
}
b2Joint const* GearJoint::GetJoint1() const
{
	return ((b2GearJoint*) mJoint)->GetJoint1();
}
b2Joint *const GearJoint::GetJoint2()
{
	return ((b2GearJoint*) mJoint)->GetJoint2();
}
b2Joint const* GearJoint::GetJoint2() const
{
	return ((b2GearJoint*) mJoint)->GetJoint2();
}
