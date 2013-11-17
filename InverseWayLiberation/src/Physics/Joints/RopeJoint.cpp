#include "RopeJoint.h"
#include "../PhysicManager.h"
#include "../../Tools/utils.h"
#include "../../Tools/Error.h"

//Ctor
RopeJoint::RopeJoint(const RopeJointDef &def)
{
	myAssert(def.body1, "Le b2Body n'existe pas.");
	myAssert(def.body2, "Le b2Body n'existe pas.");
	
	mPhysicMgr.RegisterJoint(this);

	b2RopeJointDef jointDef;
	jointDef.bodyA = def.body1;
	jointDef.bodyB = def.body2;
	jointDef.localAnchorA = def.point1;
	jointDef.localAnchorB = def.point2;
	jointDef.maxLength = def.maxLength;
	jointDef.collideConnected = def.collideconnected;
	mJoint = mPhysicMgr.Createb2Joint(&jointDef);
	mJoint->SetUserData(this);
	
	mIsAlive = true;
	mType = JointType::RopeJoint;
	
	def.body1->SetAwake(true);
	def.body2->SetAwake(true);
}

// Dtor
RopeJoint::~RopeJoint(void)
{
}

// Accesseurs
float RopeJoint::GetMaxLength() const
{
	return ((b2RopeJoint*) mJoint)->GetMaxLength();
}
void RopeJoint::SetMaxLength(float maxLenght)
{
	GetBodyA()->SetAwake(true);
	GetBodyB()->SetAwake(true);
	((b2RopeJoint*) mJoint)->SetMaxLength(maxLenght);
}
b2Vec2 RopeJoint::GetRelativeAnchorA() const
{
	return GetBodyA()->GetLocalPoint(((b2RopeJoint*)mJoint)->GetAnchorA());
}
b2Vec2 RopeJoint::GetRelativeAnchorB() const
{
	return GetBodyB()->GetLocalPoint(((b2RopeJoint*)mJoint)->GetAnchorB());
}
