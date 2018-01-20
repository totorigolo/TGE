#include "Hull.h"
#include "PartitioningTree.h"

// Ctor
Hull::Hull(b2Body const *bodyShadowCaster)
        : mMoved(true),
          mIsValid(false),
          mIsActive(true),
          mIsPhysicallyDrawable(false),
          mBodyShadowCaster(bodyShadowCaster) {
    // On vient d'apparaitre, donc on a bougé
    Moved();

    // Enregistre le hull
    if (mBodyShadowCaster)
        PartitioningTree::GetInstance().RegisterHull(this);
}

Hull::Hull(b2Body const *bodyShadowCaster, const sf::Vector2f &pos, const sf::Vector2f &size)
        : mMoved(true),
          mIsValid(true),
          mIsActive(true),
          mIsPhysicallyDrawable(false),
          mBodyShadowCaster(bodyShadowCaster),
          mPosition(pos),
          mSize(size) {
    Moved();
    if (mBodyShadowCaster)
        PartitioningTree::GetInstance().RegisterHull(this);
}

// Dtor
Hull::~Hull(void) {
    RemoveFromCells();
    PartitioningTree::GetInstance().UnregisterHull(this);
}

// Changement de position / taille
void Hull::SetPosition(const sf::Vector2f &pos) {
    Moved();

    if (pos != mPosition) {
        mPosition = pos;

        if (mSize.x != 0 && mSize.y != 0)
            mIsValid = true;

        Update();
    }
}

void Hull::SetSize(const sf::Vector2f &size) {
    Moved();

    if (size != mSize) {
        mSize = size;
        mIsValid = true;
        Update();
    }
}

void Hull::SetPosAndSize(const sf::Vector2f &pos, const sf::Vector2f &size) {
    Moved();

    if (pos != mPosition || size != mSize) {
        mPosition = pos;
        mSize = size;
        mIsValid = true;
        Update();
    }
}

// Mise à jour dans le Partitioning System
void Hull::Update(void) {
    if (mIsValid && mIsActive && mMoved && mBodyShadowCaster)
        PartitioningTree::GetInstance().UpdateHull(this);
}

void Hull::PostUpdate(void) {
    if (mIsValid && mIsActive)
        mMoved = false;
}

// Gestion de l'activité du Hull
void Hull::Activate(void) {
    Moved();
    if (mBodyShadowCaster && !mIsActive)
        PartitioningTree::GetInstance().RegisterHull(this);
    mIsActive = true;
}

void Hull::Deactivate(void) {
    Moved();
    mIsActive = false;
    PartitioningTree::GetInstance().UnregisterHull(this);
}

void Hull::SetActive(bool active) {
    if (active)
        Activate();
    else
        Deactivate();
}

bool Hull::IsActive(void) const {
    return mIsActive;
}

// Gestion des Cells
void Hull::RegisterCell(Cell *cell) {
    mCells.push_back(cell);
}

void Hull::UnregisterCell(Cell *cell) {
    mCells.remove(cell);
}

std::list<Cell *> &Hull::GetRegisteredCells(void) {
    return mCells;
}

// S'enlève de toutes les Cells
void Hull::RemoveFromCells(void) {
    for (auto it = mCells.begin(); it != mCells.end(); ++it)
        (*it)->RemoveHull(this);
    mCells.clear();
}

// Gestion du Shadow Caster
void Hull::SetPhysicallyDrawable(bool physicallyDrawable) {
    if (mBodyShadowCaster)
        mIsPhysicallyDrawable = physicallyDrawable;
    else {
        mIsPhysicallyDrawable = false;
        PartitioningTree::GetInstance().UnregisterHull(this);
    }
    //if (mIsPhysicallyDrawable)
    //	mIsDrawable = false;
}

bool Hull::IsPhysicallyDrawable(void) const {
    return mIsPhysicallyDrawable;
}

void Hull::SetBodyShadowCaster(b2Body const *bodyShadowCaster) {
    mBodyShadowCaster = bodyShadowCaster;
}

b2Body const *Hull::GetBodyShadowCaster() const {
    return mBodyShadowCaster;
}

// Accesseurs
sf::Vector2f Hull::GetPosition(void) const {
    return mPosition;
}

sf::Vector2f Hull::GetSize(void) const {
    return mSize;
}

bool Hull::IsValid(void) const {
    return mIsValid;
}

bool Hull::HasMoved(void) const {
    return mMoved;
}

void Hull::Moved(void) {
    // Retient que l'on a bougé
    mMoved = true;

    // Informe toutes les Cells survolées qu'on a bougé
    for (auto &&cell : mCells)
        cell->Moved();
}
