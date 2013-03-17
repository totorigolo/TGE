#include "MouseJoint.h"
#include "../../Tools/utils.h"

//Ctor
MouseJoint::MouseJoint(PhysicManager *physicMgr, b2Body *body, b2Body *ground, b2Vec2 target, float maxForce, float frequencyHz, float damping, sf::Color const& color)
	: Joint(physicMgr), mColor(color)
{
	assert(mPhysicMgr && "n'est pas valide.");
	assert(body && "n'est pas valide.");
	assert(ground && "n'est pas valide.");
	
	mPhysicMgr->RegisterJoint(this);

	b2MouseJointDef jointDef;
	jointDef.bodyA = ground; // Le body A ne sert pas, mais doit exister
	jointDef.bodyB = body; // Le body utilis� est le B
	jointDef.target = target;
	jointDef.dampingRatio = damping;
	jointDef.frequencyHz = frequencyHz;
	jointDef.collideConnected = true;
	jointDef.maxForce = maxForce;
	mJoint = mPhysicMgr->Createb2Joint(&jointDef);
	mJoint->SetUserData(this);

	body->SetAwake(true);
	body->SetBullet(true);

	mIsAlive = true;

	(*this)[0].color = mColor;
	(*this)[1].color = mColor;
}

// Dtor
MouseJoint::~MouseJoint(void)
{
	if (GetBodyB())
		GetBodyB()->SetBullet(false);
}

// Mets � jour le VertexArray
void MouseJoint::Update()
{
	Joint::Update();

	if (mIsAlive)
	{
		(*this)[0].position = b22sfVec(((b2MouseJoint*) mJoint)->GetTarget(), mPhysicMgr->GetPPM());
		(*this)[1].position = b22sfVec(mJoint->GetAnchorB(), mPhysicMgr->GetPPM());
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
