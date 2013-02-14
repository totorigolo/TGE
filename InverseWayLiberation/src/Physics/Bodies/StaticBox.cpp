#include "StaticBox.h"
#include "../../Tools/utils.h"
#include "../../Lights/ConvexHull.h"

//Ctor
StaticBox::StaticBox(World *world, b2Vec3 posRot, std::shared_ptr<sf::Texture> texture, float friction, float restitution
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
		bodyDef.type = b2_staticBody;
		mBody = mWorld->CreateBody(&bodyDef, this);

		// Shape
		b2PolygonShape* shape = new b2PolygonShape;
		shape->SetAsBox((mSprite->getTexture()->getSize().x / 2) * mWorld->GetMPP(),
											(mSprite->getTexture()->getSize().y / 2) * mWorld->GetMPP());
		shape->m_radius = 0.f;

		// Fixture
		b2FixtureDef fixtureDef;
		fixtureDef.density = 0.f;
		fixtureDef.friction = friction;
		fixtureDef.restitution = restitution;
		fixtureDef.filter.groupIndex = static_cast<int16>(groupIndex);
		fixtureDef.filter.categoryBits = categoryBits;
		fixtureDef.filter.maskBits = maskBits;
		fixtureDef.shape = shape;
		mBody->CreateFixture(&fixtureDef);
		
		mBody->SetUserData(this);
		mIsNull = false;

		// Enregistre le Body
		mWorld->RegisterBody(this);

		// Crée le Hull
		mHull = new ConvexHull(this, true);
	}
}

// Dtor
StaticBox::~StaticBox(void)
{
}

// Met à jour la position du sprite
void StaticBox::Update()
{
	if (mBody && mWorld && mSprite)
	{
		mSprite->setPosition(b22sfVec(mBody->GetPosition(), mWorld->GetPPM()));
		mSprite->setOrigin(mSprite->getTextureRect().width / 2.f, mSprite->getTextureRect().height / 2.f);
		mSprite->setRotation(- mBody->GetAngle() * DPR);
	}
}

// Accesseur
b2AABB StaticBox::GetBodyAABB() const
{
	b2AABB aabb;
	// La StaticBox n'a qu'un shape
	b2PolygonShape* s = (b2PolygonShape*) mBody->GetFixtureList()->GetShape();
	if (s)
	{
		s->ComputeAABB(&aabb, mBody->GetTransform(), 0);
	}
	return aabb;
}
