#pragma once
#include "Entity.h"
#include "../Resources/Texture.h"

class Player : public Entity
{
public:
	// Ctor & dtor
	Player(int layer = 1, unsigned int ID = 0U);
	virtual ~Player();

	// Création du Player
	bool Create(b2Vec3 posRot);

	// Mise à jour
	virtual void Update();

	// Accesseurs
	b2Body* GetBody();
	b2Vec2 GetPosition() const;

protected:
	// Pour le rendu
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

private:
	// ResourceMgr
	ResourceManager &mResourceMgr;

	// b2Body
	b2Body *mBody;

	// Textures
	sf::Sprite mSprite;
	std::map<std::string, Texture::Ptr> mTexturesMap;
};