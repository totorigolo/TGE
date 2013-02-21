#include "Joint.h"
#include "../Tools/utils.h"

//Ctor
Joint::Joint(PhysicManager *physicMgr)
	: mPhysicMgr(physicMgr), mJoint(nullptr), mBodyA(nullptr), mBodyB(nullptr),
	sf::VertexArray(sf::LinesStrip, 2U), mIsNull(true),
	mIsBreakableMaxForce(false), mIsBreakableMaxTorque(false), mMaxForceType(Null), mMaxForce(0.f), mMaxTorque(0.f)
{
	assert(mPhysicMgr && "n'est pas valide.");
}

// Dtor
Joint::~Joint(void)
{
	Destroy();
}

// Destruction de ce joint
void Joint::Destroy()
{
	if (!mIsNull)
	{
		if (mJoint)
		{
			mPhysicMgr->Destroyb2Joint(mJoint);
			mJoint = nullptr;
		}
		this->DestroyAllJoints();
		mIsNull = true;
	}
}

// Met à jour le body
void Joint::Update()
{
	if (mIsNull)
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
	if (mIsBreakableMaxTorque && !mIsNull)
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
		mPhysicMgr->DestroyJoint(joint);
	}
}
void Joint::DestroyAllJoints()
{
	for (auto it = mJointList.begin(); it != mJointList.end(); )
	{
		auto it2 = it; it2++;
		mPhysicMgr->DestroyJoint(*it);
		it = it2;
	}
	mJointList.clear();
}
