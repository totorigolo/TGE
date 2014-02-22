#include "stdafx.h"
#include "BasicBody.h"
#include "../Physics/PhysicManager.h"

// Ctor & dtor
BasicBody::BasicBody(int layer, unsigned int ID)
	: BaseBody(layer, ID), mShape(Shape::Null)
{
	// Défini le type de l'Entity
	mType = EntityType::BasicBody;
}

// Création du body
bool BasicBody::CreateDynBox(b2Vec3 posRot, const std::shared_ptr<Texture> texture,
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
	bodyDef.type = b2_dynamicBody;
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
	mBodyIsCreated = true;
	mShape = Shape::Box;

	return true;
}
bool BasicBody::CreateDynCircle(b2Vec3 posRot, const std::shared_ptr<Texture> texture,
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
	bodyDef.type = b2_dynamicBody;
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

	return true;
}
bool BasicBody::CreateStaticBox(b2Vec3 posRot, const std::shared_ptr<Texture> texture,
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
	bodyDef.type = b2_staticBody;
	mBody = mPhysicMgr.CreateBody(&bodyDef);

	// Shape
	b2PolygonShape shape;
	shape.SetAsBox((mSprite.getTexture()->getSize().x / 2) * mPhysicMgr.GetMPP(),
										(mSprite.getTexture()->getSize().y / 2) * mPhysicMgr.GetMPP());
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
	mBodyIsCreated = true;
	mShape = Shape::Box;

	return true;
}
bool BasicBody::CreateStaticCircle(b2Vec3 posRot, const std::shared_ptr<Texture> texture,
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
	bodyDef.type = b2_staticBody;
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
