#include "PulleyJoint.h"
#include "../../Tools/utils.h"

//Ctor
PulleyJoint::PulleyJoint(PhysicManager *physicMgr, b2Body *b1, b2Vec2 pt1, b2Body *b2, b2Vec2 p2, b2Vec2 groundP1, b2Vec2 groundP2, float ratio, bool collideconnected, sf::Color const& color)
	: Joint(physicMgr), mColor(color)
{
	assert(mPhysicMgr && "n'est pas valide.");
	assert(b1 && "n'est pas valide.");
	assert(b2 && "n'est pas valide.");
	
	mPhysicMgr->RegisterJoint(this);

	b2PulleyJointDef jointDef;
	jointDef.Initialize(b1, b2, groundP1, groundP2, b1->GetWorldPoint(pt1), b2->GetWorldPoint(p2), ratio);
	jointDef.collideConnected = collideconnected;
	mJoint = mPhysicMgr->Createb2Joint(&jointDef);
	mJoint->SetUserData(this);
	
	mIsAlive = true;
	
	b1->SetAwake(true);
	b2->SetAwake(true);

	// Règle le VertexArray
	this->resize(4U);
	this->setPrimitiveType(sf::Lines);
	(*this)[0].color = mColor;
	(*this)[1].color = mColor;
	(*this)[2].color = mColor;
	(*this)[3].color = mColor;
}

// Dtor
PulleyJoint::~PulleyJoint(void)
{
}

// Mets à jour le VertexArray
void PulleyJoint::Update()
{
	Joint::Update();

	if (mIsAlive)
	{
		(*this)[0].position = b22sfVec(GetGroundAnchorA(), mPhysicMgr->GetPPM());
		(*this)[1].position = b22sfVec(mJoint->GetAnchorA(), mPhysicMgr->GetPPM());

		(*this)[2].position = b22sfVec(GetGroundAnchorB(), mPhysicMgr->GetPPM());
		(*this)[3].position = b22sfVec(mJoint->GetAnchorB(), mPhysicMgr->GetPPM());
	}
}

// Accesseurs
float PulleyJoint::GetRatio() const
{
	return ((b2PulleyJoint*) mJoint)->GetRatio();
}
float PulleyJoint::GetLenghtA() const
{
	return ((b2PulleyJoint*) mJoint)->GetLengthA();
}
float PulleyJoint::GetLenghtB() const
{
	return ((b2PulleyJoint*) mJoint)->GetLengthB();
}
b2Vec2 PulleyJoint::GetAnchorA() const
{
	return ((b2PulleyJoint*) mJoint)->GetAnchorA();
}
b2Vec2 PulleyJoint::GetAnchorB() const
{
	return ((b2PulleyJoint*) mJoint)->GetAnchorB();
}
b2Vec2 PulleyJoint::GetGroundAnchorA() const
{
	return ((b2PulleyJoint*) mJoint)->GetGroundAnchorA();
}
b2Vec2 PulleyJoint::GetGroundAnchorB() const
{
	return ((b2PulleyJoint*) mJoint)->GetGroundAnchorB();
}
