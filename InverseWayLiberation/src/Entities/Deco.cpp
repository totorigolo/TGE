#include "Deco.h"
#include "../Tools/utils.h"
#include "../Tools/Error.h"
#include "../Physics/PhysicManager.h"

// Ctor & dtor
Deco::Deco(int layer, const std::shared_ptr<Texture> &texture, sf::Vector3f posRot, unsigned int ID)
	: Entity(layer, ID), mTexture(texture)
{
	myAssert(mTexture.get(), "La texture n'est pas chargée.");

	mType = EntityType::Deco;

	// Crée le Sprite
	mSprite.setTexture(*mTexture);
	mSprite.setPosition(getVec2(posRot));
	mSprite.setRotation(posRot.z);
	mSprite.setOrigin(u2f(mSprite.getTexture()->getSize()) / 2.f);
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

// Accesseurs
sf::Sprite* Deco::GetSprite()
{
	return &mSprite;
}
const sf::Sprite* Deco::GetSprite() const
{
	return &mSprite;
}
const b2Vec2 Deco::GetPosition() const
{
	return sf2b2Vec(mSprite.getPosition(), PhysicManager::GetInstance().GetMPP());
}
const float Deco::GetRotation() const
{
	return mSprite.getRotation();
}
