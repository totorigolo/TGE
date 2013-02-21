#include "RopeJoint.h"
#include "../../Tools/utils.h"

//Ctor
RopeJoint::RopeJoint(PhysicManager *physicMgr, b2Body *b1, b2Vec2 pt1, b2Body *b2, b2Vec2 p2, float maxLength, bool collideconnected, sf::Color const& color)
	: Joint(physicMgr), mColor(color)
{
	assert(mPhysicMgr && "n'est pas valide.");

	mBodyA = b1;
	mBodyB = b2;

	if (mBodyA && mBodyB)
	{
		b2RopeJointDef jointDef;
		jointDef.bodyA = mBodyA;
		jointDef.bodyB = mBodyB;
		jointDef.localAnchorA = pt1;
		jointDef.localAnchorB = p2;
		jointDef.maxLength = maxLength;
		jointDef.collideConnected = collideconnected;
		mJoint = (b2RopeJoint*) mPhysicMgr->CreateJoint(&jointDef, this);
		
		//mBodyA->RegisterJoint(this);
		//mBodyB->RegisterJoint(this);
		mIsNull = false;
	}

	(*this)[0].color = mColor;
	(*this)[1].color = mColor;
}

// Dtor
RopeJoint::~RopeJoint(void)
{
}

// Mets à jour le VertexArray
void RopeJoint::Update()
{
	Joint::Update();

	if (mBodyA && mBodyB && !mIsNull)
	{
		(*this)[0].position = b22sfVec(mJoint->GetAnchorA(), mPhysicMgr->GetPPM());
		(*this)[1].position = b22sfVec(mJoint->GetAnchorB(), mPhysicMgr->GetPPM());
	}
	else
	{
		//mPhysicMgr->DestroyJoint(this, false);
		//delete this;
	}
}

// Accesseurs
float RopeJoint::GetMaxLength() const
{
	return ((b2RopeJoint*) mJoint)->GetMaxLength();
}

void RopeJoint::SetMaxLength(float maxLenght)
{
	mBodyA->SetAwake(true);
	mBodyB->SetAwake(true);
	((b2RopeJoint*) mJoint)->SetMaxLength(maxLenght);
}
