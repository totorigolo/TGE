#pragma once

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
	void RegisterEntity(Entity *entity); // Les Entities s'enregistrent toutes seules
	void DestroyEntity(Entity *entity);
	void DestroyAllEntities();
	void SortByLayer();

	// Accès aux Entities
	Entity* GetEntity(unsigned int id);
	std::list<Entity*>& GetEntities();
	const std::list<Entity*>& GetEntities() const;

	// Gestion des IDs
	// Le système actuel garanti qu'il n'y a pas deux fois le même ID lors d'une session
	// sauf si DestroyAllEntities() est appelé
	unsigned int GetNewID();
	void RemoveID(unsigned int id);

protected:
	// Pour le rendu
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

private:
	// Liste des Entities
	std::list<Entity*> mEntities;

	// Liste des IDs
	std::set<unsigned int> mIDs;
	unsigned int mLastId;
};