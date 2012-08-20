#include "StaticBox.h"
#include "config.h"
#include "utils.h"

//Ctor
StaticBox::StaticBox(World *world, b2Vec2 posInMeters, std::shared_ptr<sf::Texture> texture)
	: Body(world), mTexture(texture)
{
	// Change la texture
	this->setTexture(*mTexture);

	if (mWorld)
	{
		/* Crée le body */
		// BodyDef
		b2BodyDef bodyDef;
		bodyDef.position = posInMeters;
		bodyDef.type = b2_staticBody;
		mBody = mWorld->CreateBody(&bodyDef, this);

		// Shape
		mShape = new b2PolygonShape;
		((b2PolygonShape*)mShape)->SetAsBox((mTexture->getSize().x / 2) * MPP, (mTexture->getSize().y / 2) * MPP);

		// Fixture
		b2FixtureDef fixtureDef;
		fixtureDef.density = 0.f;
		fixtureDef.shape = mShape;
		mBody->CreateFixture(&fixtureDef);
		
		mBody->SetUserData(this);
	}
}

// Dtor
StaticBox::~StaticBox(void)
{
}

// Met à jour la position du sprite
void StaticBox::Update()
{
	if (mBody && mWorld)
	{
		this->setPosition(b22sfVec(mBody->GetPosition()));
		this->setOrigin(u2f(mTexture->getSize()) / 2.f);
		this->setRotation(- mBody->GetAngle() * DPR);
	}
}
