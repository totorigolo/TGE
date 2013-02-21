#include "PlayerBody.h"
#include "../../Tools/utils.h"
#include "../../Lights/ConvexHull.h"

// Ctor
PlayerBody::PlayerBody(World *world, b2Vec3 posRot, std::shared_ptr<sf::Texture> texture)
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

		// Shape corprs
		b2PolygonShape* shape = new b2PolygonShape; // TODO: pointeurs inutils et non supprimés
		shape->SetAsBox((texture->getSize().x / 2) * mWorld->GetMPP(), (texture->getSize().y / 2) * mWorld->GetMPP());
		shape->m_radius = 0.f;
		b2FixtureDef fixtureDef;
		fixtureDef.shape = shape;
		mBody->CreateFixture(&fixtureDef);

		// Sous les pieds
		b2PolygonShape* shape2 = new b2PolygonShape;
		shape2->SetAsBox((texture->getSize().x / 2) * mWorld->GetMPP(), (texture->getSize().y / 2) * mWorld->GetMPP() * 0.1f
			, b2Vec2(0, - static_cast<float>(texture->getSize().y / 2)), 0.f);
		shape2->m_radius = 0.f;
		b2FixtureDef fixtureDef2;
		fixtureDef2.shape = shape2;
		fixtureDef2.isSensor = true;
		mBody->CreateFixture(&fixtureDef2);
		b2Fixture *f = mBody->GetFixtureList();
		f = f->GetNext();

		mBody->SetUserData(this);
		mIsNull = false;
		
		// Enregistre le Body
		mWorld->RegisterBody(this);

		// Crée le Hull
		mHull = new ConvexHull(this, false);
	}
}

// Dtor
PlayerBody::~PlayerBody(void)
{
}

// Met à jour la position du sprite
void PlayerBody::Update()
{
	if (mBody && mWorld && mSprite)
	{
		mSprite->setPosition(b22sfVec(mBody->GetPosition(), mWorld->GetPPM()));
		mSprite->setOrigin(mSprite->getTextureRect().width / 2.f, mSprite->getTextureRect().height / 2.f);
		mSprite->setRotation(- mBody->GetAngle() * DPR);
	}
}

// Change les collisions / la taille du body
void PlayerBody::SetSize(float w, float h)
{
	// TODO: Vérifier si ça marche

	// La DynamicBox n'a qu'un shape
	b2PolygonShape* s = (b2PolygonShape*) mBody->GetFixtureList()->GetShape();
	if (s)
	{
		s->SetAsBox(w / 2.f, h / 2.f);
	}
}

// Accesseurs
b2AABB PlayerBody::GetBodyAABB() const
{
	b2AABB aabb;
	// La DynamicBox n'a qu'un shape
	b2PolygonShape* s = (b2PolygonShape*) mBody->GetFixtureList()->GetShape();
	if (s)
	{
		s->ComputeAABB(&aabb, mBody->GetTransform(), 0);
	}
	return aabb;
}
