#pragma once
#include "../Tools/Singleton.h"

#include <SFML/Graphics.hpp>
#include <list>

class Entity;
class EntityManager : public Singleton<EntityManager>, public sf::Drawable
{
protected:
	friend class Singleton<EntityManager>;

	// Ctor & dtor
	EntityManager();
	virtual ~EntityManager();

public:
	// Mise à jour
	void Update();

	// Gestion des Entities
	void RegisterEntity(Entity *entity);
	void DestroyEntity(Entity *entity);
	void DestroyAllEntities();
	void SortByLayer();

	// Accès aux Entities
	const std::list<Entity*>& GetEntities() const;

	// Pour le rendu
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

private:
	// Liste des Entities
	std::list<Entity*> mEntities;
};