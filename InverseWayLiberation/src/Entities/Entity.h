#pragma once

enum class EntityType
{
	Entity,
	Deco,
	Player,
	BaseBody,
		BasicBody,
		PolyBody,
		PolyChain,
	Grapnel,
	PointLight
};

class Entity : public sf::Drawable, public NonCopyable
{
public:
	// Ctor & dtor
	Entity(int layer = 0, unsigned int ID = 0U);
	virtual ~Entity();

	// Mise à jour
	virtual void Update();

	// Pour le rendu
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const = 0;

	// Gestion du Layer
	void SetLayer(int layer);
	int GetLayer() const;

	// Accesseurs
	bool IsAlive() const;
	unsigned int GetID() const;
	EntityType GetType() const { return mType; }

	void SetOwner(Entity *e) { mOwner = e; }
	Entity* GetOwner() { return mOwner; }
	const Entity* GetOwner() const { return mOwner; }

private:
	friend class EntityManager;

	// Identifiant (uniques)
	unsigned int mID;

protected:
	// Etat
	bool mIsAlive;

	// Propriétés
	int mLayer;
	EntityType mType;
	Entity *mOwner;
};