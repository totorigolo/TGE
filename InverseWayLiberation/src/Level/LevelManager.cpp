#include "stdafx.h"
#include "LevelManager.h"
#include "LevelLoader.h"
#include "../Lua/LuaMachine.h"
#include "../Entities/Player.h"
#include "../App/InputManager.h"
#include "../Lua/TriggersManager.h"
#include "../Physics/PhysicManager.h"
#include "../Entities/EntityManager.h"
#include "../Entities/PartitioningTree.h"

// Ctor
LevelManager::LevelManager()
	: mIsCharged(false),
	// Monde
	mPhysicMgr(PhysicManager::GetInstance()),
	// Entities
	mEntityManager(EntityManager::GetInstance()),
	mPartitioningTree(PartitioningTree::GetInstance()),
	// Triggers
	mTriggersManager(TriggersManager::GetInstance()),
	// Config de la fenêtre de rendu
	mBckgC(sf::Color::White),
	// Zoom initial
	mDefaultZoom(1.f),
	// Textures
	mResourceManager(ResourceManager::GetInstance()),
	// Joueur
	mPlayer(nullptr)
{
}

// Dtor
LevelManager::~LevelManager(void)
{
	Clear();

	mIsCharged = false;
}

// Charge un niveau à partir d'un XVL
void LevelManager::LoadFromFile(const std::string &path)
{
	// c_str : workaround pour problème de compilation
	LevelLoader(path.c_str());
}

// Vide tout le niveau
void LevelManager::Clear()
{
	// Vide tous les objets du jeu
	mEntityManager.DestroyAllEntities();
	mResourceManager.Clear();
	mTriggersManager.Clear();
	mPhysicMgr.DestroyAllJoints();
	mPhysicMgr.DestroyAllBody();

	// Vide tout
	mIsCharged = false;
	mPlayer = nullptr;
	mGravity = b2Vec2_zero;
	mBckgC = sf::Color::White;
	mDefaultZoom = 1.f;
	mDefaultCenter = sf::Vector2f();
}

// Mise à jour
void LevelManager::Update()
{
	mEntityManager.Update();
	mTriggersManager.Update();
	mPartitioningTree.PostUpdateAll();
}

// Appelé juste avant la boucle de jeu, après son remplissage
void LevelManager::PrepareForGame()
{
	// Trie les Entities par leur Layer
	mEntityManager.SortByLayer();
}

// Pour le rendu
void LevelManager::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	// Affiche les Entities
	target.draw(mEntityManager, states);
}

// Réglages de la vue initiale
void LevelManager::SetDefaultZoom(float zoom)
{
	mDefaultZoom = zoom;
	InputManager::GetInstance().SetZoom(mDefaultZoom);
}
void LevelManager::SetDefaultCenter(const sf::Vector2f &center)
{
	mDefaultCenter = center;
	InputManager::GetInstance().SetCenter(mDefaultCenter);
}
float LevelManager::GetDefaultZoom() const
{
	return mDefaultZoom;
}
const sf::Vector2f& LevelManager::GetDefaultCenter() const
{
	return mDefaultCenter;
}

/* Accesseurs */
// Etat
bool LevelManager::IsCharged() const
{
	return mIsCharged;
}
void LevelManager::SetCharged(bool charged)
{
	mIsCharged = charged;
}

// Player
void LevelManager::SetPlayer(Player *player)
{
	mPlayer = player;
}
Player* LevelManager::GetPlayer()
{
	return mPlayer;
}

// Couleur de fond
void LevelManager::SetBckgColor(const sf::Color &color)
{
	mBckgC = color;
}
sf::Color const& LevelManager::GetBckgColor() const
{
	return mBckgC;
}

// TODO
void LevelManager::SetLuaConsole(LuaMachine *machine)
{
	mTriggersManager.SetLuaMachine(machine);
}
