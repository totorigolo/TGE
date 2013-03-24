#pragma once
#include <SFML/Graphics.hpp>
#include <memory>
#include "Entity.h"

class Deco : public Entity
{
public:
	// Ctor & dtor
	Deco(int layer, const std::shared_ptr<sf::Texture> &texture, sf::Vector3f posRot); // PosRot pixel / degrés
	virtual ~Deco();

	// Mise à jour
	virtual void Update();

	// Pour le rendu
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

	// Accesseurs
	sf::Sprite& GetSprite();
	const sf::Sprite& GetSprite() const;

private:
	sf::Sprite mSprite;
	std::shared_ptr<sf::Texture> mTexture;
};