#pragma once
#include <SFML/Graphics.hpp>
#include <Box2D/Box2D.h>
#include <Thor/Resources.hpp>
#include <Thor/Animation.hpp>
#include "../World.h"
#include "../Bodies/Body.h"
#include "../Bodies/DynamicBox.h"
#include "Ragdoll.h"

class Body;
class World;
class Entity
{
public:
	// Ctor & dtor
	Entity(World *world, sf::Texture *texture, b2Vec3 posRot);// Quand il y en aura plusieurs types, Ragdoll *ragdoll);
	virtual ~Entity(void);
	
	// Met à jour le perso
	virtual void Update();

	// Change les composants
	virtual void SetSprite(sf::Sprite *newSprite);

	// Change les attributs de l'Entité
	virtual void Kill();
	virtual void Heal(float amount);
	virtual void AddDamage(float damage);

	// Accesseurs
	bool IsAlive() const;
	float GetLife() const;

protected:
	World *mWorld;

	// Pour l'affichage étant "vivant"
	sf::Sprite *mSprite;
	thor::Animator<sf::Sprite, int> mAnimation;
	sf::Clock mElapsedTime;

	// Objets physique
	DynamicBox *mDynamicBox;
	Ragdoll *mRagdoll;

	// Variables relatives à la vie
	float mLife; // De 0 à 100
	bool mIsAlive;
};
