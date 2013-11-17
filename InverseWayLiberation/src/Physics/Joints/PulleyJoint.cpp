#include "PulleyJoint.h"
#include "../PhysicManager.h"
#include "../../Tools/utils.h"
#include "../../Tools/Error.h"

//Ctor
PulleyJoint::PulleyJoint(const PulleyJointDef &def)
{
	myAssert(def.body1, "Le b2Body n'existe pas.");
	myAssert(def.body2, "Le b2Body n'existe pas.");
	
	mPhysicMgr.RegisterJoint(this);

	b2PulleyJointDef jointDef;
	jointDef.Initialize(def.body1, def.body2, def.groundP1, def.groundP2, def.body1->GetWorldPoint(def.point1), def.body2->GetWorldPoint(def.point2), def.ratio);
	jointDef.collideConnected = def.collideconnected;
	mJoint = mPhysicMgr.Createb2Joint(&jointDef);
	mJoint->SetUserData(this);
	
	mIsAlive = true;
	mType = JointType::PulleyJoint;
	
	def.body1->SetAwake(true);
	def.body2->SetAwake(true);
}

// Dtor
PulleyJoint::~PulleyJoint(void)
{
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
