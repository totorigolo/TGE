#include "Entity.h"

// Ctor & dtor
Entity::Entity(int layer)
	: mIsAlive(false), mLayer(layer), mToDeleteOnDestroy(true)
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
int Entity::GetLayer() const
{
	return mLayer;
}
