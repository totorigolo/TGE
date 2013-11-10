#include "Entity.h"

// Ctor & dtor
Entity::Entity(int layer, unsigned int ID)
	: mIsAlive(false), mID(ID), mLayer(layer), mOwner(nullptr)
{
	mType = EntityType::Entity;
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
	return mID;
}
int Entity::GetLayer() const
{
	return mLayer;
}
