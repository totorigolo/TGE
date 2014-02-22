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

	// Récupère la position du centre
	b2Vec2 center(b2Vec2_zero);
	std::vector<b2Vec2> v;
	for (unsigned int i = 0; i < vertices.size(); ++i) // Calcule la position du centre
		center += vertices[i];
	center.x /= vertices.size();
	center.y /= vertices.size();
	for (unsigned int i = 0; i < vertices.size(); ++i) // Ajoute les points dans le vecteur, avec translation
		v.push_back(vertices[i] - center);

	// Crée le PolyBody
	b2Vec3 posRot(center.x, center.y, 0.f);
	Create(posRot, v, type, texture, density, friction, restitution, groupIndex, categoryBits, maskBits);

	return true;
}
bool PolyBody::Create(b2Vec3 posRot, std::vector<b2Vec2> vertices, b2BodyType type, const std::shared_ptr<Texture> &texture,
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

	// Copie les points
	mPoints = vertices;

	// Shape
	b2Vec2 *v = new b2Vec2[mPoints.size()];
	for (unsigned int i = 0; i < mPoints.size(); ++i)
		v[i] = mPoints[i];
	b2PolygonShape shape;
	shape.Set(v, mPoints.size());

	// BodyDef
	b2BodyDef bodyDef;
	bodyDef.position = getVec2(posRot);
	bodyDef.angle = posRot.z * RPD;
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

// Points du Body
const std::vector<b2Vec2>& PolyBody::GetPoints() const
{
	return mPoints;
}
