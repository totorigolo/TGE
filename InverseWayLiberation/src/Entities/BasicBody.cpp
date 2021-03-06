#include "BasicBody.h"
#include "../Physics/PhysicManager.h"
#include "../Tools/utils.h"

// Ctor & dtor
BasicBody::BasicBody(int layer)
	: BaseBody(layer), mShape(Shape::Null)
{
	// Défini le type de l'Entity
	mType = EntityType::BasicBody;
}

// Mise à jour
void BasicBody::PreUpdate()
{
	BaseBody::PreUpdate();
}

// Création du body
bool BasicBody::CreateBox(b2Vec3 posRot, b2BodyType type, Texture::Ptr texture,
						  float density, float friction, float restitution, int groupIndex, uint16 categoryBits, uint16 maskBits)
{
	// On n'en crée pas de nouveau si il y en a déjà un
	if (mBodyIsCreated)
		return false;

	// Change la texture
	SetTexture(texture);

	// BodyDef
	b2BodyDef bodyDef;
	bodyDef.angle = posRot.z * RPD;
	bodyDef.position = getVec2(posRot);
	bodyDef.type = type;
	mBody = mPhysicMgr.CreateBody(&bodyDef);

	// Shape
	b2PolygonShape shape;
	shape.SetAsBox((mTexture->getSize().x / 2) * mPhysicMgr.GetMPP(), (mTexture->getSize().y / 2) * mPhysicMgr.GetMPP());
	shape.m_radius = 0.f;

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
	mBody->SetAwake(true);
	mBodyIsCreated = true;
	mShape = Shape::Box;
	mIsAlive = true;

	// Règle le Hull
	mHull.SetBodyShadowCaster(mBody);
	mHull.SetPhysicallyDrawable(true);
	mHull.Activate();

	return true;
}
bool BasicBody::CreateCircle(b2Vec3 posRot, b2BodyType type, Texture::Ptr texture,
							 float density, float friction, float restitution, int groupIndex, uint16 categoryBits, uint16 maskBits)
{
	// On n'en crée pas de nouveau si il y en a déjà un
	if (mBodyIsCreated)
		return false;

	// Change la texture
	SetTexture(texture);

	// BodyDef
	b2BodyDef bodyDef;
	bodyDef.angle = posRot.z * RPD;
	bodyDef.position = getVec2(posRot);
	bodyDef.type = type;
	mBody = mPhysicMgr.CreateBody(&bodyDef);

	// Shape
	b2CircleShape shape;
	shape.m_radius = mSprite.getTexture()->getSize().x / 2.f * mPhysicMgr.GetMPP();

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
	mShape = Shape::Circle;
	mIsAlive = true;

	// Règle le Hull
	mHull.SetBodyShadowCaster(mBody);
	mHull.SetPhysicallyDrawable(true);
	mHull.Activate();

	return true;
}

// Destruction du body
void BasicBody::Destroy()
{
	BaseBody::Destroy();
	mShape = Shape::Null;
}

/* Accesseurs */
// Forme du Body
BasicBody::Shape BasicBody::GetBasicBodyShape() const
{
	return mShape;
}
