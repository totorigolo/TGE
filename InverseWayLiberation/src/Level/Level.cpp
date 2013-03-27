#include "Level.h"
#include "LevelLoader.h"
#include "../Game/InputManager.h"
#include "../Tools/utils.h"

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

// Charge un niveau à partir d'un XVL
void Level::LoadFromFile(const std::string &path)
{
	LevelLoader(path, this);
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

/* Accesseurs */
// Etat
bool Level::IsCharged() const
{
	return mIsCharged;
}
void Level::SetCharged(bool charged)
{
	mIsCharged = charged;
}

// PPM
void Level::SetPPM(float ppm)
{
	if (mPhysicMgr)
		mPhysicMgr->SetPPM(ppm);
}
const float Level::GetPPM() const
{
	return (mPhysicMgr) ? mPhysicMgr->GetPPM() : 0.f;
}

// Gravité
void Level::SetGravity(const b2Vec2 &gravity)
{
	mGravity = gravity;
	if (mPhysicMgr)
		mPhysicMgr->SetGravity(mGravity);
}
const b2Vec2& Level::GetGravity() const
{
	return mGravity;
}

// Player
void Level::SetPlayer(Player *player)
{
	mPlayer = player;
}
Player* Level::GetPlayer()
{
	return mPlayer;
}

// EntityManager
EntityManager* Level::GetEntityManager()
{
	return &mEntityManager;
}
const EntityManager* Level::GetEntityManager() const
{
	return &mEntityManager;
}

// Zoom par défaut & zoom
void Level::SetZoom(float zoom)
{
	InputManager::GetInstance().SetZoom(zoom);
}
void Level::SetDefaultZoom(float zoom)
{
	mDefaulfZoom = zoom;
	InputManager::GetInstance().SetDefaultZoom(mDefaulfZoom);
}
float Level::GetDefaultZoom() const
{
	return mDefaulfZoom;
}

// Couleur de fond
void Level::SetBckgColor(const sf::Color &color)
{
	mBckgC = color;
}
sf::Color const& Level::GetBckgColor() const
{
	return mBckgC;
}

// Position de la vue
void Level::SetViewPosition(const b2Vec2 &pos)
{
	if (mPhysicMgr)
		InputManager::GetInstance().SetCenter(b22sfVec(pos, GetPPM()));
}
void Level::SetOriginView(const b2Vec2 &origin)
{
	mOriginView = origin;
	if (mPhysicMgr)
		InputManager::GetInstance().SetCenter(b22sfVec(mOriginView, GetPPM()));
}
b2Vec2 const& Level::GetOriginView() const
{
	return mOriginView;
}
