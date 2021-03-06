#include "Entity.h"
#include "EntityManager.h"
#include "../Tools/Error.h"

// Ctor & dtor
Entity::Entity(int layer)
        : mID(0U),
          mIsAlive(false),
          mLayer(layer),
          mOwner(nullptr) {
    mType = EntityType::Entity;

    // S'enregistre dans l'EntityManager
    EntityManager::GetInstance().RegisterEntity(this);
}

Entity::~Entity() {
    // TODO: Ne se désenregistre pas en mourrant ?
}

// Mise à jour
void Entity::PreUpdate() {
}

void Entity::Update() {
}

void Entity::PostUpdate() {
}

// Gestion du Layer
void Entity::SetLayer(int layer) {
    mLayer = layer;

    // Trie les Entities
    EntityManager::GetInstance().SortByLayer();
}

int Entity::GetLayer() const {
    return mLayer;
}

// Accesseurs
bool Entity::IsAlive() const {
    return mIsAlive;
}

unsigned int Entity::GetID() const {
    myAssert(mID != 0U, "L'ID 0 ne devrait pas exister.");

    return mID;
}
