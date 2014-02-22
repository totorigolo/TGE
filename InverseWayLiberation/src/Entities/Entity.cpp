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

// Accesseurs
bool Entity::IsAlive() const
{
	return mIsAlive;
}
unsigned int Entity::GetID() const
{
	myAssert(mID != 0U, "L'ID 0 ne devrait pas exister.\nDans Entity::GetID().");

	return mID;
}
int Entity::GetLayer() const
{
	return mLayer;
}
