#include "stdafx.h"
#include "EntityManager.h"
#include "Entity.h"

// Ctor & dtor
EntityManager::EntityManager()
	: mLastId(0U)
{
}
EntityManager::~EntityManager()
{
}

// Mise à jour
void EntityManager::Update()
{
	// Supprime les Entities mortes
	for (std::list<Entity*>::iterator it = mEntities.begin(); it != mEntities.end();)
	{
		if (!(*it)->IsAlive())
			DestroyEntity(*(it++));
		else
			++it;
	}

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

	// Change l'ID de l'Entity
	entity->mID = GetNewID();
}
void EntityManager::DestroyEntity(Entity *entity)
{
	// Vérifie que le pointeur soit valide
	myAssert(entity, "L'entity n'est pas valide !");

	// Enlève son ID de la liste des IDs
	this->RemoveID(entity->mID);

	// Supprime l'entité et l'enlève de la liste
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

	// Vide les IDs
	mIDs.clear();
	mLastId = 0U;
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
std::list<Entity*>& EntityManager::GetEntities()
{
	return mEntities;
}
const std::list<Entity*>& EntityManager::GetEntities() const
{
	return mEntities;
}

// Gestion des IDs
unsigned int EntityManager::GetNewID()
{
	mIDs.insert(++mLastId);
	return mLastId;
}
void EntityManager::RemoveID(unsigned int id)
{
	myCheckError_v(id != 0U, "Il est impossible de supprimer l'ID d'Entity 0.");
	myAssert(mIDs.find(id) != mIDs.end(), "Tentative de suppression d'ID d'Entity inconnu.");

	// Enlève l'ID de la liste
	mIDs.erase(id);
}

// Pour le rendu
void EntityManager::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	// Affiche toutes les Entities
	// TODO: Uniquement seules les présentes
	for (std::list<Entity*>::const_iterator it = mEntities.begin(); it != mEntities.end(); ++it)
	{
		target.draw(**it, states);
	}
}
