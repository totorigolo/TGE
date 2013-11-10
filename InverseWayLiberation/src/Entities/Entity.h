#pragma once
#include "../Tools/NonCopyable.h"

#include <SFML/Graphics.hpp>

enum class EntityType
{
	Entity,
	Deco,
	LivingBeing,
	Player,
	BasicBody,
	Grapnel
};

class Entity : public sf::Drawable, public NonCopyable
{
public:
	// Ctor & dtor
	Entity(int layer = 0, unsigned int ID = 0U);
	virtual ~Entity();

	// Mise à jour
	virtual void Update();

	// Pour le rendu
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const = 0;

	// Accesseurs
	bool IsAlive() const;
	unsigned int GetID() const;
	int GetLayer() const;
	EntityType GetType() const { return mType; }

	void SetOwner(Entity *e) { mOwner = e; }
	Entity* GetOwner() { return mOwner; }
	const Entity* GetOwner() const { return mOwner; }

protected:
	// Etat
	bool mIsAlive;

	// Identifiant
	// Les ID ne sont pas uniques. 0 est l'ID par défaut
	unsigned int mID;

	// Propriétés
	int mLayer;
	EntityType mType;
	Entity *mOwner;
};