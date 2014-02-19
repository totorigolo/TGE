#include "PolyBody.h"
#include "../Tools/utils.h"
#include "../Tools/Error.h"
#include "../Physics/PhysicManager.h"

#include <iostream>

// Ctor & dtor
PolyBody::PolyBody(int layer, unsigned int ID)
	: Entity(layer, ID), mCollisionType(CollisionType::Default),
	mBody(nullptr), mBodyIsCreated(false),
	mPhysicMgr(PhysicManager::GetInstance())
{
	// Défini le type de l'Entity
	mType = EntityType::PolyBody;
}
PolyBody::~PolyBody()
{
	Destroy();
}

// Création du body
bool PolyBody::Create(std::vector<b2Vec2> vertices, b2BodyType type, const std::shared_ptr<Texture> &texture,
					  float density, float friction, float restitution, int groupIndex, uint16 categoryBits, uint16 maskBits)
{
	// On n'en crée pas de nouveau si il y en a déjà un
	if (mBodyIsCreated)
		return false;

	// Change la texture
	mTexture = texture;
	myAssert(mTexture.get(), "La texture n'est pas chargée.");
	mSprite.setTexture(*mTexture);
	mSprite.setOrigin(mSprite.getTextureRect().width / 2.f, mSprite.getTextureRect().height / 2.f);

	// Shape
	b2Vec2 center(b2Vec2_zero);
	b2Vec2 *v = new b2Vec2[vertices.size()];
	for (int i = 0; i < vertices.size(); ++i) // Calcule la position du centre
		center += vertices[i];
	center.x /= vertices.size();
	center.y /= vertices.size();
	for (int i = 0; i < vertices.size(); ++i) // Ajoute les points dans le vecteur, avec translation
		v[i] = vertices[i] - center;
	b2PolygonShape shape;
	shape.Set(v, vertices.size());

	// BodyDef
	b2BodyDef bodyDef;
	bodyDef.position = center;
	bodyDef.angle = 0;
	bodyDef.type = type;
	mBody = mPhysicMgr.CreateBody(&bodyDef);

	// Fixture
	b2FixtureDef fixtureDef;
	fixtureDef.density = density;
	fixtureDef.friction = friction;
	fixtureDef.restitution = restitution;
	fixtureDef.filter.groupIndex = static_cast<int16>(groupIndex);
	fixtureDef.filter.categoryBits = categoryBits;
	fixtureDef.filter.maskBits = maskBits;
	fixtureDef.shape = &shape;
	mBody->CreateFixture(&fixtureDef);

	// Enregistrements
	mBody->SetUserData(this);
	mBodyIsCreated = true;

	return true;
}

// Mise à jour
void PolyBody::Update()
{
	// Si le body est valide
	if (mBody)
	{
		// Mise à jour de la texture
		mSprite.setPosition(b22sfVec(mBody->GetPosition(), mPhysicMgr.GetPPM()));
		mSprite.setRotation(- mBody->GetAngle() * DPR);
	}
}

// Destruction du body
void PolyBody::Destroy()
{
	if (mBody)
	{
		// Supprime le Body
		mPhysicMgr.DestroyBody(mBody);
		mBody = nullptr;
	}
	mBodyIsCreated = false;
	mIsAlive = false;
	mTexture.reset();
}

// Pour le rendu
void PolyBody::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	if (mBodyIsCreated)
		target.draw(mSprite, states);
}

/* Accesseurs */
// Type de b2Body
b2BodyType PolyBody::Getb2BodyType() const
{
	myAssert(mBody, "b2Body null");
	return mBody->GetType();
}
void PolyBody::Setb2BodyType(const b2BodyType &type)
{
	// Change le type
	mBody->SetType(type);

	// Rétablie la masse
	if (type == b2_dynamicBody && mBody->GetMass() == 0)
	{
		mBody->ResetMassData();
	}
}
// Type de collision
void PolyBody::SetCollisionType(PolyBody::CollisionType type)
{
	mCollisionType = type;
}
PolyBody::CollisionType PolyBody::GetCollisionType() const
{
	return mCollisionType;
}
// Paramètres de collision
float PolyBody::GetDensity() const
{
	myAssert(mBody, "b2Body null");
	myAssert(mBody->GetFixtureList(), "b2Body sans fixture attaché.");
	return mBody->GetFixtureList()->GetDensity();
}
void PolyBody::SetDensity(float density)
{
	myAssert(mBody, "b2Body null");
	myAssert(mBody->GetFixtureList(), "b2Body sans fixture attaché.");
	mBody->GetFixtureList()->SetDensity(density);
	mBody->ResetMassData();
}
float PolyBody::GetFriction() const
{
	myAssert(mBody, "b2Body null");
	myAssert(mBody->GetFixtureList(), "b2Body sans fixture attaché.");
	return mBody->GetFixtureList()->GetFriction();
}
void PolyBody::SetFriction(float friction)
{
	myAssert(mBody, "b2Body null");
	myAssert(mBody->GetFixtureList(), "b2Body sans fixture attaché.");
	mBody->GetFixtureList()->SetFriction(friction);
}
float PolyBody::GetRestitution() const
{
	myAssert(mBody, "b2Body null");
	myAssert(mBody->GetFixtureList(), "b2Body sans fixture attaché.");
	return mBody->GetFixtureList()->GetRestitution();
}
void PolyBody::SetRestitution(float restitution)
{
	myAssert(mBody, "b2Body null");
	myAssert(mBody->GetFixtureList(), "b2Body sans fixture attaché.");
	mBody->GetFixtureList()->SetRestitution(restitution);
}
// Sprite
sf::Sprite* PolyBody::GetSprite()
{
	return &mSprite;
}
const sf::Sprite* PolyBody::GetSprite() const
{
	return &mSprite;
}
// Body
b2Body* PolyBody::GetBody()
{
	return mBody;
}
const b2Body* PolyBody::GetBody() const
{
	return mBody;
}
// Position et rotation
const b2Vec2 PolyBody::GetPosition() const
{
	myAssert(mBody, "b2Body null");
	return mBody->GetPosition();
}
const float PolyBody::GetRotationD() const // Degrés
{
	myAssert(mBody, "b2Body null");
	return mBody->GetAngle() * DPR;
}
const float PolyBody::GetRotationR() const // Radians
{
	myAssert(mBody, "b2Body null");
	return mBody->GetAngle();
}
// Fonction à n'employer que pour éditer les niveaux
void PolyBody::SetTransform(const b2Vec2 &position, float angle)
{
	myAssert(mBody, "b2Body null");
	mBody->SetTransform(position, angle);
	mBody->SetAwake(true);
}
