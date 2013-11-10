#pragma once
#include "Entity.h"
#include "../Resources/Texture.h"

#include <SFML/Graphics.hpp>
#include <Box2D/Box2D.h>
#include <memory>

class Deco : public Entity
{
public:
	// Ctor & dtor
	Deco(int layer, const std::shared_ptr<Texture> &texture, sf::Vector3f posRot, unsigned int ID = 0U); // PosRot pixel / degrés
	virtual ~Deco();

	// Mise à jour
	virtual void Update();

	// Pour le rendu
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

	// Accesseurs
	sf::Sprite* GetSprite();
	const sf::Sprite* GetSprite() const;
	const b2Vec2 GetPosition() const;
	const float GetRotation() const;

private:
	sf::Sprite mSprite;
	std::shared_ptr<Texture> mTexture;
};