#include "DistanceJoint.h"
#include "../PhysicManager.h"
#include "../../Tools/utils.h"
#include "../../Tools/Error.h"

//Ctor
DistanceJoint::DistanceJoint(const DistanceJointDef &def)
{
	myAssert(def.body1, "Le b2Body n'existe pas.");
	myAssert(def.body2, "Le b2Body n'existe pas.");
	
	mPhysicMgr.RegisterJoint(this);

	b2DistanceJointDef jointDef;
	jointDef.bodyA = def.body1;
	jointDef.bodyB = def.body2;
	jointDef.localAnchorA = def.point1;
	jointDef.localAnchorB = def.point2;
	b2Vec2 d = def.body2->GetWorldPoint(def.point2) - def.body1->GetWorldPoint(def.point1);
	jointDef.length = d.Length();
	jointDef.collideConnected = def.collideconnected;
	jointDef.frequencyHz = def.frequencyHz;
	jointDef.dampingRatio = def.damping;
	mJoint = mPhysicMgr.Createb2Joint(&jointDef);
	mJoint->SetUserData(this);
	
	mIsAlive = true;
	mType = JointType::DistanceJoint;
	
	def.body1->SetAwake(true);
	def.body2->SetAwake(true);
}

// Dtor
DistanceJoint::~DistanceJoint(void)
{
}

// Accesseurs
float DistanceJoint::GetLength() const
{
	return ((b2DistanceJoint*) mJoint)->GetLength();
}
float DistanceJoint::GetFrequencyHz() const
{
	return ((b2DistanceJoint*) mJoint)->GetFrequency();
}
float DistanceJoint::GetDampingRatio() const
{
	return ((b2DistanceJoint*) mJoint)->GetDampingRatio();
}
b2Vec2 DistanceJoint::GetRelativeAnchorA() const
{
	return GetBodyA()->GetLocalPoint(((b2DistanceJoint*) mJoint)->GetAnchorA());
}
b2Vec2 DistanceJoint::GetRelativeAnchorB() const
{
	return GetBodyB()->GetLocalPoint(((b2DistanceJoint*) mJoint)->GetAnchorB());
}

void DistanceJoint::SetLength(float length)
{
	GetBodyA()->SetAwake(true);
	GetBodyB()->SetAwake(true);
	((b2DistanceJoint*) mJoint)->SetLength(length);
}
void DistanceJoint::SetFrequencyHz(float frequencyHz)
{
	((b2DistanceJoint*) mJoint)->SetFrequency(frequencyHz);
}
void DistanceJoint::SetDampingRatio(float damping)
{
	((b2DistanceJoint*) mJoint)->SetDampingRatio(damping);
}
