#include "FrictionJoint.h"
#include "../../Tools/utils.h"

//Ctor
FrictionJoint::FrictionJoint(PhysicManager *physicMgr, b2Body *b1, b2Vec2 pt1, b2Body *b2, b2Vec2 p2, float maxForce, float maxTorque, bool collideconnected)
	: Joint(physicMgr)
{
	assert(mPhysicMgr && "n'est pas valide.");
	assert(b1 && "n'est pas valide.");
	assert(b2 && "n'est pas valide.");
	
	mPhysicMgr->RegisterJoint(this);

	b2FrictionJointDef jointDef;
	jointDef.bodyA = b1;
	jointDef.bodyB = b2;
	jointDef.localAnchorA = pt1;
	jointDef.localAnchorB = p2;
	jointDef.maxForce = maxForce;
	jointDef.maxTorque = maxTorque;
	jointDef.collideConnected = collideconnected;
	mJoint = mPhysicMgr->Createb2Joint(&jointDef);
	mJoint->SetUserData(this);
	
	mIsAlive = true;
	
	b1->SetAwake(true);
	b2->SetAwake(true);
}

// Dtor
FrictionJoint::~FrictionJoint(void)
{
}

// Accesseurs
float FrictionJoint::GetMaxForce() const
{
	return ((b2FrictionJoint*) mJoint)->GetMaxForce();
}
float FrictionJoint::GetMaxTorque() const
{
	return ((b2FrictionJoint*) mJoint)->GetMaxTorque();
}
b2Vec2 FrictionJoint::GetAnchorA() const
{
	return ((b2FrictionJoint*) mJoint)->GetAnchorA();
}
b2Vec2 FrictionJoint::GetAnchorB() const
{
	return ((b2FrictionJoint*) mJoint)->GetAnchorB();
}


void FrictionJoint::SetMaxForce(float maxForce)
{
	((b2FrictionJoint*) mJoint)->SetMaxForce(maxForce);
}
void FrictionJoint::SetMaxTorque(float maxTorque)
{
	((b2FrictionJoint*) mJoint)->SetMaxTorque(maxTorque);
}
