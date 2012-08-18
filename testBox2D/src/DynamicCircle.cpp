#include "DynamicCircle.h"
#include "config.h"
#include "utils.h"

//Ctor
DynamicCircle::DynamicCircle(b2World *world, b2Vec2 posInMeters, std::shared_ptr<sf::Texture> texture)
	: Body(world, texture)
{
	Create(posInMeters);
}

// Dtor
DynamicCircle::~DynamicCircle(void)
{
}

// Crée le body dans le monde
void DynamicCircle::Create(b2Vec2 posInMeters)
{
	if (mWorld)
	{
		/* Crée le body */
		// BodyDef
		b2BodyDef bodyDef;
		bodyDef.position = posInMeters;
		bodyDef.type = b2_dynamicBody;
		mBody = mWorld->CreateBody(&bodyDef);

		// Shape
		mShape = new b2CircleShape;
		((b2CircleShape*)mShape)->m_radius = mTexture->getSize().x / 2.f * MPP;

		// Fixture
		b2FixtureDef fixtureDef;
		fixtureDef.density = 1.f;
		fixtureDef.friction = 0.7f;
		fixtureDef.restitution = 0.6f;
		fixtureDef.shape = mShape;
		mBody->CreateFixture(&fixtureDef);
	}
}
