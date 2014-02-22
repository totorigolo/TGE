#include "stdafx.h"
#include "BaseBody.h"
#include "../Physics/PhysicManager.h"

// Ctor & dtor
BaseBody::BaseBody(int layer, unsigned int ID)
	: Entity(layer, ID), mCollisionType(CollisionType::Default),
	mBody(nullptr), mBodyIsCreated(false),
	mPhysicMgr(PhysicManager::GetInstance())
{
	// Défini le type de l'Entity
	mType = EntityType::BaseBody;
}
BaseBody::~BaseBody()
{
	Destroy();
}

// Mise à jour
void BaseBody::Update()
{
	// Si le body est valide
	if (mBody)
	{
		// Mise à jour de la texture
		mSprite.setPosition(b22sfVec(mBody->GetPosition(), mPhysicMgr.GetPPM()));
		mSprite.setRotation(- mBody->GetAngle() * DPR);
	}
}

// Destruction du body
void BaseBody::Destroy()
{
	if (mBody)
	{
		// Supprime le Body
		mPhysicMgr.DestroyBody(mBody);
		mBody = nullptr;
	}
	mBodyIsCreated = false;
	mIsAlive = false;
	mTexture.reset();
}

// Pour le rendu
void BaseBody::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	if (mBodyIsCreated)
		target.draw(mSprite, states);
}

/* Accesseurs */
// Type de b2Body
b2BodyType BaseBody::Getb2BodyType() const
{
	myAssert(mBody, "b2Body null");
	return mBody->GetType();
}
void BaseBody::Setb2BodyType(const b2BodyType &type)
{
	// Change le type
	mBody->SetType(type);

	// Rétablie la masse
	if (type == b2_dynamicBody && mBody->GetMass() == 0)
	{
		mBody->ResetMassData();
	}
}
// Type de collision
void BaseBody::SetCollisionType(BaseBody::CollisionType type)
{
	mCollisionType = type;
}
BaseBody::CollisionType BaseBody::GetCollisionType() const
{
	return mCollisionType;
}
// Paramètres de collision
float BaseBody::GetDensity() const
{
	myAssert(mBody, "b2Body null");
	myAssert(mBody->GetFixtureList(), "b2Body sans fixture attaché.");
	return mBody->GetFixtureList()->GetDensity();
}
void BaseBody::SetDensity(float density)
{
	myAssert(mBody, "b2Body null");
	myAssert(mBody->GetFixtureList(), "b2Body sans fixture attaché.");
	mBody->GetFixtureList()->SetDensity(density);
	mBody->ResetMassData();
}
float BaseBody::GetFriction() const
{
	myAssert(mBody, "b2Body null");
	myAssert(mBody->GetFixtureList(), "b2Body sans fixture attaché.");
	return mBody->GetFixtureList()->GetFriction();
}
void BaseBody::SetFriction(float friction)
{
	myAssert(mBody, "b2Body null");
	myAssert(mBody->GetFixtureList(), "b2Body sans fixture attaché.");
	mBody->GetFixtureList()->SetFriction(friction);
}
float BaseBody::GetRestitution() const
{
	myAssert(mBody, "b2Body null");
	myAssert(mBody->GetFixtureList(), "b2Body sans fixture attaché.");
	return mBody->GetFixtureList()->GetRestitution();
}
void BaseBody::SetRestitution(float restitution)
{
	myAssert(mBody, "b2Body null");
	myAssert(mBody->GetFixtureList(), "b2Body sans fixture attaché.");
	mBody->GetFixtureList()->SetRestitution(restitution);
}
// Texture
std::shared_ptr<Texture> BaseBody::GetTexture()
{
	return mTexture;
}
void BaseBody::SetTexture(std::shared_ptr<Texture> texture)
{
	mTexture = texture;
	myAssert(mTexture.get(), "La texture n'est pas chargée.");
	mSprite.setTexture(*mTexture);
	mSprite.setTextureRect(sf::IntRect(sf::Vector2i(0, 0), u2i(mTexture->getSize())));
	mSprite.setOrigin(mSprite.getTextureRect().width / 2.f, mSprite.getTextureRect().height / 2.f);
}
// Sprite
sf::Sprite* BaseBody::GetSprite()
{
	return &mSprite;
}
const sf::Sprite* BaseBody::GetSprite() const
{
	return &mSprite;
}
// Body
b2Body* BaseBody::GetBody()
{
	return mBody;
}
const b2Body* BaseBody::GetBody() const
{
	return mBody;
}
// Position et rotation
const b2Vec2 BaseBody::GetPosition() const
{
	myAssert(mBody, "b2Body null");
	return mBody->GetPosition();
}
const float BaseBody::GetRotationD() const // Degrés
{
	myAssert(mBody, "b2Body null");
	return mBody->GetAngle() * DPR;
}
const float BaseBody::GetRotationR() const // Radians
{
	myAssert(mBody, "b2Body null");
	return mBody->GetAngle();
}
// Fonction à n'employer que pour éditer les niveaux
void BaseBody::SetTransform(const b2Vec2 &position, float angle)
{
	myAssert(mBody, "b2Body null");
	mBody->SetTransform(position, angle);
	mBody->SetAwake(true);
}
