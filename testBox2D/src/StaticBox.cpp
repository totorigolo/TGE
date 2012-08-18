#include "StaticBox.h"
#include "config.h"
#include "utils.h"

//Ctor
StaticBox::StaticBox(b2World *world, b2Vec2 posInMeters, std::shared_ptr<sf::Texture> texture)
	: Body(world, texture)
{
	Create(posInMeters);
}

// Dtor
StaticBox::~StaticBox(void)
{
}

// Crée le body dans le monde
void StaticBox::Create(b2Vec2 posInMeters)
{
	if (mWorld)
	{
		/* Crée le body */
		// BodyDef
		b2BodyDef bodyDef;
		bodyDef.position = posInMeters;
		bodyDef.type = b2_staticBody;
		mBody = mWorld->CreateBody(&bodyDef);

		// Shape
		mShape = new b2PolygonShape;
		((b2PolygonShape*)mShape)->SetAsBox((mTexture->getSize().x / 2) * MPP, (mTexture->getSize().y / 2) * MPP);

		// Fixture
		b2FixtureDef fixtureDef;
		fixtureDef.density = 0.f;
		fixtureDef.shape = mShape;
		mBody->CreateFixture(&fixtureDef);
	}
}
