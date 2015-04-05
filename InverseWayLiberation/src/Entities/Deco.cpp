#include "stdafx.h"
#include "Deco.h"
#include "../Physics/PhysicManager.h"

// Ctor & dtor
Deco::Deco(int layer, Texture::Ptr texture, sf::Vector3f posRot)
	: Entity(layer), mTexture(texture)
{
	if (!mTexture.get())
	{
		Dialog::Error("La texture n'est pas chargée.\nDéco supprimée");
		mIsAlive = false;
	}
	else
	{
		mType = EntityType::Deco;

		SetTexture(texture);
		SetPosition(getVec2(posRot));
		SetRotationD(posRot.z);

		mIsAlive = true;
	}
}
Deco::~Deco()
{
}

// Pour le rendu
void Deco::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	if (!mIsAlive) return;

	target.draw(mSprite, states);
}

// Texture
Texture::Ptr& Deco::GetTexture(void)
{
	return mTexture;
}
void Deco::SetTexture(Texture::Ptr texture)
{
	mTexture = texture;
	mSprite.setTexture(*mTexture, true);
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
