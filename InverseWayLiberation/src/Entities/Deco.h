#pragma once
#include "Entity.h"
#include <SFML/Graphics.hpp>

class Deco : public Entity
{
public:
	// Ctor & dtor
	Deco(int layer, sf::Sprite *sprite);
	Deco(int layer, sf::Texture *texture, sf::Vector3f posRot); // PosRot pixel / degr�s
	virtual ~Deco();

	// Mise � jour
	virtual void Update();

	// Pour le rendu
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

	// Gestion des d�pendences
	virtual void DependencyDestroyed(void *dependency);

	// Accesseurs
	bool IsItMySprite() const;
	const sf::Sprite* GetSprite() const;

private:
	bool mSpriteIsMine;
	sf::Sprite *mSprite;
};