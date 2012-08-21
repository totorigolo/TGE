#include "Joint.h"
#include "utils.h"

//Ctor
Joint::Joint(World *world)
	: mWorld(world), mJoint(nullptr), mBodyA(nullptr), mBodyB(nullptr), mAnchorA(0.f, 0.f), mAnchorB(0.f, 0.f),
	sf::VertexArray(sf::LinesStrip, 2U)
{
}

// Dtor
Joint::~Joint(void)
{
	if (mBodyA)
	{
		mBodyA->RemoveJoint(this);
		//mBodyA = nullptr;
	}
	if (mBodyB)
	{
		mBodyB->RemoveJoint(this);
		//mBodyB = nullptr;
	}
	if (mJoint)
	{
		mWorld->DestroyJoint(this, false);
		mJoint = nullptr;
	}
	mWorld = nullptr;
}

// Met à jour le body
void Joint::Update()
{
	// Ne fait rien
}

// Accesseurs
void Joint::SetJoint(b2Joint *joint) // NE PAS UTILISER
{
	mJoint = joint;
}
