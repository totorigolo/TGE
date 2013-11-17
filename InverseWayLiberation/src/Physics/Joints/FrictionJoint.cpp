#include "FrictionJoint.h"
#include "../PhysicManager.h"
#include "../../Tools/utils.h"
#include "../../Tools/Error.h"

//Ctor
FrictionJoint::FrictionJoint(const FrictionJointDef &def)
{
	myAssert(def.body1, "Le b2Body n'existe pas.");
	myAssert(def.body2, "Le b2Body n'existe pas.");
	
	mPhysicMgr.RegisterJoint(this);

	b2FrictionJointDef jointDef;
	jointDef.bodyA = def.body1;
	jointDef.bodyB = def.body2;
	jointDef.localAnchorA = def.point1;
	jointDef.localAnchorB = def.point2;
	jointDef.maxForce = def.maxFrictionForce;
	jointDef.maxTorque = def.maxFrictionTorque;
	jointDef.collideConnected = def.collideconnected;
	mJoint = mPhysicMgr.Createb2Joint(&jointDef);
	mJoint->SetUserData(this);
	
	mIsAlive = true;
	mType = JointType::FrictionJoint;
	
	def.body1->SetAwake(true);
	def.body2->SetAwake(true);
}

// Dtor
FrictionJoint::~FrictionJoint(void)
{
}

// Accesseurs
float FrictionJoint::GetMaxFrictionForce() const
{
	return ((b2FrictionJoint*) mJoint)->GetMaxForce();
}
float FrictionJoint::GetMaxFrictionTorque() const
{
	return ((b2FrictionJoint*) mJoint)->GetMaxTorque();
}
b2Vec2 FrictionJoint::GetRelativeAnchorA() const
{
	return GetBodyA()->GetLocalPoint(((b2FrictionJoint*) mJoint)->GetAnchorA());
}
b2Vec2 FrictionJoint::GetRelativeAnchorB() const
{
	return GetBodyB()->GetLocalPoint(((b2FrictionJoint*)mJoint)->GetAnchorB());
}


void FrictionJoint::SetMaxFrictionForce(float maxForce)
{
	((b2FrictionJoint*) mJoint)->SetMaxForce(maxForce);
}
void FrictionJoint::SetMaxFrictionTorque(float maxTorque)
{
	((b2FrictionJoint*) mJoint)->SetMaxTorque(maxTorque);
}
