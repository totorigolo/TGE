#include "BasicBody.h"
#include "../Tools/utils.h"

// Ctor & dtor
BasicBody::BasicBody(PhysicManager *mgr, int layer)
	: Entity(layer), mBasicBodyType(BasicBodyType::Null), mPhysicMgr(mgr), mBody(nullptr), mBodyIsCreated(false)
{
	assert(mPhysicMgr && "n'est pas valide.");

	// Défini le type de l'Entity
	mType = EntityType::BasicBody;
}
BasicBody::~BasicBody()
{
	Destroy();
}

// Création du body
bool BasicBody::CreateDynBox(b2Vec3 posRot, std::shared_ptr<sf::Texture> texture, float density, float friction, float restitution
																				, int groupIndex, uint16 categoryBits, uint16 maskBits)
{
	// On n'en crée pas de nouveau si il y en a déjà un
	if (mBodyIsCreated)
		return false;

	// Change la texture
	mSprite.setTexture(*texture);

	// BodyDef
	b2BodyDef bodyDef;
	bodyDef.angle = posRot.z * RPD;
	bodyDef.position = getVec2(posRot);
	bodyDef.type = b2_dynamicBody;
	mBody = mPhysicMgr->CreateBody(&bodyDef);

	// Shape
	b2PolygonShape shape;
	shape.SetAsBox((texture->getSize().x / 2) * mPhysicMgr->GetMPP(), (texture->getSize().y / 2) * mPhysicMgr->GetMPP());
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
	mBasicBodyType = BasicBodyType::DynamicBox;

	return true;
}
bool BasicBody::CreateDynCircle(b2Vec3 posRot, std::shared_ptr<sf::Texture> texture, float density, float friction, float restitution
																				   , int groupIndex, uint16 categoryBits, uint16 maskBits)
{
	// On n'en crée pas de nouveau si il y en a déjà un
	if (mBodyIsCreated)
		return false;

	// Change la texture
	mSprite.setTexture(*texture);

	// BodyDef
	b2BodyDef bodyDef;
	bodyDef.angle = posRot.z * RPD;
	bodyDef.position = getVec2(posRot);
	bodyDef.type = b2_dynamicBody;
	mBody = mPhysicMgr->CreateBody(&bodyDef);

	// Shape
	b2CircleShape* shape = new b2CircleShape;
	shape->m_radius = mSprite.getTexture()->getSize().x / 2.f * mPhysicMgr->GetMPP();

	// Fixture
	b2FixtureDef fixtureDef;
	fixtureDef.density = density;
	fixtureDef.friction = friction;
	fixtureDef.restitution = restitution;
	fixtureDef.filter.groupIndex = static_cast<int16>(groupIndex);
	fixtureDef.filter.categoryBits = categoryBits;
	fixtureDef.filter.maskBits = maskBits;
	fixtureDef.shape = shape;
	mBody->CreateFixture(&fixtureDef);

	// Enregistrements
	mBody->SetUserData(this);
	mBodyIsCreated = true;
	mBasicBodyType = BasicBodyType::DynamicCircle;

	return true;
}
bool BasicBody::CreateStaticBox(b2Vec3 posRot, std::shared_ptr<sf::Texture> texture, float friction, float restitution
																				   , int groupIndex, uint16 categoryBits, uint16 maskBits)
{
	// On n'en crée pas de nouveau si il y en a déjà un
	if (mBodyIsCreated)
		return false;

	// Change la texture
	mSprite.setTexture(*texture);

	// BodyDef
	b2BodyDef bodyDef;
	bodyDef.angle = posRot.z * RPD;
	bodyDef.position = getVec2(posRot);
	bodyDef.type = b2_staticBody;
	mBody = mPhysicMgr->CreateBody(&bodyDef);

	// Shape
	b2PolygonShape* shape = new b2PolygonShape;
	shape->SetAsBox((mSprite.getTexture()->getSize().x / 2) * mPhysicMgr->GetMPP(),
										(mSprite.getTexture()->getSize().y / 2) * mPhysicMgr->GetMPP());
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
	
	// Enregistrements
	mBody->SetUserData(this);
	mBodyIsCreated = true;
	mBasicBodyType = BasicBodyType::StaticBox;

	return true;
}

// Mise à jour
void BasicBody::Update()
{
	// Si le body est valide
	if (mBody && mBodyIsCreated)
	{
		// Mise à jour de la texture
		mSprite.setPosition(b22sfVec(mBody->GetPosition(), mPhysicMgr->GetPPM()));
		mSprite.setOrigin(mSprite.getTextureRect().width / 2.f, mSprite.getTextureRect().height / 2.f); // TODO: utile ?
		mSprite.setRotation(- mBody->GetAngle() * DPR);
	}
	else if (mBodyIsCreated)
	{
		// TODO: Suppression et suicide -> erreur
	}
}

// Destruction du body
void BasicBody::Destroy()
{
	if (mBody)
	{
		// Supprime le Body
		mPhysicMgr->DestroyBody(mBody);
		mBody = nullptr;
		mBasicBodyType = BasicBodyType::Null;
		mBodyIsCreated = false;
	}
}

// Pour le rendu
void BasicBody::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	if (mBodyIsCreated)
		target.draw(mSprite, states);
}

/* Accesseurs */
// Type de BasicBody
BasicBodyType BasicBody::GetBasicBodyType() const
{
	return mBasicBodyType;
}
// Sprite
sf::Sprite* BasicBody::GetSprite()
{
	return &mSprite;
}
const sf::Sprite* BasicBody::GetSprite() const
{
	return &mSprite;
}
// Body
b2Body* BasicBody::GetBody()
{
	return mBody;
}
const b2Body* BasicBody::GetBody() const
{
	return mBody;
}
// PhysicManager
PhysicManager* BasicBody::GetPhysicManager()
{
	return mPhysicMgr;
}
const PhysicManager* BasicBody::GetPhysicManager() const
{
	return mPhysicMgr;
}
