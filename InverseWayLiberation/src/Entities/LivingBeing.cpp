#include "LivingBeing.h"
#include "EntityManager.h"
#include "../Tools/utils.h"
#include <iostream>

// Ctor & dtor
LivingBeing::LivingBeing(PhysicManager *physicMgr, b2Vec2 position, std::shared_ptr<sf::Texture> texture, int layer)
	: Entity(layer), mBody(nullptr), mTexture(texture), mPhysicMgr(physicMgr), mIsDead(true), mCanJump(false),
	mBodyIsCreated(false)
{
	assert(mPhysicMgr && "n'est pas valide.");

	// Change le type
	mType = EntityType::LivingBeing;

	// Initialise le sprite
	mSprite.setTexture(*mTexture);
	mSprite.setOrigin(u2f(mTexture->getSize()) / 2.f);
	
	// BodyDef
	b2BodyDef bodyDef;
	bodyDef.angle = 0.f;
	bodyDef.position = position;
	bodyDef.type = b2_dynamicBody;
	mBody = mPhysicMgr->CreateBody(&bodyDef);
	
	// Shape & Fixture du corps
	float x = (texture->getSize().x / 2) * mPhysicMgr->GetMPP();
	float y = (texture->getSize().y / 2) * mPhysicMgr->GetMPP();
	b2PolygonShape shape1;
	shape1.SetAsBox(x, y);
	shape1.m_radius = 0.f;
	b2FixtureDef fixtureDef1;
	fixtureDef1.shape = &shape1;
	mBody->CreateFixture(&fixtureDef1);

	// Shape & Fixture pour la d�tection du saut
	b2PolygonShape shape2;
	b2Vec2 center(0.f, - y - 0.02f);
	shape2.SetAsBox(x - 0.015f, 0.02f, center, 0.f);
	b2FixtureDef fixtureDef2;
	fixtureDef2.shape = &shape2;
	fixtureDef2.isSensor = true;
	mBody->CreateFixture(&fixtureDef2);

	// Enregistrements
	mBody->SetUserData(this);
	mBodyIsCreated = true;
	
	// Tout est Ok
	mIsAlive = true;
	mIsDead = false;
}
LivingBeing::~LivingBeing()
{
	// Supprime le Body
	if (mBody)
	{
		mPhysicMgr->DestroyBody(mBody);
		mBody = nullptr;
	}
	mBodyIsCreated = false;
	mIsAlive = false;
}

// Mise � jour
void LivingBeing::Update()
{
	// Si on a un body valide
	if (mBody)
	{
		// On r�cup�re sa position
		mSprite.setPosition(b22sfVec(mBody->GetPosition(), mPhysicMgr->GetPPM()));

		// V�rifie le saut
		mCanJump = false;
		for (b2ContactEdge* ce = mBody->GetContactList(); ce; ce = ce->next)
		{
			b2Contact* c = ce->contact;
			
			if (c->GetFixtureA()->IsSensor() || c->GetFixtureB()->IsSensor())
			{
				if (c->IsTouching())
				{
					mCanJump = true;
					break;
				}
			}
		}
	}
}

// Pour le rendu
void LivingBeing::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	if (mIsAlive)
	{
		target.draw(mSprite, states);
	}
}

// D�finit si l'entit� peut sauter
void LivingBeing::CanJump(bool b)
{
	mCanJump = b;
}

// Gestion des d�pendences
void LivingBeing::DependencyDestroyed(void *dependency)
{
	std::cout << "LivingBeing::DependencyDestroyed" << std::endl;

	/*/ V�rifie que le Body ne soit pas supprim�
	if (dependency == mCollisionBody)
	{
		mIsAlive = false;
		mCollisionBody = nullptr;
		EntityManager::GetInstance().DestroyEntity(this, false);
		delete this;
	}*/
	//mTexture; // TODO: Gestion des textures plus pouss�e
}

// Accesseurs
b2Body* LivingBeing::GetCollisionBody()
{
	return mBody;
}
const b2Body* LivingBeing::GetCollisionBody() const
{
	return mBody;
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
