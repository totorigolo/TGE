#pragma once

class Cell;
class Hull
{
public:
	// Ctor & dtor
	Hull(sf::Drawable const * shadowCaster);
	Hull(sf::Drawable const * shadowCaster, const sf::Vector2f& pos, const sf::Vector2f& size); // pos : top left
	virtual ~Hull(void);

	// Changement de position / taille
	void SetPosition(const sf::Vector2f& pos); // pos = top left
	void SetSize(const sf::Vector2f& size);
	void SetPosAndSize(const sf::Vector2f& pos, const sf::Vector2f& size);

	// Mise à jour dans le Partitioning System
	void Update(void);

	// Gestion de l'activité du Hull
	void Activate(void);
	void Deactivate(void);
	void SetActive(bool active);
	bool IsActive(void) const;

	// Gestion des Cells
	void RegisterCell(Cell *cell);
	void UnregisterCell(Cell *cell);
	std::list<Cell*>& GetRegisteredCells(void);

	// S'enlève de toutes les Cells
	void RemoveFromCells(void);

	// Gestion du Shadow Caster
	void SetDrawable(bool drawable);
	bool IsDrawable(void) const;
	void SetPhysicallyDrawable(bool physicallyDrawable);
	bool IsPhysicallyDrawable(void) const;
	void SetShadowCaster(sf::Drawable const * shadowCaster);
	void SetBodyShadowCaster(b2Body const * bodyShadowCaster);
	sf::Drawable const* GetShadowCaster() const;
	b2Body const* GetBodyShadowCaster() const;

	// Accesseurs
	sf::Vector2f GetPosition(void) const;
	sf::Vector2f GetSize(void) const;
	bool IsValid(void) const;

private:
	// Propriétés
	bool mIsValid;
	bool mIsActive;
	bool mIsDrawable;
	bool mIsPhysicallyDrawable;

	// Shadow Caster
	sf::Drawable const * mShadowCaster;
	b2Body const * mBodyShadowCaster;

	// Positionement dans le plan
	sf::Vector2f mPosition; // haut gauche
	sf::Vector2f mSize;

	// Cells auxquelles le Hull est rataché
	std::list<Cell*> mCells;
};
