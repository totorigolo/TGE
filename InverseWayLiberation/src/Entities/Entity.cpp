#include "stdafx.h"
#include "Entity.h"
#include "EntityManager.h"

// Ctor & dtor
Entity::Entity(int layer, unsigned int ID)
	: mIsAlive(false), mID(ID), mLayer(layer), mOwner(nullptr)
{
	mType = EntityType::Entity;

	// S'enregistre dans l'EntityManager
	EntityManager::GetInstance().RegisterEntity(this);
}
Entity::~Entity()
{
}

// Mise à jour
void Entity::Update()
{
}

// Gestion du Layer
void Entity::SetLayer(int layer)
{
	mLayer = layer;

	// Trie les Entities
	EntityManager::GetInstance().SortByLayer();
}
int Entity::GetLayer() const
{
	return mLayer;
}

// Accesseurs
bool Entity::IsAlive() const
{
	return mIsAlive;
}
unsigned int Entity::GetID() const
{
	myAssert(mID != 0U, "L'ID 0 ne devrait pas exister.");

	return mID;
}
