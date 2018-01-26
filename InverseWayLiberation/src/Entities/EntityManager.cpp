#include "EntityManager.h"
#include "Entity.h"
#include "../Tools/Error.h"

#include <iostream>

// Ctor & dtor
EntityManager::EntityManager()
        : mLastId(0U) {
}

EntityManager::~EntityManager() {
}

// Mise à jour
void EntityManager::Update() {
    // Supprime les Entities mortes
    for (std::list<Entity *>::iterator it = mEntities.begin(); it != mEntities.end();) {
        if (!(*it)->IsAlive())
            DestroyEntity(*(it++));
        else
            ++it;
    }

    // PreUpdate toutes les Entities
    for (std::list<Entity *>::iterator it = mEntities.begin(); it != mEntities.end(); ++it) {
        (*it)->PreUpdate();
    }
    // Update toutes les Entities
    for (std::list<Entity *>::iterator it = mEntities.begin(); it != mEntities.end(); ++it) {
        (*it)->Update();
    }
    // PostUpdate toutes les Entities
    for (std::list<Entity *>::iterator it = mEntities.begin(); it != mEntities.end(); ++it) {
        (*it)->PostUpdate();
    }
}

// Gestion des Entities
void EntityManager::RegisterEntity(Entity *entity) {
    // Vérifie que le pointeur soit valide
    myAssert(entity, "L'entity n'est pas valide !");

    // Ajoute l'Entity à la liste
    mEntities.push_back(entity);

    // Change l'ID de l'Entity
    entity->mID = GetNewID();
}

void EntityManager::DestroyEntity(Entity *entity) {
    // Vérifie que le pointeur soit valide
    myAssert(entity, "L'entity n'est pas valide !");

    // Enlève le nom si elle est nommée
    this->Anonymize(this->GetName(entity));

    // Enlève son ID de la liste des IDs
    this->RemoveID(entity->mID);

    // Supprime l'entité et l'enlève de la liste
    mEntities.remove(entity);
    delete entity;
}

void EntityManager::DestroyAllEntities() {
    // Supprime toutes les Entities
    for (std::list<Entity *>::iterator it = mEntities.begin(); it != mEntities.end();) {
        delete *it;
        it = mEntities.erase(it);
    }
    mEntities.clear();

    // Vide les noms
    mNames.clear();

    // Vide les IDs
    mIDs.clear();
    mLastId = 0U;
}

bool compareEntities(Entity *a, Entity *b) // Compare deux Entities avec leur Layer
{
    return a->GetLayer() > b->GetLayer();
}

void EntityManager::SortByLayer() {
    // Trie les Entities
    mEntities.sort(compareEntities);
}

// Accès aux Entities
Entity *EntityManager::GetEntity(unsigned int id) {
    // TODO: La liste est triée

    for (auto &&entity : mEntities)
        if (entity->GetID() == id)
            return entity;

    return nullptr;
}

Entity *EntityManager::GetEntity(const std::string &name) {
    auto it = mNames.left.find(name);
    if (it == mNames.left.end())
        return nullptr;
    return it->second;
}

std::list<Entity *> &EntityManager::GetEntities() {
    return mEntities;
}

const std::list<Entity *> &EntityManager::GetEntities() const {
    return mEntities;
}

// Gestion des IDs
unsigned int EntityManager::GetNewID() {
    mIDs.insert(++mLastId);
    return mLastId;
}

void EntityManager::RemoveID(unsigned int id) {
    myCheckError_v(id != 0U, "Il est impossible de supprimer l'ID d'Entity 0.");
    if (mIDs.find(id) == mIDs.end()) {
        std::cerr << "Tentative de suppression d'ID d'Entity inconnu (id: " << id << ")." << std::endl;
        return;
    };

    // Enlève l'ID de la liste
    mIDs.erase(id);
}

// Gestion des noms
bool EntityManager::Name(const std::string &name, Entity *entity) {
    bool r = false;
    auto it = mNames.left.find(name);
    if (it != mNames.left.end())
        r = true;
    mNames.insert(name_bimap_position(name, entity));
    return r;
}

void EntityManager::Anonymize(const std::string &name) {
    if (name.empty()) return;
    mNames.left.erase(name);
}

std::string EntityManager::GetName(Entity *const entity) const {
    auto it = mNames.right.find(entity);
    if (it == mNames.right.end())
        return "";
    return it->second;
}

// Pour le rendu
void EntityManager::draw(sf::RenderTarget &target, sf::RenderStates states) const {
    // Affiche toutes les Entities
    // TODO: Uniquement seules les présentes
    for (std::list<Entity *>::const_iterator it = mEntities.begin(); it != mEntities.end(); ++it) {
        target.draw(**it, states);
    }
}

// Changement d'ID pendant le chargement de niveau
void EntityManager::ChangeID(unsigned int id, unsigned int newID) {
    if (id == newID) return;

    // Récupère un pointeur vers l'Entity
    Entity * e1 = GetEntity(id);
    myAssert(e1, "Impossible de trouver d'Entity avec l'ID #" + Parser::intToString(id) + " !");

    // Regarde s'il y a une entity avec le nouvel ID
    Entity * e2 = GetEntity(newID);
    if (e2) {
        e2->mID = GetNewID();
    } else {
        this->RemoveID(id);
        mIDs.insert(newID);
    }
    e1->mID = newID;
}
