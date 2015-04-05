#pragma once

class Cell;
class Hull
{
public:
	// Ctor & dtor
	Hull(b2Body const * bodyShadowCaster);
	Hull(b2Body const * bodyShadowCaster, const sf::Vector2f& pos, const sf::Vector2f& size); // pos : top left
	virtual ~Hull(void);

	// Changement de position / taille
	void SetPosition(const sf::Vector2f& pos); // pos = top left
	void SetSize(const sf::Vector2f& size);
	void SetPosAndSize(const sf::Vector2f& pos, const sf::Vector2f& size);

	// Mise � jour dans le Partitioning System
	void Update(void);
	void PostUpdate(void);

	// Gestion de l'activit� du Hull
	void Activate(void);
	void Deactivate(void);
	void SetActive(bool active);
	bool IsActive(void) const;

	// Gestion des Cells
	void RegisterCell(Cell *cell);
	void UnregisterCell(Cell *cell);
	std::list<Cell*>& GetRegisteredCells(void);

	// S'enl�ve de toutes les Cells
	void RemoveFromCells(void);

	// Gestion du Shadow Caster
	void SetPhysicallyDrawable(bool physicallyDrawable);
	bool IsPhysicallyDrawable(void) const;
	void SetBodyShadowCaster(b2Body const * bodyShadowCaster);
	b2Body const* GetBodyShadowCaster() const;

	// Gestion de l'info de d�placement
	bool HasMoved(void) const;
	void Moved(void);

	// Accesseurs
	sf::Vector2f GetPosition(void) const;
	sf::Vector2f GetSize(void) const;
	bool IsValid(void) const;

private:
	// Propri�t�s
	bool mMoved;
	bool mIsValid;
	bool mIsActive;
	bool mIsPhysicallyDrawable;

	// Shadow Caster
	b2Body const * mBodyShadowCaster;

	// Positionement dans le plan
	sf::Vector2f mPosition; // haut gauche
	sf::Vector2f mSize;

	// Cells auxquelles le Hull est ratach�
	std::list<Cell*> mCells;
};
