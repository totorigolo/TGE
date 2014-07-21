#include "stdafx.h"
#include "Hull.h"
#include "PartitioningTree.h"

// Ctor
Hull::Hull(sf::Drawable const * shadowCaster)
	: mIsValid(false), mIsActive(true), mIsDrawable(false), mShadowCaster(shadowCaster), mIsPhysicallyDrawable(false), mBodyShadowCaster(nullptr)
{
	PartitioningTree::GetInstance().RegisterHull(this);
}
Hull::Hull(sf::Drawable const * shadowCaster, const sf::Vector2f& pos, const sf::Vector2f& size)
	: mPosition(pos), mSize(size), mIsValid(true), mIsActive(true), mShadowCaster(shadowCaster), mBodyShadowCaster(nullptr)
{
	PartitioningTree::GetInstance().RegisterHull(this);
}

// Dtor
Hull::~Hull(void)
{
	RemoveFromCells();
	PartitioningTree::GetInstance().UnregisterHull(this);
}

// Changement de position / taille
void Hull::SetPosition(const sf::Vector2f& pos)
{
	if (pos != mPosition)
	{
		mPosition = pos;

		if (mSize.x != 0 && mSize.y != 0)
			mIsValid = true;

		Update();
	}
}
void Hull::SetSize(const sf::Vector2f& size)
{
	if (size != mSize)
	{
		mSize = size;
		mIsValid = true;
		Update();
	}
}
void Hull::SetPosAndSize(const sf::Vector2f& pos, const sf::Vector2f& size)
{
	if (pos != mPosition || size != mSize)
	{
		mPosition = pos;
		mSize = size;
		mIsValid = true;
		Update();
	}
}

// Mise à jour dans le Partitioning System
void Hull::Update(void)
{
	if (mIsValid && mIsActive)
		PartitioningTree::GetInstance().UpdateHull(this);
}

// Gestion de l'activité du Hull
void Hull::Activate(void)
{
	mIsActive = true;
	Update();
}
void Hull::Deactivate(void)
{
	mIsActive = false;
	PartitioningTree::GetInstance().UnregisterHull(this);
}
void Hull::SetActive(bool active)
{
	if (active)
		Activate();
	else
		Deactivate();
}
bool Hull::IsActive(void) const
{
	return mIsActive;
}

// Gestion des Cells
void Hull::RegisterCell(Cell *cell)
{
	mCells.push_back(cell);
}
void Hull::UnregisterCell(Cell *cell)
{
	mCells.remove(cell);
}
std::list<Cell*>& Hull::GetRegisteredCells(void)
{
	return mCells;
}

// S'enlève de toutes les Cells
void Hull::RemoveFromCells(void)
{
	for (auto it = mCells.begin(); it != mCells.end(); ++it)
		(*it)->RemoveHull(this);
	mCells.clear();
}

// Gestion du Shadow Caster
void Hull::SetDrawable(bool drawable)
{
	mIsDrawable = drawable;
	if (mIsDrawable)
		mIsPhysicallyDrawable = false;
}
bool Hull::IsDrawable(void) const
{
	return mIsDrawable;
}
void Hull::SetPhysicallyDrawable(bool physicallyDrawable)
{
	mIsPhysicallyDrawable = physicallyDrawable;
	if (mIsPhysicallyDrawable)
		mIsDrawable = false;
}
bool Hull::IsPhysicallyDrawable(void) const
{
	return mIsPhysicallyDrawable;
}
void Hull::SetShadowCaster(sf::Drawable const * shadowCaster)
{
	mShadowCaster = shadowCaster;
}
void Hull::SetBodyShadowCaster(b2Body const * bodyShadowCaster)
{
	mBodyShadowCaster = bodyShadowCaster;
}
sf::Drawable const* Hull::GetShadowCaster() const
{
	return mShadowCaster;
}
b2Body const* Hull::GetBodyShadowCaster() const
{
	return mBodyShadowCaster;
}

// Accesseurs
sf::Vector2f Hull::GetPosition(void) const
{
	return mPosition;
}
sf::Vector2f Hull::GetSize(void) const
{
	return mSize;
}
bool Hull::IsValid(void) const
{
	return mIsValid;
}
