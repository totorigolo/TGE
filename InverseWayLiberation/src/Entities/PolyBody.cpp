#include "stdafx.h"
#include "PolyBody.h"
#include "../Physics/PhysicManager.h"

// Ctor & dtor
PolyBody::PolyBody(int layer, unsigned int ID)
	: BaseBody(layer, ID)
{
	// Défini le type de l'Entity
	mType = EntityType::PolyBody;
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
