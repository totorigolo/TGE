#include "Deco.h"
#include "../Tools/utils.h"
#include "../Tools/Error.h"

// Ctor & dtor
Deco::Deco(int layer, const std::shared_ptr<sf::Texture> &texture, sf::Vector3f posRot)
	: Entity(layer), mTexture(texture)
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
sf::Sprite& Deco::GetSprite()
{
	return mSprite;
}
const sf::Sprite& Deco::GetSprite() const
{
	return mSprite;
}
