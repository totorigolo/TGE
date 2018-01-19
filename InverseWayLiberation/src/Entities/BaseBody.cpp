#include "BaseBody.h"
#include "../Physics/PhysicManager.h"
#include "../Tools/utils.h"

// Ctor & dtor
BaseBody::BaseBody(int layer)
	: Entity(layer), mCollisionType(CollisionType::Default),
	mBody(nullptr), mBodyIsCreated(false),
	mPhysicMgr(PhysicManager::GetInstance()),
	mHull(nullptr), mHasMoved(true)
{
	// Défini le type de l'Entity
	mType = EntityType::BaseBody;
}
BaseBody::~BaseBody()
{
	Destroy();
}

// Mise à jour
void BaseBody::PreUpdate()
{
	// Si le body est valide
	if (!mBody || !mBodyIsCreated || !mIsAlive) return;

	// Mise à jour de la texture
	mSprite.setPosition(b22sfVec(mBody->GetPosition(), mPhysicMgr.GetPPM()));
	mSprite.setRotation(-mBody->GetAngle() * DPR);

	// Mise à jour du Hull
	if (mHasMoved || (mBody->IsAwake() && mBody->GetType() != b2BodyType::b2_staticBody))
	{
		b2AABB aabb = GetBoundingBox();
		b2Vec2 pos;
		pos.x = aabb.lowerBound.x;
		pos.y = aabb.upperBound.y;
		b2Vec2 size = aabb.upperBound - aabb.lowerBound;
		sf::Vector2f sfpos = b22sfVec(pos, PhysicManager::GetInstance().GetPPM());
		sf::Vector2f sfsize = b22sfVec(size, PhysicManager::GetInstance().GetPPM(), true);
		mHull.SetPosAndSize(sfpos, sfsize);
	}
}
void BaseBody::Update()
{
	// Si le body est valide
	if (mBody && mBodyIsCreated && mIsAlive)
	{
		mHasMoved = false;
	}
}

// Destruction du body
void BaseBody::Destroy()
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

// Ombres
void BaseBody::ActivateShadows(void)
{
	mHull.Activate();

	mHasMoved = true;
}
void BaseBody::DeactivateShadows(void)
{
	mHull.Deactivate();

	mHasMoved = true;
}
void BaseBody::SetShadowsActive(bool active)
{
	if (active)
		ActivateShadows();
	else
		DeactivateShadows();
}
bool BaseBody::IsActiveShadows(void) const
{
	return mHull.IsActive();
}

// Boite englobante
b2AABB BaseBody::GetBoundingBox(void) const
{
	// Crée le aabb final
	b2AABB aabb;
	aabb.lowerBound = b2Vec2(FLT_MAX, FLT_MAX);
	aabb.upperBound = b2Vec2(-FLT_MAX, -FLT_MAX);

	// Parcours toutes les Fixtures
	for (b2Fixture* fixture = mBody->GetFixtureList(); fixture; fixture = fixture->GetNext())
	{
		// Pour chaque élément de la forme
		// Un pour les formes simples, un par segment pour les chaines
		for (int32 i = 0; i < fixture->GetShape()->GetChildCount(); ++i)
		{
			// Combine cette aabb avec l'aabb générale
			aabb.Combine(aabb, fixture->GetAABB(i));
		}
	}

	return aabb;
}
sf::FloatRect BaseBody::GetsfBoundingBox(void) const
{
	b2AABB aabb(GetBoundingBox());
	float ppm = PhysicManager::GetInstance().GetPPM();
	return sf::FloatRect(aabb.upperBound.x * ppm, aabb.upperBound.y * ppm, aabb.lowerBound.x * ppm, aabb.lowerBound.y * ppm);
}

// Pour le rendu
void BaseBody::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	if (mBodyIsCreated)
		target.draw(mSprite, states);
}

