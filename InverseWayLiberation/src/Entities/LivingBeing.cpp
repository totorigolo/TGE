#include "LivingBeing.h"
#include "EntityManager.h"
#include "../Physics/PhysicManager.h"
#include "../Tools/utils.h"
#include "../Tools/Error.h"

// Ctor & dtor
LivingBeing::LivingBeing(b2Vec2 position, const std::shared_ptr<sf::Texture> &texture, int layer)
	: Entity(layer), mBody(nullptr), mTexture(texture), mIsDead(true), mCanJump(false),
	mBodyIsCreated(false),
	mPhysicMgr(PhysicManager::GetInstance())
{
	myAssert(mTexture.get(), "La texture n'est pas chargée.");

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
	mBody = mPhysicMgr.CreateBody(&bodyDef);
	
	// Shape & Fixture du corps
	float x = (mTexture->getSize().x / 2) * mPhysicMgr.GetMPP();
	float y = (mTexture->getSize().y / 2) * mPhysicMgr.GetMPP();
	b2PolygonShape shape1;
	shape1.SetAsBox(x, y);
	shape1.m_radius = 0.f;
	b2FixtureDef fixtureDef1;
	fixtureDef1.shape = &shape1;
	mBody->CreateFixture(&fixtureDef1);

	// Shape & Fixture pour la détection du saut
	b2PolygonShape shape2;
	b2Vec2 center(0.f, - y - 0.03f);
	shape2.SetAsBox(x - 0.02f, 0.03f, center, 0.f);
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
		mPhysicMgr.DestroyBody(mBody);
		mBody = nullptr;
	}
	mBodyIsCreated = false;
	mIsAlive = false;
}

// Mise à jour
void LivingBeing::Update()
{
	// Si on a un body valide
	if (mBody)
	{
		// On récupère sa position
		mSprite.setPosition(b22sfVec(mBody->GetPosition(), mPhysicMgr.GetPPM()));

		// Vérifie le saut
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

// Accesseurs
b2Body* LivingBeing::GetCollisionBody()
{
	return mBody;
}
const b2Body* LivingBeing::GetCollisionBody() const
{
	return mBody;
}
