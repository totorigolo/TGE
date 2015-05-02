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
	Entity* GetEntity(const std::string &name);
	std::list<Entity*>& GetEntities();
	const std::list<Entity*>& GetEntities() const;

	// Gestion des IDs
	// Le système actuel garanti qu'il n'y a pas deux fois le même ID lors d'une session
	// sauf si DestroyAllEntities() est appelé
	unsigned int GetNewID();
	void RemoveID(unsigned int id);

	// Gestion des noms
	bool Name(const std::string &name, Entity *entity); // true if already exists
	void Anonymize(const std::string &name);
	std::string GetName(Entity *const entity) const;

protected:
	// Pour le rendu
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

private:
	// Changement d'ID pendant le chargement de niveau
	friend class LevelLoader;
	void ChangeID(unsigned int id, unsigned int newID);

private:
	// Liste des Entities
	std::list<Entity*> mEntities;

	// Liste des IDs
	std::set<unsigned int> mIDs;
	unsigned int mLastId;

	// Liste des noms
	typedef boost::bimap<std::string, Entity*> name_bimap;
	typedef name_bimap::value_type name_bimap_position;
	name_bimap mNames;
};