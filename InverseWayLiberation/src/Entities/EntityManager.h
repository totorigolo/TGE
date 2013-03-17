#pragma once
#include "Entity.h"
#include "../Tools/Singleton.h"
#include <list>

class EntityManager : public Singleton<EntityManager>, public sf::Drawable
{
public:
	// Ctor & dtor
	EntityManager();
	virtual ~EntityManager();

	// Mise à jour
	void Update();

	// Gestion des Entities
	void RegisterEntity(Entity *entity);
	void DestroyEntity(Entity *entity);
	void DestroyAllEntities();
	void SortByLayer();

	// Pour le rendu
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

private:
	// Liste des Entities
	std::list<Entity*> mEntities;
};