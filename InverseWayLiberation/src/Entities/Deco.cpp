#include "Deco.h"
#include "../Tools/utils.h"

// Ctor & dtor
Deco::Deco(int layer, sf::Sprite *sprite)
	: Entity(layer), mSpriteIsMine(false), mSprite(sprite)
{
	mType = EntityType::Deco;
}
Deco::Deco(int layer, sf::Texture *texture, sf::Vector3f posRot)
	: Entity(layer), mSpriteIsMine(false), mSprite(nullptr)
{
	mType = EntityType::Deco;

	// Crée le Sprite
	mSprite = new sf::Sprite(*texture);
	mSpriteIsMine = (mSprite != nullptr);
	mSprite->setPosition(getVec2(posRot));
	mSprite->setRotation(posRot.z);
	mSprite->setOrigin(u2f(mSprite->getTexture()->getSize()) / 2.f);
}
Deco::~Deco()
{
	if (mSpriteIsMine)
		delete mSprite;
	mSprite = nullptr;
}

// Mise à jour
void Deco::Update()
{
	// Rien à faire
}

// Pour le rendu
void Deco::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(*mSprite, states);
}

// Accesseurs
bool Deco::IsItMySprite() const
{
	return mSpriteIsMine;
}
const sf::Sprite* Deco::GetSprite() const
{
	return mSprite;
}
