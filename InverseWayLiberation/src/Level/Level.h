#pragma once
#include <SFML/Graphics.hpp>
#include <Thor/Resources.hpp>
#include <Box2D/Box2D.h>
#include <string>
#include <map>
#include <utility>
#include <list>
#include "LevelLoader.h"
#include "../Tools/utils.h"
#include "../Entities/Player.h"
#include "../Physics/PhysicManager.h"
#include "../Entities/EntityManager.h"
#include "../Resources/ResourceManager.h"

class LevelLoader;
class Level : public sf::Drawable
{
	friend class LevelLoader;

public:
	// Ctor & dtor
	Level(PhysicManager *physicMgr);
	virtual ~Level(void);

	// Vide tout le niveau
	void Clear();

	// Mise à jour
	void Update();
	
	// Appelé juste avant la boucle de jeu, après son remplissage
	void PrepareForGame();
	
	// Pour le rendu
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

	// Accesseurs
	bool IsCharged() const { return mIsCharged; }
	void SetCharged(bool charged) { mIsCharged = charged; }
	
	Player* GetPlayer() { return mPlayer; }
	const Player* GetPlayer() const { return mPlayer; }

	EntityManager* GetEntityManager() { return &mEntityManager; }
	const EntityManager* GetEntityManager() const { return &mEntityManager; }

	float GetDefaultZoom() const { return mDefaulfZoom; }
	sf::Color const& GetBckgColor() const { return mBckgC; }
	b2Vec2 const& GetOriginView() const { return mOriginView; }
	bool const& GetLightning() const { return mLightning; }

private:
	bool mIsCharged;

	// Monde
	PhysicManager *mPhysicMgr;

	// Entities
	EntityManager &mEntityManager;

	// Config de la fenêtre de rendu
	sf::Color mBckgC;
	float mDefaulfZoom;
	b2Vec2 mOriginView;
	bool mLightning;
	
	// Textures
	ResourceManager &mResourceManager;
	TextureMap &mTextureMap;

	// Joueur
	Player *mPlayer;
};

