#include "Level.h"

// Ctor
Level::Level(PhysicManager *physicMgr)
	: mIsCharged(false),
	mPhysicMgr(physicMgr),
	mBckgC(255, 255, 255),
	mResourceManager(ResourceManager::GetInstance()),
	mTextureMap(mResourceManager.GetTextureMap()),
	mEntityManager(EntityManager::GetInstance())
{
	assert(mPhysicMgr && "n'est pas valide.");
}

// Dtor
Level::~Level(void)
{
	Clear();

	mIsCharged = false;
}

// Vide tout le niveau
void Level::Clear()
{
	// Vide tous les objets du jeu
	// TODO: Vider les textures ? (-> faire un système pour ne vider que les textures inutilisées)
	mPhysicMgr->DestroyAllJoints();
	mEntityManager.DestroyAllEntities();
	mPhysicMgr->DestroyAllBody();
}

// Mise à jour
void Level::Update()
{
	mEntityManager.Update();
}

// Appelé juste avant la boucle de jeu, après son remplissage
void Level::PrepareForGame()
{
	// Trie les Entities par leur Layer
	mEntityManager.SortByLayer();
}

// Pour le rendu
void Level::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	// Affiche les Entities
	target.draw(mEntityManager, states);
}
