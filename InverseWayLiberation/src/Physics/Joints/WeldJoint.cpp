#include "WeldJoint.h"
#include "../../Tools/utils.h"

//Ctor
WeldJoint::WeldJoint(b2Body *b1, b2Body *b2, b2Vec2 anchor, float frequencyHz, float damping, bool collideconnected)
{
	assert(b1 && "n'est pas valide.");
	assert(b2 && "n'est pas valide.");
	
	mPhysicMgr.RegisterJoint(this);

	b2WeldJointDef jointDef;
	jointDef.Initialize(b1, b2, b1->GetWorldPoint(anchor));
	jointDef.frequencyHz = frequencyHz;
	jointDef.dampingRatio = damping;
	jointDef.collideConnected = collideconnected;
	mJoint = (b2WeldJoint*) mPhysicMgr.Createb2Joint(&jointDef);
	mJoint->SetUserData(this);
	
	mIsAlive = true;
	
	b1->SetAwake(true);
	b2->SetAwake(true);
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
