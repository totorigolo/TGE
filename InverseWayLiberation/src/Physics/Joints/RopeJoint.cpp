#include "RopeJoint.h"
#include "../../utils.h"

//Ctor
RopeJoint::RopeJoint(World *world, Body *b1, b2Vec2 p1, Body *b2, b2Vec2 p2, float maxLength, bool collideconnected, sf::Color const& color)
	: Joint(world), mColor(color)
{
	mBodyA = b1;
	mBodyB = b2;

	if (mWorld && mBodyA && mBodyB)
	{
		b2RopeJointDef jointDef;
		jointDef.bodyA = mBodyA->GetBody();
		jointDef.bodyB = mBodyB->GetBody();
		jointDef.localAnchorA = p1;
		jointDef.localAnchorB = p2;
		jointDef.maxLength = maxLength;
		jointDef.collideConnected = collideconnected;
		mJoint = (b2RopeJoint*) mWorld->CreateJoint(&jointDef, this);
		
		mBodyA->RegisterJoint(this);
		mBodyB->RegisterJoint(this);
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
	if (mBodyA && mBodyB)
	{
		if (!mBodyA->IsNull() && !mBodyB->IsNull())
		{
			(*this)[0].position = b22sfVec(mJoint->GetAnchorA(), mWorld->GetPPM());
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
float RopeJoint::GetMaxLength() const
{
	return ((b2RopeJoint*) mJoint)->GetMaxLength();
}

void RopeJoint::SetMaxLength(float maxLenght)
{
	mBodyA->GetBody()->SetAwake(true);
	mBodyB->GetBody()->SetAwake(true);
	((b2RopeJoint*) mJoint)->SetMaxLength(maxLenght);
}
