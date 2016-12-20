#pragma once

#include <map>
#include <list>
#include <cmath>
#include <SFML/Graphics.hpp>
#include "../Tools/Singleton.h"

class Hull;
class Cell
{
public:
	// Ctor & dtor
	Cell(void);
	~Cell(void);

	// Gestion des Hulls
	void AddHull(Hull *hull);
	void RemoveHull(Hull *hull);

	// Gestion de l'info de déplacement
	bool HasMoved(void) const;
	void Moved(void);
	void PostUpdate(void);

	// Récupération des Hulls
	std::list<Hull*>& GetHulls(void);

private:
	// Etat
	bool mMoved;

	// Liste des Hull
	std::list<Hull*> mHulls;
};

class PartitioningTree : public Singleton<PartitioningTree>
{
protected:
	friend class Singleton<PartitioningTree>;

	// Ctor & dtor
	PartitioningTree(void);
	virtual ~PartitioningTree(void);

public:
	// Gestion des Hull
	void UpdateHull(Hull *hull);
	void PostUpdateAll(void);
	void RegisterHull(Hull *hull);
	void UnregisterHull(Hull *hull);

	// Recherche
	bool HasMovedIn(const sf::FloatRect &rect);
	std::list<Hull*> GetHulls(const sf::FloatRect &rect, bool *moved = nullptr);
	template<typename F>
	void ApplyOnHulls(const sf::FloatRect &rect, F &&func, bool *moved = nullptr)
	{
		// Récupère l'index de la position
		int ix = static_cast<int>(floor(rect.left / mCellSize));
		int iy = static_cast<int>(floor(rect.top / mCellSize));

		// Parcours toutes les Cells survollées par le rect
		int imax = static_cast<int>(floor((rect.left + rect.width) / mCellSize)) - ix;
		int jmax = static_cast<int>(floor((rect.top + rect.height) / mCellSize)) - iy;
		for (int i = 0; i <= imax; ++i)
		{
			for (int j = 0; j <= jmax; ++j)
			{
				// Cherche la cellule actuelle
				auto it = mCells.find(std::make_pair(ix + i, iy + j));
				if (it == mCells.end()) continue;

				// Récupère la Cell
				Cell& c(it->second);

				// Regarde la Cell a bougé
				if (c.HasMoved() && moved)
					*moved = true;

				// Applique la fonction sur chaque Hull
				for (auto &&hull : c.GetHulls())
					func(hull);
			}
		}
	}

	// Debug Draw
	void CreateDebug();
	void DrawDebug(sf::RenderTarget& target) const;

private:
	// Taille des zones et nb de zones
	unsigned int mCellSize;

	// Listes des Cells
	std::map<std::pair<int, int>, Cell> mCells;

	// Liste des Hulls
	std::list<Hull*> mHulls;

	// Debug Draw
	sf::VertexArray mDebugDrawGrid;
};

