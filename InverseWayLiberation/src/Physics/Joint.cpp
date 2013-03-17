#include "Joint.h"
#include "../Tools/utils.h"

//Ctor
Joint::Joint(PhysicManager *physicMgr)
	: mPhysicMgr(physicMgr), mJoint(nullptr), mID(-1),
	sf::VertexArray(sf::LinesStrip, 2U), mToDestroy(false), mIsAlive(false), mOwner(nullptr),
	mIsBreakableMaxForce(false), mIsBreakableMaxTorque(false), mMaxForceType(Null), mMaxForce(0.f), mMaxTorque(0.f)
{
	assert(mPhysicMgr && "n'est pas valide.");
}

// Dtor
Joint::~Joint(void)
{
	// N'existe plus
	mIsAlive = false;

	// Avertit le propriétaire
	if (mOwner)
	{
		mOwner->DependencyDestroyed(this);
		mOwner = nullptr;
	}

	// Supprime les joints dépendants
	this->DestroyAllLinkedJoints();

	// Supprime le joint
	if (mJoint)
	{
		mPhysicMgr->Destroyb2Joint(mJoint);
		mJoint = nullptr;
	}

	// N'a plus à être supprimé
	mToDestroy = false;
}

// Destruction de ce joint
void Joint::Destroy()
{
	mToDestroy = true;
	mIsAlive = false;
}

// Met à jour le body
void Joint::Update()
{
	// Ne se met à jour que si il existe
	if (!mIsAlive || !mJoint)
		return;
	
	// Gestion du joint cassable par Force
	if (mIsBreakableMaxForce)
	{
		b2Vec2 mf = mJoint->GetReactionForce(1.f / mPhysicMgr->GetTimeStep());
		
		if (mMaxForceType == Vector)
		{
			if (mMaxVecForce.x < mf.x && mMaxVecForce.y < mf.y)
				Destroy();
		}
		else if (mMaxForceType == Float)
		{
			if (mMaxForce < mf.Length())
				Destroy();
		}
	}

	// Gestion du joint cassable par Torque
	if (mIsBreakableMaxTorque && mIsAlive)
	{
		float mt = mJoint->GetReactionTorque(1.f / mPhysicMgr->GetTimeStep());
		
		if (mMaxTorque > mt)
				Destroy();
	}
}

// Définit si le joint est cassable
void Joint::SetBreakableByForce(bool breakable)
{
	mIsBreakableMaxForce = breakable;
}
void Joint::SetMaxForce(b2Vec2 maxForce)
{
	mMaxVecForce = maxForce;
	mMaxForceType = Vector;
}
void Joint::SetMaxForce(float maxForce)
{
	mMaxForce = maxForce;
	mMaxForceType = Float;
}
void Joint::SetBreakableByTorque(bool breakable)
{
	mIsBreakableMaxTorque = breakable;
}
void Joint::SetMaxTorque(float maxTorque)
{
	mMaxTorque = maxTorque;
}
bool Joint::IsBreakableMaxForce() const
{
	return mIsBreakableMaxForce;
}
float Joint::GetMaxForce() const
{
	return mMaxForce;
}
b2Vec2 Joint::GetMaxVecForce() const
{
	return mMaxVecForce;
}
bool Joint::IsBreakableMaxTorque() const
{
	return mIsBreakableMaxTorque;
}
float Joint::GetMaxTorque() const
{
	return mMaxTorque;
}

// Accesseurs
b2Vec2 Joint::GetReactionForce(float inv_dt) const
{
	return mJoint->GetReactionForce(inv_dt);
}
float Joint::GetReactionTorque(float inv_dt) const
{
	return mJoint->GetReactionTorque(inv_dt);
}

b2Body* Joint::GetBodyA()
{
	if (!mIsAlive)
		return nullptr;

	return mJoint->GetBodyA();
}
b2Body* Joint::GetBodyB()
{
	if (!mIsAlive)
		return nullptr;

	return mJoint->GetBodyB();
}
const b2Body* Joint::GetBodyA() const
{
	if (!mIsAlive)
		return nullptr;

	return mJoint->GetBodyA();
}
const b2Body* Joint::GetBodyB() const
{
	if (!mIsAlive)
		return nullptr;

	return mJoint->GetBodyB();
}

// Gestion des joints à supprimer avant celui-ci
void Joint::RegisterLinkedJoint(int jointID)
{
	assert(mPhysicMgr->JointExists(jointID) && "le joint n'existe pas.");

	mLinkedJointList.push_back(jointID);
}
void Joint::RemoveLinkedJoint(int jointID)
{
	mLinkedJointList.remove(jointID);
}
void Joint::DestroyLinkedJoint(int jointID)
{
	mLinkedJointList.remove(jointID);
	mPhysicMgr->DestroyJoint(jointID);
}
void Joint::DestroyAllLinkedJoints()
{
	for (auto it = mLinkedJointList.begin(); it != mLinkedJointList.end(); ++it)
	{
		mPhysicMgr->DestroyJoint(*it);
	}
	mLinkedJointList.clear();
}

