#include "DynamicCircle.h"
#include "../../utils.h"

//Ctor
DynamicCircle::DynamicCircle(World *world, b2Vec3 posRot, std::shared_ptr<sf::Texture> texture, float density, float friction, float restitution
																							  , int groupIndex, uint16 categoryBits, uint16 maskBits)
	: Body(world)
{
	// Change la texture
	mSprite->setTexture(*texture);

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
		((b2CircleShape*)mShape)->m_radius = mSprite->getTexture()->getSize().x / 2.f * mWorld->GetMPP();

		// Fixture
		b2FixtureDef fixtureDef;
		fixtureDef.density = density;
		fixtureDef.friction = friction;
		fixtureDef.restitution = restitution;
		fixtureDef.filter.groupIndex = static_cast<int16>(groupIndex);
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
	if (mBody && mWorld && mSprite)
	{
		mSprite->setPosition(b22sfVec(mBody->GetPosition(), mWorld->GetPPM()));
		mSprite->setOrigin(mSprite->getTextureRect().width / 2.f, mSprite->getTextureRect().height / 2.f);
		mSprite->setRotation(- mBody->GetAngle() * DPR);
	}
}

// Accesseurs
b2AABB DynamicCircle::GetBodyAABB() const
{
	b2AABB aabb;
	((b2PolygonShape*) mShape)->ComputeAABB(&aabb, mBody->GetTransform(), 0);
	return aabb;
}
