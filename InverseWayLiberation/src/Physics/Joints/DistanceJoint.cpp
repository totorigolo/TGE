#include "DistanceJoint.h"
#include "../../Tools/utils.h"
// TODO: Classe de communication / erreur

//Ctor
DistanceJoint::DistanceJoint(b2Body *b1, b2Vec2 pt1, b2Body *b2, b2Vec2 pt2, float frequencyHz, float damping, bool collideconnected)
{
	assert(b1 && "n'est pas valide.");
	assert(b2 && "n'est pas valide.");
	
	mPhysicMgr.RegisterJoint(this);

	b2DistanceJointDef jointDef;
	jointDef.bodyA = b1;
	jointDef.bodyB = b2;
	jointDef.localAnchorA = pt1;
	jointDef.localAnchorB = pt2;
	b2Vec2 d = b2->GetWorldPoint(pt2) - b1->GetWorldPoint(pt1);
	jointDef.length = d.Length();
	jointDef.collideConnected = collideconnected;
	jointDef.frequencyHz = frequencyHz;
	jointDef.dampingRatio = damping;
	mJoint = mPhysicMgr.Createb2Joint(&jointDef);
	mJoint->SetUserData(this);
	
	mIsAlive = true;
	
	b1->SetAwake(true);
	b2->SetAwake(true);
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
b2Vec2 DistanceJoint::GetAnchorA() const
{
	return ((b2DistanceJoint*) mJoint)->GetAnchorA();
}
b2Vec2 DistanceJoint::GetAnchorB() const
{
	return ((b2DistanceJoint*) mJoint)->GetAnchorB();
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
