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

	// Récupération des Hulls
	std::list<Hull*>& GetHulls(void);

private:
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
	void UpdateAll(void);
	void RegisterHull(Hull *hull);
	void UnregisterHull(Hull *hull);

	// Recherche
	std::list<Hull*> GetHulls(const sf::FloatRect &rect);
	template<typename F>
	void ApplyOnHulls(const sf::FloatRect &rect, F &&func)
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

				// Récupère la Cell et copie les Hulls
				Cell& c(it->second);

				// Applique la fonction sur chaque Hull
				for each (auto hull in c.GetHulls())
					func(hull);
			}
		}
	}

private:
	// Taille des zones et nb de zones
	unsigned int mCellSize;

	// Listes des Cells
	std::map<std::pair<int, int>, Cell> mCells;

	// Liste des Hulls
	std::list<Hull*> mHulls;
};

