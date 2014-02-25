#include "stdafx.h"
#include "Deco.h"
#include "../Physics/PhysicManager.h"

// Ctor & dtor
Deco::Deco(int layer, const std::shared_ptr<Texture> &texture, sf::Vector3f posRot, unsigned int ID)
	: Entity(layer, ID), mTexture(texture)
{
	myAssert(mTexture.get(), "La texture n'est pas chargée.");

	mType = EntityType::Deco;

	SetTexture(texture);
	SetPosition(getVec2(posRot));
	SetRotationD(posRot.z);

	/*/ Crée le Sprite
	mSprite.setTexture(*mTexture);
	mSprite.setPosition(getVec2(posRot));
	mSprite.setRotation(posRot.z);
	mSprite.setOrigin(u2f(mSprite.getTexture()->getSize()) / 2.f);*/

	mIsAlive = true;
}
Deco::~Deco()
{
}

// Mise à jour
void Deco::Update()
{
	// Rien à faire
}

// Pour le rendu
void Deco::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(mSprite, states);
}

// Texture
void Deco::SetTexture(const std::shared_ptr<Texture> &texture)
{
	mTexture = texture;
	mSprite.setTexture(*mTexture);
	mSprite.setOrigin(u2f(mSprite.getTexture()->getSize()) / 2.f);
}

// Position et rotation
void Deco::SetPosition(const b2Vec2 &pos)
{
	mSprite.setPosition(b22sfVec(pos, PhysicManager::GetInstance().GetPPM()));
}
void Deco::SetPosition(const sf::Vector2f &pos)
{
	mSprite.setPosition(pos);
}
const b2Vec2 Deco::GetPosition() const
{
	return sf2b2Vec(mSprite.getPosition(), PhysicManager::GetInstance().GetMPP());
}
const sf::Vector2f Deco::GetPositionPx() const
{
	return mSprite.getPosition();
}
void Deco::SetRotationD(float rot)
{
	mSprite.setRotation(rot);
}
void Deco::SetRotationR(float rot)
{
	mSprite.setRotation(rot * DPR);
}
const float Deco::GetRotationD() const
{
	return mSprite.getRotation();
}
const float Deco::GetRotationR() const
{
	return mSprite.getRotation() * RPD;
}

// Accesseurs
sf::Sprite* Deco::GetSprite()
{
	return &mSprite;
}
const sf::Sprite* Deco::GetSprite() const
{
	return &mSprite;
}
