#include "MouseJoint.h"
#include "../../utils.h"

//Ctor
MouseJoint::MouseJoint(World *world, Body *body, Body *ground, b2Vec2 target, float maxForce, float frequencyHz, float damping, sf::Color const& color)
	: Joint(world), mColor(color)
{
	mBodyA = ground;
	mBodyB = body;

	if (mWorld && mBodyA && mBodyB)
	{
		b2MouseJointDef jointDef;
		jointDef.bodyA = mBodyA->GetBody(); // Le body A ne sert pas, mais doit exister
		jointDef.bodyB = mBodyB->GetBody(); // Le body utilis� est le B
		jointDef.target = target;
		jointDef.dampingRatio = damping;
		jointDef.frequencyHz = frequencyHz;
		jointDef.collideConnected = true;
		jointDef.maxForce = maxForce;
		mJoint = (b2MouseJoint*) mWorld->CreateJoint(&jointDef, this);
		mBodyB->GetBody()->SetAwake(true);
		mBodyB->GetBody()->SetBullet(true);
		mIsNull = false;
		mBodyB->RegisterJoint(this);
	}

	(*this)[0].color = mColor;
	(*this)[1].color = mColor;
}

// Dtor
MouseJoint::~MouseJoint(void)
{
	if (mBodyB)
		if (mBodyB->GetBody())
			mBodyB->GetBody()->SetBullet(false);
}

// Mets � jour le VertexArray
void MouseJoint::Update()
{
	if (mBodyA && mBodyB)
	{
		if (!mBodyA->IsNull() && !mBodyB->IsNull())
		{
			(*this)[0].position = b22sfVec(((b2MouseJoint*) mJoint)->GetTarget(), mWorld->GetPPM());
			(*this)[1].position = b22sfVec(mJoint->GetAnchorB(), mWorld->GetPPM());
		}
		else
		{
			mWorld->DestroyJoint(this, false);
			delete this;
		}
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
Body const* MouseJoint::GetAttachedBody() const
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
