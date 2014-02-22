#include "stdafx.h"
#include "Joint.h"
#include "PhysicManager.h"

//Ctor
Joint::Joint()
	: 
	mIsAlive(false), mToDestroy(false), mType(JointType::Null), mID(-1),
	mPhysicMgr(PhysicManager::GetInstance()), mJoint(nullptr),
	mIsBreakableMaxForce(false), mMaxForceType(ForceType::Null), mMaxForce(0.f), mIsBreakableMaxTorque(false), mMaxTorque(0.f)
{
}
Joint::Joint(const JointDef &def)
	: mJoint(nullptr), mID(-1),
	mToDestroy(false), mIsAlive(false),
	mPhysicMgr(PhysicManager::GetInstance())
{
	mIsBreakableMaxForce = def.isBreakableMaxForce;
	mMaxForceType = def.maxForceType;
	mMaxForce = def.maxForce;
	mMaxVecForce = def.maxVecForce;
	mIsBreakableMaxTorque = def.isBreakableMaxTorque;
	mMaxTorque = def.maxTorque;
}

// Dtor
Joint::~Joint(void)
{
	// N'existe plus
	mIsAlive = false;

	// Supprime les joints dépendants
	this->DestroyAllLinkedJoints();

	// Supprime le joint
	if (mJoint)
	{
		mPhysicMgr.Destroyb2Joint(mJoint);
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
		b2Vec2 mf = mJoint->GetReactionForce(1.f / mPhysicMgr.GetTimeStep());
		
		if (mMaxForceType == ForceType::Vector)
		{
			if (mMaxVecForce.x < mf.x && mMaxVecForce.y < mf.y)
				Destroy();
		}
		else if (mMaxForceType == ForceType::Float)
		{
			if (mMaxForce < mf.Length())
				Destroy();
		}
	}

	// Gestion du joint cassable par Torque
	if (mIsBreakableMaxTorque && mIsAlive)
	{
		float mt = mJoint->GetReactionTorque(1.f / mPhysicMgr.GetTimeStep());
		
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
	mMaxForceType = ForceType::Vector;
}
void Joint::SetMaxForce(float maxForce)
{
	mMaxForce = maxForce;
	mMaxForceType = ForceType::Float;
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
ForceType Joint::GetMaxForceType() const
{
	return mMaxForceType;
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
JointType Joint::GetType() const
{
	return mType;
}
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
	return mJoint->GetBodyA();
}
b2Body* Joint::GetBodyB()
{
	return mJoint->GetBodyB();
}
const b2Body* Joint::GetBodyA() const
{
	return mJoint->GetBodyA();
}
const b2Body* Joint::GetBodyB() const
{
	return mJoint->GetBodyB();
}
bool Joint::IsCollideConnected() const
{
	return mJoint->GetCollideConnected();
}

// Gestion des joints à supprimer avant celui-ci
void Joint::RegisterLinkedJoint(int jointID)
{
	myAssert(mPhysicMgr.JointExists(jointID), "Le joint #"+ Parser::intToString(jointID) +" n'existe pas.");

	mLinkedJointList.push_back(jointID);
}
void Joint::RemoveLinkedJoint(int jointID)
{
	mLinkedJointList.remove(jointID);
}
void Joint::DestroyLinkedJoint(int jointID)
{
	mLinkedJointList.remove(jointID);
	mPhysicMgr.DestroyJoint(jointID);
}
void Joint::DestroyAllLinkedJoints()
{
	for (auto it = mLinkedJointList.begin(); it != mLinkedJointList.end(); ++it)
	{
		mPhysicMgr.DestroyJoint(*it);
	}
	mLinkedJointList.clear();
}
const std::list<int>& Joint::GetLinkedJoints()
{
	return mLinkedJointList;
}
