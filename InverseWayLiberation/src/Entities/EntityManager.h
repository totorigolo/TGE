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
	// Mise � jour
	void Update();

	// Gestion des Entities
	void RegisterEntity(Entity *entity); // Les Entities s'enregistrent toutes seules
	void DestroyEntity(Entity *entity);
	void DestroyAllEntities();
	void SortByLayer();

	// Acc�s aux Entities
	Entity* GetEntity(unsigned int id);
	std::list<Entity*>& GetEntities();
	const std::list<Entity*>& GetEntities() const;

	// Gestion des IDs
	// Le syst�me actuel garanti qu'il n'y a pas deux fois le m�me ID lors d'une session
	// sauf si DestroyAllEntities() est appel�
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