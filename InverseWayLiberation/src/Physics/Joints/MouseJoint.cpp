#include "MouseJoint.h"
#include "../../Tools/utils.h"

//Ctor
MouseJoint::MouseJoint(PhysicManager *physicMgr, b2Body *body, b2Body *ground, b2Vec2 target, float maxForce, float frequencyHz, float damping, sf::Color const& color)
	: Joint(physicMgr), mColor(color)
{
	assert(mPhysicMgr && "n'est pas valide.");

	mBodyA = ground;
	mBodyB = body;

	if (mBodyA && mBodyB)
	{
		b2MouseJointDef jointDef;
		jointDef.bodyA = mBodyA; // Le body A ne sert pas, mais doit exister
		jointDef.bodyB = mBodyB; // Le body utilisé est le B
		jointDef.target = target;
		jointDef.dampingRatio = damping;
		jointDef.frequencyHz = frequencyHz;
		jointDef.collideConnected = true;
		jointDef.maxForce = maxForce;
		mJoint = (b2MouseJoint*) mPhysicMgr->CreateJoint(&jointDef, this);
		mBodyB->SetAwake(true);
		mBodyB->SetBullet(true);
		mIsNull = false;
		//mBodyB->RegisterJoint(this);
	}

	(*this)[0].color = mColor;
	(*this)[1].color = mColor;
}

// Dtor
MouseJoint::~MouseJoint(void)
{
	if (mBodyB)
		mBodyB->SetBullet(false);
}

// Mets à jour le VertexArray
void MouseJoint::Update()
{
	Joint::Update();

	if (mBodyA && mBodyB && !mIsNull)
	{
		(*this)[0].position = b22sfVec(((b2MouseJoint*) mJoint)->GetTarget(), mPhysicMgr->GetPPM());
		(*this)[1].position = b22sfVec(mJoint->GetAnchorB(), mPhysicMgr->GetPPM());
	}
	else
	{
		//mPhysicMgr->DestroyJoint(this, false);
		//delete this;
	}
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
