#include "StaticBox.h"
#include "../utils.h"

//Ctor
StaticBox::StaticBox(World *world, b2Vec3 posRot, std::shared_ptr<sf::Texture> texture, float friction, float restitution)
	: Body(world), mTexture(texture)
{
	// Change la texture
	this->setTexture(*mTexture);

	if (mWorld)
	{
		/* Crée le body */
		// BodyDef
		b2BodyDef bodyDef;
		bodyDef.angle = posRot.z * RPD;
		bodyDef.position = getVec2(posRot);
		bodyDef.type = b2_staticBody;
		mBody = mWorld->CreateBody(&bodyDef, this);

		// Shape
		mShape = new b2PolygonShape;
		((b2PolygonShape*)mShape)->SetAsBox((mTexture->getSize().x / 2) * mWorld->GetMPP(), (mTexture->getSize().y / 2) * mWorld->GetMPP());

		// Fixture
		b2FixtureDef fixtureDef;
		fixtureDef.density = 0.f;
		fixtureDef.friction = friction;
		fixtureDef.restitution = restitution;
		fixtureDef.shape = mShape;
		mBody->CreateFixture(&fixtureDef);
		
		mBody->SetUserData(this);
		mIsNull = false;
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
		this->setPosition(b22sfVec(mBody->GetPosition(), mWorld->GetPPM()));
		this->setOrigin(u2f(mTexture->getSize()) / 2.f);
		this->setRotation(- mBody->GetAngle() * DPR);
	}
}
