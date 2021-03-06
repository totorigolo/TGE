#pragma once

#include <SFML/Graphics.hpp>
#include "../Tools/NonCopyable.h"

enum class EntityType
{
	Entity,
	Deco,
	Player,
	BaseBody,
		BasicBody,
		PolyBody,
		PolyChain,
	PointLight,
	Hum
};

class Entity : public sf::Drawable, public NonCopyable
{
public:
	// Ctor & dtor
	Entity(int layer = 0);
	virtual ~Entity();

	// Mise à jour
	virtual void PreUpdate();
	virtual void Update();
	virtual void PostUpdate();

	// Pour le rendu
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const = 0;

	// Gestion du Layer
	void SetLayer(int layer);
	int GetLayer() const;

	// Accesseurs
	bool IsAlive() const;
	unsigned int GetID() const;
	EntityType GetType() const { return mType; }

	void SetOwner(Entity *e) { mOwner = e; }
	Entity* GetOwner() { return mOwner; }
	const Entity* GetOwner() const { return mOwner; }

private:
	friend class EntityManager;

	// Identifiant (uniques)
	unsigned int mID;

protected:
	// Etat
	bool mIsAlive;

	// Propriétés
	int mLayer;
	EntityType mType;
	Entity *mOwner;
};