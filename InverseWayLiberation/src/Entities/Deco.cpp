#include "stdafx.h"
#include "Deco.h"
#include "../Physics/PhysicManager.h"

// Ctor & dtor
Deco::Deco(int layer, Texture::Ptr texture, sf::Vector3f posRot)
	: Entity(layer), mTexture(texture), mHull(this)
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

		// Règle le Hull
		mHull.Deactivate();
		auto gb = mSprite.getGlobalBounds();
		mHull.SetPosAndSize(sf::Vector2f(gb.left, gb.top), sf::Vector2f(gb.width, gb.height));
		mHull.SetDrawable(true);
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

	// Mise à jour du Hull
	auto gb = mSprite.getGlobalBounds();
	mHull.SetPosAndSize(sf::Vector2f(gb.left, gb.top), sf::Vector2f(gb.width, gb.height));
}

// Ombres
void Deco::ActivateShadows(void)
{
	mHull.Activate();
}
void Deco::DeactivateShadows(void)
{
	mHull.Deactivate();
}
void Deco::SetShadowsActive(bool active)
{
	mHull.SetActive(active);
}
bool Deco::IsActiveShadows(void) const
{
	return mHull.IsActive();
}

// Position et rotation
void Deco::SetPosition(const b2Vec2 &pos)
{
	mSprite.setPosition(b22sfVec(pos, PhysicManager::GetInstance().GetPPM()));

	// Mise à jour du Hull
	auto gb = mSprite.getGlobalBounds();
	mHull.SetPosition(sf::Vector2f(gb.left, gb.top));
}
void Deco::SetPosition(const sf::Vector2f &pos)
{
	mSprite.setPosition(pos);

	// Mise à jour du Hull
	auto gb = mSprite.getGlobalBounds();
	mHull.SetPosition(sf::Vector2f(gb.left, gb.top));
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

	// Mise à jour du Hull
	auto gb = mSprite.getGlobalBounds();
	mHull.SetSize(sf::Vector2f(gb.width, gb.height));
}
void Deco::SetRotationR(float rot)
{
	mSprite.setRotation(rot * DPR);

	// Mise à jour du Hull
	auto gb = mSprite.getGlobalBounds();
	mHull.SetSize(sf::Vector2f(gb.width, gb.height));
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
