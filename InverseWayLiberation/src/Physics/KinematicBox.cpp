#include "KinematicBox.h"
#include "../utils.h"

//Ctor
KinematicBox::KinematicBox(World *world, b2Vec3 posRot, std::shared_ptr<sf::Texture> texture, float restitution
																							, int groupIndex, uint16 categoryBits, uint16 maskBits)
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
		bodyDef.type = b2_kinematicBody;
		mBody = mWorld->CreateBody(&bodyDef, this);

		// Shape
		mShape = new b2PolygonShape;
		((b2PolygonShape*)mShape)->SetAsBox((mTexture->getSize().x / 2) * mWorld->GetMPP(), (mTexture->getSize().y / 2) * mWorld->GetMPP());
		((b2PolygonShape*)mShape)->m_radius = 0.f;

		// Fixture
		b2FixtureDef fixtureDef;
		fixtureDef.restitution = restitution;
		fixtureDef.filter.groupIndex = groupIndex;
		fixtureDef.filter.categoryBits = categoryBits;
		fixtureDef.filter.maskBits = maskBits;
		fixtureDef.shape = mShape;
		mBody->CreateFixture(&fixtureDef);

		mBody->SetUserData(this);
		mIsNull = false;
	}
}

// Dtor
KinematicBox::~KinematicBox(void)
{
}

// Met à jour la position du sprite
void KinematicBox::Update()
{
	if (mBody && mWorld)
	{
		this->setPosition(b22sfVec(mBody->GetPosition(), mWorld->GetPPM()));
		this->setOrigin(u2f(mTexture->getSize()) / 2.f);
		this->setRotation(- mBody->GetAngle() * DPR);
	}
}

void KinematicBox::SetVelocity(b2Vec2 const& velocity, float angularVelocity)
{
	if (mBody && mWorld)
	{
		mBody->SetLinearVelocity(velocity);
		mBody->SetAngularVelocity(angularVelocity);

		this->Update();
	}
}
void KinematicBox::SetPosition(b2Vec2 const& pos, float angle)
{
	if (mBody && mWorld)
	{
		mBody->SetTransform(pos, angle);

		this->Update();
	}
}
