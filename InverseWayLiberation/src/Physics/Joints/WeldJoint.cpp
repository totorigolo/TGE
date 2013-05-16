#include "WeldJoint.h"
#include "../PhysicManager.h"
#include "../../Tools/utils.h"
#include "../../Tools/Error.h"

//Ctor
WeldJoint::WeldJoint(const WeldJointDef &def)
{
	myAssert(def.body1, "Le b2Body n'existe pas.");
	myAssert(def.body2, "Le b2Body n'existe pas.");
	
	mPhysicMgr.RegisterJoint(this);

	b2WeldJointDef jointDef;
	jointDef.Initialize(def.body1, def.body2, def.body1->GetWorldPoint(def.anchor));
	jointDef.frequencyHz = def.frequencyHz;
	jointDef.dampingRatio = def.damping;
	jointDef.collideConnected = def.collideconnected;
	mJoint = (b2WeldJoint*) mPhysicMgr.Createb2Joint(&jointDef);
	mJoint->SetUserData(this);
	
	mIsAlive = true;
	
	def.body1->SetAwake(true);
	def.body2->SetAwake(true);
}

// Dtor
WeldJoint::~WeldJoint(void)
{
}

// Accesseurs
float WeldJoint::GetReferenceAngle() const
{
	return ((b2WeldJoint*) mJoint)->GetReferenceAngle();
}
float WeldJoint::GetFrequencyHz() const
{
	return ((b2WeldJoint*) mJoint)->GetFrequency();
}
float WeldJoint::GetDampingRatio() const
{
	return ((b2WeldJoint*) mJoint)->GetDampingRatio();
}
b2Vec2 WeldJoint::GetAnchorA() const
{
	return ((b2WeldJoint*) mJoint)->GetAnchorA();
}
b2Vec2 WeldJoint::GetAnchorB() const
{
	return ((b2WeldJoint*) mJoint)->GetAnchorB();
}

void WeldJoint::SetFrequencyHz(float frequencyHz)
{
	((b2WeldJoint*) mJoint)->SetFrequency(frequencyHz);
}
void WeldJoint::SetDampingRatio(float damping)
{
	((b2WeldJoint*) mJoint)->SetDampingRatio(damping);
}
