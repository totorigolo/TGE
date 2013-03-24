#include "Deco.h"
#include "../Tools/utils.h"

// Ctor & dtor
Deco::Deco(int layer, const std::shared_ptr<sf::Texture> &texture, sf::Vector3f posRot)
	: Entity(layer), mTexture(texture)
{
	assert(mTexture.get() && "n'est pas valide.");

	mType = EntityType::Deco;

	// Cr�e le Sprite
	mSprite.setTexture(*mTexture);
	mSprite.setPosition(getVec2(posRot));
	mSprite.setRotation(posRot.z);
	mSprite.setOrigin(u2f(mSprite.getTexture()->getSize()) / 2.f);
}
Deco::~Deco()
{
}

// Mise � jour
void Deco::Update()
{
	// Rien � faire
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
