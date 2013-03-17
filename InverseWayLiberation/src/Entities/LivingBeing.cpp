#include "LivingBeing.h"
#include "EntityManager.h"
#include "../Tools/utils.h"
#include <iostream>

// Ctor & dtor
LivingBeing::LivingBeing(PhysicManager *physicMgr, b2Vec2 position, std::shared_ptr<sf::Texture> texture, int layer)
	: Entity(layer), mCollisionBody(nullptr), mTexture(texture), mPhysicMgr(physicMgr), mIsDead(true), mCanJump(false)
{
	// Change le type
	mType = EntityType::LivingBeing;

	// Initialise le sprite
	mSprite.setTexture(*mTexture);
	mSprite.setOrigin(u2f(mTexture->getSize()) / 2.f);

	/*////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Change la texture
	mSprite->setTexture(*texture);

	//* Crée le body * //
	// BodyDef
	b2BodyDef bodyDef;
	bodyDef.angle = posRot.z * RPD;
	bodyDef.position = getVec2(posRot);
	bodyDef.type = b2_dynamicBody;
	mBody = mWorld->CreateBody(&bodyDef, this);

	// Shape
	mShape = new b2PolygonShape;
	((b2PolygonShape*)mShape)->SetAsBox((texture->getSize().x / 2) * mWorld->GetMPP(), (texture->getSize().y / 2) * mWorld->GetMPP());
	((b2PolygonShape*)mShape)->m_radius = 0.f;

	// Fixture
	b2FixtureDef fixtureDef;
	fixtureDef.density = density;
	fixtureDef.friction = friction;
	fixtureDef.restitution = restitution;
	fixtureDef.filter.groupIndex = static_cast<int16>(groupIndex);
	fixtureDef.filter.categoryBits = categoryBits;
	fixtureDef.filter.maskBits = maskBits;
	fixtureDef.shape = mShape;
	mBody->CreateFixture(&fixtureDef);

	mBody->SetUserData(this);
	mIsNull = false;
		
	// Enregistre le Body
	mWorld->RegisterBody(this);

	// Crée le Hull
	mHull = new ConvexHull(this, false);
	*/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	// Crée le body
	//mCollisionBody = new DynamicBox(mWorld, getVec3(position), mTexture);
	//mCollisionBody->GetBody()->SetFixedRotation(true);
	//mCollisionBody->SetType(BodyType::Bullet);
	//mCollisionBody->SetEntity(this);

	// Crée le body pour le saut

	// Tout est Ok
	mIsAlive = true;
	mIsDead = false;
}
LivingBeing::~LivingBeing()
{
	mIsAlive = false;

	// Supprime le Body
	if (mCollisionBody)
	{
		mPhysicMgr->DestroyBody(mCollisionBody);
		mCollisionBody = nullptr;
	}
}

// Mise à jour
void LivingBeing::Update()
{
	// Si on a un body valide
	if (mCollisionBody)
	{
		// On récupère sa position
		//mSprite.setTexture(*mTexture);
		mSprite.setPosition(b22sfVec(mCollisionBody->GetPosition(), mPhysicMgr->GetPPM()));
	}

	//if (mAnimatedSprite.IsValid())
	//	mAnimatedSprite.Update();
}

// Pour le rendu
void LivingBeing::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	if (mIsAlive)// && mAnimatedSprite.IsValid())
	{
		//target.draw(mAnimatedSprite, states);
		target.draw(mSprite, states);
	}
}

// ContactListener uniquement : indique si l'entité peut sauter
void LivingBeing::CanJump(bool b)
{
	mCanJump = b;
}

// Gestion des dépendences
void LivingBeing::DependencyDestroyed(void *dependency)
{
	std::cout << "LivingBeing::DependencyDestroyed" << std::endl;

	/*/ Vérifie que le Body ne soit pas supprimé
	if (dependency == mCollisionBody)
	{
		mIsAlive = false;
		mCollisionBody = nullptr;
		EntityManager::GetInstance().DestroyEntity(this, false);
		delete this;
	}*/
	//mTexture; // TODO: Gestion des textures plus poussée
}

// Accesseurs
b2Body* LivingBeing::GetCollisionBody()
{
	return mCollisionBody;
}
const b2Body* LivingBeing::GetCollisionBody() const
{
	return mCollisionBody;
}
/*
AnimatedSprite& LivingBeing::GetAnimatedSprite()
{
	return mAnimatedSprite;
}
const AnimatedSprite& LivingBeing::GetAnimatedSprite() const
{
	return mAnimatedSprite;
}
//*/
