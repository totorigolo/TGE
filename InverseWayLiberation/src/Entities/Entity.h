#pragma once
#include <SFML/Graphics.hpp>

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

	void DeleteOnDestroy(bool b) { mToDeleteOnDestroy = b; }
	bool HasToBeDeletedOnDestroy() const { return mToDeleteOnDestroy; }

protected:
	// Etat
	bool mIsAlive;

	// Propriétés
	int mLayer;
	EntityType mType;
	// Est-ce que l'EntityManager doit delete cette Entity en supprimant tout ?
	bool mToDeleteOnDestroy;
};