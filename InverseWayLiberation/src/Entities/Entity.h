#pragma once
#include <SFML/Graphics.hpp>
#include <list>

enum class EntityType
{
	Entity,
	Deco,
	RawBody,
	LivingBeing,
	Player,
	Ragdoll,
	BasicBody,
	Grapnel
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
	EntityType GetType() const { return mType; }

	void SetOwner(Entity *e) { mOwner = e; }
	Entity* GetOwner() { return mOwner; }
	const Entity* GetOwner() const { return mOwner; }

protected:
	// Etat
	bool mIsAlive;

	// Propriétés
	int mLayer;
	EntityType mType;
	Entity *mOwner;
};