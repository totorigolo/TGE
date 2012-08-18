#include "DynamicBox.h"
#include "config.h"
#include "utils.h"

//Ctor
DynamicBox::DynamicBox(b2World *world, b2Vec2 posInMeters, std::shared_ptr<sf::Texture> texture)
	: Body(world, texture)
{
	Create(posInMeters);
}

// Dtor
DynamicBox::~DynamicBox(void)
{
}

// Crée le body dans le monde
void DynamicBox::Create(b2Vec2 posInMeters)
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
		mShape = new b2PolygonShape;
		((b2PolygonShape*)mShape)->SetAsBox((mTexture->getSize().x / 2) * MPP, (mTexture->getSize().y / 2) * MPP);

		// Fixture
		b2FixtureDef fixtureDef;
		fixtureDef.density = 1.f;
		fixtureDef.friction = 0.7f;
		fixtureDef.shape = mShape;
		mBody->CreateFixture(&fixtureDef);
	}
}
