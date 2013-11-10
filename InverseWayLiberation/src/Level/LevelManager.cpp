#include "LevelManager.h"
#include "LevelLoader.h"
#include "../Tools/utils.h"
#include "../Entities/Player.h"
#include "../Game/InputManager.h"
#include "../Game/LuaMachine.h" // TODO
#include "../Physics/PhysicManager.h"
#include "../Entities/EntityManager.h"

// Ctor
LevelManager::LevelManager()
	: mIsCharged(false),
	// Monde
	mPhysicMgr(PhysicManager::GetInstance()),
	// Entities
	mEntityManager(EntityManager::GetInstance()),
	// Config de la fenêtre de rendu
	mBckgC(sf::Color::White),
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
	mResourceManager.Clear();
	mTriggersManager.Clear();
	mPhysicMgr.DestroyAllJoints();
	mEntityManager.DestroyAllEntities();
	mPhysicMgr.DestroyAllBody();
}

// Mise à jour
void LevelManager::Update()
{
	mEntityManager.Update();
	mTriggersManager.Update();
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
	mTriggersManager.DebugDraw(target);
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

// TriggersManager
TriggersManager& LevelManager::GetTriggersMgr()
{
	return mTriggersManager;
}
const TriggersManager& LevelManager::GetTriggersMgr() const
{
	return mTriggersManager;
}

// TODO
void LevelManager::SetLuaConsole(LuaMachine *machine)
{
	mTriggersManager.SetLuaMachine(machine);
}
