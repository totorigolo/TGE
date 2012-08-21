#include "DynamicCircle.h"
#include "config.h"
#include "../utils.h"

//Ctor
DynamicCircle::DynamicCircle(World *world, b2Vec2 pos, std::shared_ptr<sf::Texture> texture)
	: Body(world), mTexture(texture)
{
	// Change la texture
	this->setTexture(*mTexture);

	if (mWorld)
	{
		/* Crée le body */
		// BodyDef
		b2BodyDef bodyDef;
		bodyDef.position = pos;
		bodyDef.type = b2_dynamicBody;
		mBody = mWorld->CreateBody(&bodyDef, this);

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

		mBody->SetUserData(this);
	}
}

// Dtor
DynamicCircle::~DynamicCircle(void)
{
}

// Met à jour la position du sprite
void DynamicCircle::Update()
{
	if (mBody && mWorld)
	{
		this->setPosition(b22sfVec(mBody->GetPosition()));
		this->setOrigin(u2f(mTexture->getSize()) / 2.f);
		this->setRotation(- mBody->GetAngle() * DPR);
	}
}
