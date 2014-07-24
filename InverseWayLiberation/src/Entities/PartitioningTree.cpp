#include "stdafx.h"
#include "PartitioningTree.h"
#include "Hull.h"

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
	Moved();
	mHulls.push_back(hull);
}
void Cell::RemoveHull(Hull *hull)
{
	Moved();
	mHulls.remove(hull);
}

// Gestion de l'info de d�placement
bool Cell::HasMoved(void) const
{
	return mMoved;
}
void Cell::Moved(void)
{
	mMoved = true;
}
void Cell::PostUpdate(void)
{
	mMoved = false;
}

// R�cup�ration des Hulls
std::list<Hull*>& Cell::GetHulls(void)
{
	return mHulls;
}

// Ctor
PartitioningTree::PartitioningTree(void)
	: mCellSize(600U) // 6m
{
}

// Dtor
PartitioningTree::~PartitioningTree(void)
{
	// Vide la liste des Cells et les Hulls
	mCells.clear();
	mHulls.clear();
}

// Gestion des �l�ments
void PartitioningTree::UpdateHull(Hull *hull)
{
	if (!hull->IsValid()) return;

	// Enl�ve le hull de ses Cells en les pr�venant qu'on s'en va
	hull->Moved();
	hull->RemoveFromCells();

	// R�cup�re l'index de la position
	int ix = static_cast<int>(floor(hull->GetPosition().x / mCellSize));
	int iy = static_cast<int>(floor(hull->GetPosition().y / mCellSize));

	// Parcours toutes les Cells survoll�es par le Hull
	int imax = static_cast<int>(floor((hull->GetPosition().x + hull->GetSize().x) / mCellSize)) - ix;
	int jmax = static_cast<int>(floor((hull->GetPosition().y + hull->GetSize().y) / mCellSize)) - iy;
	for (int i = 0; i <= imax; ++i)
	{
		for (int j = 0; j <= jmax; ++j)
		{
			// Ajoute le Hull � la Cell
			Cell* c = &mCells[std::make_pair(ix + i, iy + j)];
			c->AddHull(hull);
			hull->RegisterCell(c);
		}
	}
}
void PartitioningTree::PostUpdateAll(void)
{
	for (auto &hull : mHulls)
		hull->PostUpdate();
	for (auto &cell : mCells)
		cell.second.PostUpdate();
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
bool PartitioningTree::HasMovedIn(const sf::FloatRect &rect)
{
	// Retour
	bool moved = false;

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
			if (c.HasMoved())
				moved = true;
		}
	}

	return moved;
}
std::list<Hull*> PartitioningTree::GetHulls(const sf::FloatRect &rect, bool *moved)
{
	// Liste de retour
	std::list<Hull*> ret;

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

			// R�cup�re la Cell et copie les Hulls
			Cell& c(it->second);
			std::copy(c.GetHulls().begin(), c.GetHulls().end(), std::back_inserter(ret));

			// Regarde la Cell a boug�
			if (c.HasMoved() && moved)
				*moved = true;
		}
	}

	return ret;
}
