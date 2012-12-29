#include "Level.h"
#include "../Tools/utils.h"

// Ctor
Level::Level(World *world)
	: mIsValid(false), mIsCharged(false), mWorld(world),  mBckgC(255, 255, 255), mLightning(false),
	mResourceManager(ResourceManager::GetInstance()), mTextureMap(mResourceManager.GetTextureMap()),
	mEntityManager(EntityManager::GetInstance())
{
	if (mWorld)
	{
		mIsValid = true;
	}
}

// Dtor
Level::~Level(void)
{
	Clear();

	mIsValid = false;
	mIsCharged = false;
}

// Vide tout le niveau
void Level::Clear()
{
	// Vide tous les objets du jeu
	// TODO: Vider les textures ? (-> faire un système pour ne vider que les textures inutilisées)
	mWorld->DestroyAllJoints();
	mWorld->DestroyAllBody();
	mEntityManager.DestroyAllEntities();
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
