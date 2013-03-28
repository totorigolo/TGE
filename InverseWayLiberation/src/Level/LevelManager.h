#pragma once
#include <SFML/Graphics.hpp>
#include <Thor/Resources.hpp>
#include <Box2D/Box2D.h>
#include <string>
#include "../Tools/utils.h"
#include "../Entities/Player.h"
#include "../Tools/Singleton.h"
#include "../Physics/PhysicManager.h"
#include "../Entities/EntityManager.h"
#include "../Resources/ResourceManager.h"

class LevelLoader;
class LevelManager : public sf::Drawable, public Singleton<LevelManager>
{
	friend class LevelLoader;

public:
	// Ctor & dtor
	LevelManager();
	virtual ~LevelManager(void);

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

	// Zoom par défaut & zoom
	void SetZoom(float zoom);
	void SetDefaultZoom(float zoom);
	float GetDefaultZoom() const;

	// Couleur de fond
	void SetBckgColor(const sf::Color &color);
	sf::Color const& GetBckgColor() const;

	// Position de la vue
	void SetViewPosition(const b2Vec2 &pos);
	void SetOriginView(const b2Vec2 &origin);
	b2Vec2 const& GetOriginView() const;

private:
	bool mIsCharged;

	// Monde
	b2Vec2 mGravity;
	PhysicManager &mPhysicMgr;

	// Entities
	EntityManager &mEntityManager;

	// Config de la fenêtre de rendu
	sf::Color mBckgC;
	float mDefaulfZoom;
	b2Vec2 mOriginView;
	
	// Textures
	ResourceManager &mResourceManager;
	TextureMap &mTextureMap;

	// Joueur
	Player *mPlayer;
};

