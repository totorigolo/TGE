#include "stdafx.h"
#include "Player.h"
#include "../Resources/ResourceManager.h"

// Ctor
Player::Player(int layer, unsigned int ID)
: Entity(layer, ID), mResourceMgr(ResourceManager::GetInstance()), mBody(nullptr)
{
	// Défini le type de l'Entity
	mType = EntityType::Player;
}

// Dtor
Player::~Player()
{
}

// Création du Player
bool Player::Create(b2Vec3 posRot)
{
	// Remplit la TextureMap


	// Création du Body


	mIsAlive = true;
	return true;
}

// Mise à jour
void Player::Update()
{
	if (mTexturesMap.size() > 0)
		mSprite.setTexture(*mTexturesMap.begin()->second.get());
}

// Accesseurs
b2Body* Player::GetBody()
{
	return mBody;
}
b2Vec2 Player::GetPosition() const
{
	return b2Vec2_zero;
}

// Pour le rendu
void Player::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(mSprite, states);
}
