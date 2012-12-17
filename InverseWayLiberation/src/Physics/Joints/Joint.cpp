#include "Joint.h"
#include "../../Tools/utils.h"

//Ctor
Joint::Joint(World *world)
	: mWorld(world), mJoint(nullptr), mBodyA(nullptr), mBodyB(nullptr),
	sf::VertexArray(sf::LinesStrip, 2U), mIsNull(true)
{
}

// Dtor
Joint::~Joint(void)
{
	this->DestroyAllJoints();
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
	this->DestroyAllJoints();
	mWorld = nullptr;
	mIsNull = true;
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

b2Vec2 Joint::GetReactionForce(float inv_dt) const
{
	return mJoint->GetReactionForce(inv_dt);
}
float Joint::GetReactionTorque(float inv_dt) const
{
	return mJoint->GetReactionTorque(inv_dt);
}

// Gestion des joints à supprimer avant celui-ci
void Joint::RegisterJoint(Joint *joint)
{
	if (joint)
	{
		mJointList.push_back(joint);
	}
}
void Joint::RemoveJoint(Joint *joint)
{
	if (joint)
	{
		mJointList.remove(joint);
	}
}
void Joint::DestroyJoint(Joint *joint, bool remove)
{
	if (joint)
	{
		if (remove)
			mJointList.remove(joint);
		mWorld->DestroyJoint(joint);
	}
}
void Joint::DestroyAllJoints()
{
	for (auto it = mJointList.begin(); it != mJointList.end(); )
	{
		auto it2 = it; it2++;
		mWorld->DestroyJoint(*it);
		it = it2;
	}
}
