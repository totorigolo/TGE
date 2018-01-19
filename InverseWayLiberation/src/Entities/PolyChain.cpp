#include <Box2D/Box2D.h>

#include "PolyChain.h"
#include "../Physics/PhysicManager.h"
#include "../Tools/utils.h"

// Ctor & dtor
PolyChain::PolyChain(int layer)
	: BaseBody(layer), mChainType(Type::None)
{
	// Défini le type de l'Entity
	mType = EntityType::PolyChain;
}

// Mise à jour
void PolyChain::PreUpdate()
{
	BaseBody::PreUpdate();
}

// Création du body
bool PolyChain::Create(const std::vector<b2Vec2> &vertices, Type type, Texture::Ptr texture,
					  float density, float friction, float restitution, int groupIndex, uint16 categoryBits, uint16 maskBits)
{
	// On n'en crée pas de nouveau si il y en a déjà un
	if (mBodyIsCreated)
		return false;

	// Vérifie les points
	if (!CheckPoints(vertices))
	{
		Dialog::Error(L"Le PolyBody n'a pas été créé :\nPoints invalides.");
		return false;
	}

	// Récupère la position du centre
	b2Vec2 center(b2Vec2_zero);
	std::vector<b2Vec2> v;
	for (unsigned int i = 0; i < vertices.size(); ++i) // Calcule la position du centre
		center += vertices[i];
	center.x /= vertices.size();
	center.y /= vertices.size();
	for (unsigned int i = 0; i < vertices.size(); ++i) // Ajoute les points dans le vecteur, avec translation
		v.push_back(vertices[i] - center);

	// Crée le PolyChain
	b2Vec3 posRot(center.x, center.y, 0.f);
	Create(posRot, v, type, texture, density, friction, restitution, groupIndex, categoryBits, maskBits);

	return true;
}
bool PolyChain::Create(b2Vec3 posRot, const std::vector<b2Vec2> &vertices, Type type, Texture::Ptr texture,
					  float density, float friction, float restitution, int groupIndex, uint16 categoryBits, uint16 maskBits)
{
	// On n'en crée pas de nouveau si il y en a déjà un
	if (mBodyIsCreated)
		return false;

	// Vérifie les points
	if (!CheckPoints(vertices))
	{
		Dialog::Error(L"Le PolyBody n'a pas été créé :\nPoints invalides.");
		return false;
	}

	// Vérifie le type
	if (type == Type::None)
	{
		Dialog::Error(L"Le PolyChain n'a pas été créé :\nType invalide (None).");
		return false;
	}
	mChainType = type;

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
	b2ChainShape shape;
	if (mChainType == Type::Chain)
		shape.CreateChain(v, mPoints.size());
	else //if (mChainType == Type::Loop)
		shape.CreateLoop(v, mPoints.size());

	// BodyDef
	b2BodyDef bodyDef;
	bodyDef.position = getVec2(posRot);
	bodyDef.angle = posRot.z * RPD;
	bodyDef.type = b2BodyType::b2_staticBody;
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
	mIsAlive = true;
	mHasMoved = true;

	// Active les ombres
	mHull.SetBodyShadowCaster(mBody);
	mHull.SetPhysicallyDrawable(true);
	mHull.Activate();

	return true;
}

// Vérifie les points passés
bool PolyChain::CheckPoints(const std::vector<b2Vec2> &vertices)
{
	// Si il n'y a pas le bon nombre de points, on arrête
	if (vertices.size() < 3)
		return false;

	// Vérifie qu'il n'y a pas d'arête de longueur nulle
	b2Vec2 last = vertices.front();
	for (unsigned int i = 1; i < vertices.size(); ++i)
	{
		if (vertices[i] == last)
			return false;
		last = vertices[i];
	}
	if (vertices.front() == vertices.back())
		return false;

	return true;
}

// Points du Body
const std::vector<b2Vec2>& PolyChain::GetPoints() const
{
	return mPoints;
}
// Type
PolyChain::Type PolyChain::GetChainType() const
{
	return mChainType;
}
