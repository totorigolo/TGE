#include "stdafx.h"
#include "PartitioningTree.h"
#include "Hull.h"

#define INT(x) ((x >= 0.0) ? int(x) : (int(x) - 1)) // TODO: Limits

// Ctor
Cell::Cell(void)
{

}

// Dtor
Cell::~Cell(void)
{
	// Vide la liste des Hull
	mHulls.clear();
}

// Gestion des Hulls
void Cell::AddHull(Hull *hull)
{
	mHulls.push_back(hull);
}
void Cell::RemoveHull(Hull *hull)
{
	mHulls.remove(hull);
}

// Récupération des Hulls
std::list<Hull*>& Cell::GetHulls(void)
{
	return mHulls;
}

// Ctor
PartitioningTree::PartitioningTree(void)
	: mCellSize(1000U) // 10m
{
}

// Dtor
PartitioningTree::~PartitioningTree(void)
{
	// Vide la liste des Cells et les Hulls
	mCells.clear();
	mHulls.clear();
}

// Gestion des éléments
void PartitioningTree::UpdateHull(Hull *hull)
{
	if (!hull->IsValid()) return;

	// Enlève le hull de ses Cells
	hull->RemoveFromCells();

	// Récupère l'index de la position
	int ix = static_cast<int>(floor(hull->GetPosition().x / mCellSize));
	int iy = static_cast<int>(floor(hull->GetPosition().y / mCellSize));

	// Parcours toutes les Cells survollées par le Hull
	int imax = static_cast<int>(floor((hull->GetPosition().x + hull->GetSize().x) / mCellSize)) - ix;
	int jmax = static_cast<int>(floor((hull->GetPosition().y + hull->GetSize().y) / mCellSize)) - iy;
	for (int i = 0; i <= imax; ++i)
	{
		for (int j = 0; j <= jmax; ++j)
		{
			// Ajoute le Hull à la Cell
			Cell* c = &mCells[std::make_pair(ix + i, iy + j)];
			c->AddHull(hull);
			hull->RegisterCell(c);
		}
	}
}
void PartitioningTree::UpdateAll(void)
{
	for (auto &hull : mHulls)
		hull->Update();
}
void PartitioningTree::RegisterHull(Hull *hull)
{
	mHulls.push_back(hull);
	UpdateHull(hull);
}
void PartitioningTree::UnregisterHull(Hull *hull)
{
	hull->RemoveFromCells();
	mHulls.remove(hull);
}

// Recherche
std::list<Hull*> PartitioningTree::GetHulls(const sf::FloatRect &rect)
{
	// Liste de retour
	std::list<Hull*> ret;

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
			Cell* c(&it->second);
			std::copy(c->GetHulls().begin(), c->GetHulls().end(), std::back_inserter(ret));
		}
	}

	return ret;
}
