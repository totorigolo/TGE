#include "EntityManager.h"
#include "Entity.h"
#include "../Tools/Error.h"

// Ctor & dtor
EntityManager::EntityManager()
{
}
EntityManager::~EntityManager()
{
}

// Mise à jour
void EntityManager::Update()
{
	// Met à jour toutes les Entities
	for (std::list<Entity*>::iterator it = mEntities.begin(); it != mEntities.end(); ++it)
	{
		(*it)->Update();
	}
}

// Gestion des Entities
void EntityManager::RegisterEntity(Entity *entity)
{
	// Vérifie que le pointeur soit valide
	myAssert(entity, "L'entity n'est pas valide !");

	// Ajoute l'Entity à la liste
	mEntities.push_back(entity);
}
void EntityManager::DestroyEntity(Entity *entity)
{
	// Vérifie que le pointeur soit valide
	myAssert(entity, "L'entity n'est pas valide !");

	mEntities.remove(entity);
	delete entity;
}
void EntityManager::DestroyAllEntities()
{
	// Supprime toutes les Entities
	for (std::list<Entity*>::iterator it = mEntities.begin(); it != mEntities.end(); )
	{
		delete *it;
		it = mEntities.erase(it);
	}
	mEntities.clear();
}
bool compareEntities(Entity *a, Entity *b) // Compare deux Entities avec leur Layer
{
	return a->GetLayer() > b->GetLayer();
}
void EntityManager::SortByLayer()
{
	// Trie les Entities
	mEntities.sort(compareEntities);
}

// Accès aux Entities
const std::list<Entity*>& EntityManager::GetEntities() const
{
	return mEntities;
}

// Pour le rendu
void EntityManager::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	// Affiche toutes les Entities
	for (std::list<Entity*>::const_iterator it = mEntities.begin(); it != mEntities.end(); ++it)
	{
		target.draw(**it, states);
	}
}
