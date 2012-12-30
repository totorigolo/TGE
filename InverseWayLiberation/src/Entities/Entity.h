#pragma once
#include <SFML/Graphics.hpp>

enum class EntityType
{
	Entity,
	Deco,
	RawBody,
	Ragdoll
};

class Entity : public sf::Drawable
{
public:
	// Ctor & dtor
	Entity(int layer = 0);
	virtual ~Entity();

	// Mise à jour
	virtual void Update();

	// Pour le rendu
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const = 0;

	// Accesseurs
	bool IsAlive() const;
	int GetLayer() const;

protected:
	// Etat
	bool mIsAlive;

	// Propriétés
	int mLayer;
	EntityType mType;
};