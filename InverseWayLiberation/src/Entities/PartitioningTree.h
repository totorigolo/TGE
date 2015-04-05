#pragma once

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

	// Gestion de l'info de d�placement
	bool HasMoved(void) const;
	void Moved(void);
	void PostUpdate(void);

	// R�cup�ration des Hulls
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
		// R�cup�re l'index de la position
		int ix = static_cast<int>(floor(rect.left / mCellSize));
		int iy = static_cast<int>(floor(rect.top / mCellSize));

		// Parcours toutes les Cells survoll�es par le rect
		int imax = static_cast<int>(floor((rect.left + rect.width) / mCellSize)) - ix;
		int jmax = static_cast<int>(floor((rect.top + rect.height) / mCellSize)) - iy;
		for (int i = 0; i <= imax; ++i)
		{
			for (int j = 0; j <= jmax; ++j)
			{
				// Cherche la cellule actuelle
				auto it = mCells.find(std::make_pair(ix + i, iy + j));
				if (it == mCells.end()) continue;

				// R�cup�re la Cell
				Cell& c(it->second);

				// Regarde la Cell a boug�
				if (c.HasMoved() && moved)
					*moved = true;

				// Applique la fonction sur chaque Hull
				for each (auto &hull in c.GetHulls())
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

