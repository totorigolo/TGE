#include "Level.h"

// Ctor
Level::Level(PhysicManager *physicMgr)
	: mIsCharged(false),
	// Monde
	mPhysicMgr(physicMgr),
	// Entities
	mEntityManager(EntityManager::GetInstance()),
	// Config de la fenêtre de rendu
	mBckgC(sf::Color::White),
	mDefaulfZoom(1.f),
	mOriginView(b2Vec2_zero),
	// Textures
	mResourceManager(ResourceManager::GetInstance()),
	mTextureMap(mResourceManager.GetTextureMap()),
	// Joueur
	mPlayer(nullptr)
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
