#include "RopeJoint.h"
#include "../../Tools/utils.h"

//Ctor
RopeJoint::RopeJoint(b2Body *b1, b2Vec2 pt1, b2Body *b2, b2Vec2 p2, float maxLength, bool collideconnected)
{
	assert(b1 && "n'est pas valide.");
	assert(b2 && "n'est pas valide.");
	
	mPhysicMgr.RegisterJoint(this);

	b2RopeJointDef jointDef;
	jointDef.bodyA = b1;
	jointDef.bodyB = b2;
	jointDef.localAnchorA = pt1;
	jointDef.localAnchorB = p2;
	jointDef.maxLength = maxLength;
	jointDef.collideConnected = collideconnected;
	mJoint = mPhysicMgr.Createb2Joint(&jointDef);
	mJoint->SetUserData(this);
	
	mIsAlive = true;
	
	b1->SetAwake(true);
	b2->SetAwake(true);
}

// Dtor
RopeJoint::~RopeJoint(void)
{
}

// Accesseurs
float RopeJoint::GetMaxLength() const
{
	return ((b2RopeJoint*) mJoint)->GetMaxLength();
}

void RopeJoint::SetMaxLength(float maxLenght)
{
	GetBodyA()->SetAwake(true);
	GetBodyB()->SetAwake(true);
	((b2RopeJoint*) mJoint)->SetMaxLength(maxLenght);
}
