#include "Body.h"
#include "../../utils.h"

//Ctor
Body::Body(World *world)
	: mWorld(world), mBody(nullptr), mShape(nullptr), mIsNull(true), mIsDrawable(true)
{
}

// Dtor
Body::~Body(void)
{
	this->DestroyAllJoints();

	if (mWorld && mBody)
		mWorld->DestroyBody(this, false);

	mIsNull = true;
}

// Met à jour le body
void Body::Update()
{
	// Ne fait rien
}

// Retourne true si l'objet est trop loin
bool Body::IsInRange(b2Vec2 const& xw, b2Vec2 const& yh) const
{
	if (mBody)
		if (mBody->GetPosition().x < xw.x && mBody->GetPosition().x > xw.y && mBody->GetPosition().y < yh.x && mBody->GetPosition().y > yh.y)
			return true;
	return false;
}

// Gestion des joints
void Body::RegisterJoint(Joint *joint)
{
	if (joint)
	{
		mJointList.push_back(joint);
	}
}
void Body::RemoveJoint(Joint *joint)
{
	if (joint)
	{
		mJointList.remove(joint);
	}
}
void Body::DestroyJoint(Joint *joint, bool remove)
{
	if (joint)
	{
		if (remove)
			mJointList.remove(joint);
		mWorld->DestroyJoint(joint);
	}
}
void Body::DestroyAllJoints()
{
	for (auto it = mJointList.begin(); it != mJointList.end(); )
	{
		auto it2 = it; it2++;
		mWorld->DestroyJoint(*it);
		it = it2;
	}
}

// Accesseurs
void Body::SetBody(b2Body *body) // NE PAS UTILISER
{
	mBody = body;
}
