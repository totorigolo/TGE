#include "MouseJoint.h"
#include "../../Tools/utils.h"

//Ctor
MouseJoint::MouseJoint(b2Body *body, b2Body *ground, b2Vec2 target, float maxForce, float frequencyHz, float damping)
{
	assert(body && "n'est pas valide.");
	assert(ground && "n'est pas valide.");
	
	mPhysicMgr.RegisterJoint(this);

	b2MouseJointDef jointDef;
	jointDef.bodyA = ground; // Le body A ne sert pas, mais doit exister
	jointDef.bodyB = body; // Le body utilisé est le B
	jointDef.target = target;
	jointDef.dampingRatio = damping;
	jointDef.frequencyHz = frequencyHz;
	jointDef.collideConnected = true;
	jointDef.maxForce = maxForce;
	mJoint = mPhysicMgr.Createb2Joint(&jointDef);
	mJoint->SetUserData(this);

	body->SetAwake(true);
	body->SetBullet(true);

	mIsAlive = true;
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
	return ((b2MouseJoint*) mJoint)->GetAnchorB();
}
b2Vec2 MouseJoint::GetTarget() const
{
	return ((b2MouseJoint*) mJoint)->GetTarget();
}
