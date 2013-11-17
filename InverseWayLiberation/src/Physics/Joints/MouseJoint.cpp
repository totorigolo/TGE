#include "MouseJoint.h"
#include "../PhysicManager.h"
#include "../../Tools/utils.h"
#include "../../Tools/Error.h"

//Ctor
MouseJoint::MouseJoint(const MouseJointDef &def)
{
	myAssert(def.body, "Le b2Body n'existe pas.");
	myAssert(def.ground, "Le b2Body n'existe pas.");
	
	mPhysicMgr.RegisterJoint(this);

	b2MouseJointDef jointDef;
	jointDef.bodyA = def.ground; // Le body A ne sert pas, mais doit exister
	jointDef.bodyB = def.body; // Le body utilisé est le B
	jointDef.target = def.target;
	jointDef.dampingRatio = def.damping;
	jointDef.frequencyHz = def.frequencyHz;
	jointDef.collideConnected = true;
	jointDef.maxForce = def.maxForce;
	mJoint = mPhysicMgr.Createb2Joint(&jointDef);
	mJoint->SetUserData(this);

	def.body->SetAwake(true);
	def.body->SetBullet(true);

	mIsAlive = true;
	mType = JointType::MouseJoint;
}

// Dtor
MouseJoint::~MouseJoint(void)
{
	if (GetBodyB())
		GetBodyB()->SetBullet(false);
}

// Accesseurs
void MouseJoint::SetTarget(b2Vec2 const& target)
{
	((b2MouseJoint*) mJoint)->SetTarget(target);
}

float MouseJoint::GetMaxForce() const
{
	return ((b2MouseJoint*) mJoint)->GetMaxForce();
}
float MouseJoint::GetFrequencyHz() const
{
	return ((b2MouseJoint*) mJoint)->GetFrequency();
}
float MouseJoint::GetDampingRatio() const
{
	return ((b2MouseJoint*) mJoint)->GetDampingRatio();
}
b2Body const* MouseJoint::GetAttachedBody() const
{
	return this->GetBodyB();
}
b2Vec2 MouseJoint::GetAnchor() const
{
	return ((b2MouseJoint*)mJoint)->GetAnchorB();
}
b2Vec2 MouseJoint::GetTarget() const
{
	return ((b2MouseJoint*) mJoint)->GetTarget();
}
