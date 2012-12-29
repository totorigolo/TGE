#include "Body.h"
#include "../../Tools/utils.h"
#include "../../Lights/LightManager.h"
#include <cassert>

//Ctor
Body::Body(World *world, BodyType type)
	: mWorld(world), mBody(nullptr), mShape(nullptr), mIsNull(true), mIsDrawable(true)
	, mType(type), mItsMySprite(false), mHull(nullptr), mEntity(nullptr)
{
	// Vérifie les objets passés
	assert(mWorld && "World invalid!");

	mSprite = new sf::Sprite;
	mItsMySprite = true;
}

// Dtor
Body::~Body(void)
{
	// Supprime le Hull
	if (mHull)
	{
		LightManager::GetInstance().DeleteHull(mHull);
		mHull = nullptr;
	}

	// Supprime le Sprite
	if (mItsMySprite && mSprite)
		delete mSprite;

	// Se déclare nullptr
	mIsNull = true;
}

// Met à jour le body
void Body::Update()
{
	// Ne fait rien
}

// Retourne true si l'objet est dedans
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
	assert(mWorld && "pointeur invalide !");

	for (auto it = mJointList.begin(); it != mJointList.end(); )
	{
		auto it2 = it; it2++;
		mWorld->DestroyJoint(*it);
		it = it2;
	}
}

// Accesseurs
void Body::SetSprite(sf::Sprite *sprite)
{
	if (mItsMySprite && mSprite)
		delete mSprite;

	mSprite = sprite;
	mItsMySprite = false;
}
const sf::Sprite* Body::GetSprite() const
{
	return mSprite;
}

void Body::SetBody(b2Body *body) // NE PAS UTILISER
{
	mBody = body;
}

void Body::SetType(BodyType type)
{
	mType = type;
	if (mType == BodyType::Bullet)
		mBody->SetBullet(true);
	else
		mBody->SetBullet(false);
}

void Body::SetEntity(Entity *entity)
{
	mEntity = entity;
}

b2AABB Body::GetBodyAABB() const
{
	return b2AABB();
}
b2Vec2 Body::GetBodySize() const
{
	return sf2b2Vec(u2f(mSprite->getTexture()->getSize()), mWorld->GetMPP());
}
float Body::GetBodyAngle() const
{
	return mBody->GetAngle() * DPR;
}
b2Vec2 Body::GetBodyPosition() const
{
	return mBody->GetPosition();
}

void Body::SetHull(Hull *hull)
{
	mHull = hull;
}

void Body::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
	target.draw(*mSprite, states);
}
