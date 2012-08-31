#include "DynamicCircle.h"
#include "../../utils.h"

//Ctor
DynamicCircle::DynamicCircle(World *world, b2Vec3 posRot, std::shared_ptr<sf::Texture> texture, float density, float friction, float restitution
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
		bodyDef.type = b2_dynamicBody;
		mBody = mWorld->CreateBody(&bodyDef, this);

		// Shape
		mShape = new b2CircleShape;
		((b2CircleShape*)mShape)->m_radius = mTexture->getSize().x / 2.f * mWorld->GetMPP();

		// Fixture
		b2FixtureDef fixtureDef;
		fixtureDef.density = density;
		fixtureDef.friction = friction;
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
DynamicCircle::~DynamicCircle(void)
{
}

// Met à jour la position du sprite
void DynamicCircle::Update()
{
	if (mBody && mWorld)
	{
		this->setPosition(b22sfVec(mBody->GetPosition(), mWorld->GetPPM()));
		this->setOrigin(u2f(mTexture->getSize()) / 2.f);
		this->setRotation(- mBody->GetAngle() * DPR);
	}
}

// Accesseurs
b2AABB DynamicCircle::GetBodyAABB() const
{
	b2AABB aabb;
	((b2PolygonShape*) mShape)->ComputeAABB(&aabb, mBody->GetTransform(), 0);
	return aabb;
}