/* Accesseurs */
// Est-ce que le Body est créé
bool BaseBody::IsCreated() const
{
	return mBodyIsCreated;
}
// Type de b2Body
b2BodyType BaseBody::Getb2BodyType() const
{
	myAssert(mBody, "b2Body null");
	return mBody->GetType();
}
void BaseBody::Setb2BodyType(const b2BodyType &type)
{
	// Change le type
	mBody->SetType(type);
	mHasMoved = true;

	// Rétablie la masse
	if (type == b2_dynamicBody && mBody->GetMass() == 0)
	{
		mBody->ResetMassData();
	}
}
// Type de collision
void BaseBody::SetCollisionType(BaseBody::CollisionType type)
{
	mCollisionType = type;
	mHasMoved = true;
}
BaseBody::CollisionType BaseBody::GetCollisionType() const
{
	return mCollisionType;
}
// Paramètres de collision
float BaseBody::GetDensity() const
{
	myAssert(mBody, "b2Body null");
	myAssert(mBody->GetFixtureList(), "b2Body sans fixture attaché.");
	return mBody->GetFixtureList()->GetDensity();
}
void BaseBody::SetDensity(float density)
{
	myAssert(mBody, "b2Body null");
	myAssert(mBody->GetFixtureList(), "b2Body sans fixture attaché.");
	mBody->GetFixtureList()->SetDensity(density);
	mBody->ResetMassData();
}
float BaseBody::GetFriction() const
{
	myAssert(mBody, "b2Body null");
	myAssert(mBody->GetFixtureList(), "b2Body sans fixture attaché.");
	return mBody->GetFixtureList()->GetFriction();
}
void BaseBody::SetFriction(float friction)
{
	myAssert(mBody, "b2Body null");
	myAssert(mBody->GetFixtureList(), "b2Body sans fixture attaché.");
	mBody->GetFixtureList()->SetFriction(friction);
}
float BaseBody::GetRestitution() const
{
	myAssert(mBody, "b2Body null");
	myAssert(mBody->GetFixtureList(), "b2Body sans fixture attaché.");
	return mBody->GetFixtureList()->GetRestitution();
}
void BaseBody::SetRestitution(float restitution)
{
	myAssert(mBody, "b2Body null");
	myAssert(mBody->GetFixtureList(), "b2Body sans fixture attaché.");
	mBody->GetFixtureList()->SetRestitution(restitution);
}
// Texture
Texture::Ptr BaseBody::GetTexture()
{
	return mTexture;
}
void BaseBody::SetTexture(Texture::Ptr texture)
{
	mTexture = texture;
	myAssert(mTexture.get(), "La texture n'est pas chargée.");
	mSprite.setTexture(*mTexture);
	mSprite.setTextureRect(sf::IntRect(sf::Vector2i(0, 0), u2i(mTexture->getSize())));
	mSprite.setOrigin(mSprite.getTextureRect().width / 2.f, mSprite.getTextureRect().height / 2.f);

	mHasMoved = true;
}
// Sprite
sf::Sprite* BaseBody::GetSprite()
{
	return &mSprite;
}
const sf::Sprite* BaseBody::GetSprite() const
{
	return &mSprite;
}
// Body
b2Body* BaseBody::GetBody()
{
	return mBody;
}
const b2Body* BaseBody::GetBody() const
{
	return mBody;
}
// Position et rotation
b2Vec2 BaseBody::GetPosition() const
{
	myAssert(mBody, "b2Body null");
	return mBody->GetPosition();
}
float BaseBody::GetRotationD() const // Degrés
{
	myAssert(mBody, "b2Body null");
	return mBody->GetAngle() * DPR;
}
float BaseBody::GetRotationR() const // Radians
{
	myAssert(mBody, "b2Body null");
	return mBody->GetAngle();
}
// Fonction à n'employer que pour éditer les niveaux
void BaseBody::SetTransform(const b2Vec2 &position, float angle)
{
	myAssert(mBody, "b2Body null");
	mBody->SetTransform(position, angle);
	mBody->SetAwake(true);
	mHasMoved = true;
}
