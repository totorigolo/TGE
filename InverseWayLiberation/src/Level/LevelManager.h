#pragma once
#include <Thor/Resources.hpp>
#include <SFML/Graphics.hpp>
#include <Box2D/Box2D.h>
#include <string>
#include "TriggersManager.h"
#include "../Tools/Singleton.h"
#include "../Resources/ResourceManager.h"

class Player;
class LevelLoader;
class EntityManager;
class PhysicManager;
class LuaMachine; // TODO:
class LevelManager : public sf::Drawable, public Singleton<LevelManager>
{
	friend class LevelLoader;

protected:
	friend class Singleton<LevelManager>;

	// Ctor & dtor
	LevelManager();
	virtual ~LevelManager(void);

public:
	// Charge un niveau à partir d'un XVL
	void LoadFromFile(const std::string &path);

	// Vide tout le niveau
	void Clear();

	// Mise à jour
	void Update();
	
	// Appelé juste avant la boucle de jeu, après son remplissage
	void PrepareForGame();
	
	// Pour le rendu
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

	/* Accesseurs */
	// Etat
	bool IsCharged() const;
	void SetCharged(bool charged);

	// Player
	void SetPlayer(Player *player);
	Player* GetPlayer();

	// Couleur de fond
	void SetBckgColor(const sf::Color &color);
	sf::Color const& GetBckgColor() const;

	// TODO
	void SetLuaConsole(LuaMachine *machine);

private:
	bool mIsCharged;

	// Monde
	b2Vec2 mGravity;
	PhysicManager &mPhysicMgr;

	// Entities
	EntityManager &mEntityManager;

	// Triggers
	TriggersManager mTriggersManager;

	// Config de la fenêtre de rendu
	sf::Color mBckgC;

	// Textures
	ResourceManager &mResourceManager;
	TextureMap &mTextureMap;

	// Joueur
	Player *mPlayer;
};

