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
#include "../Physics/World.h"
#include "../Entities/EntityManager.h"
#include "../Resources/ResourceManager.h"

class LevelLoader;
class Level : public sf::Drawable
{
	friend class LevelLoader;

public:
	// Ctor & dtor
	Level(World *world);
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
	bool IsValid() const { return mIsValid; }

	bool IsCharged() const { return mIsValid; }
	void SetCharged(bool charged) { mIsCharged = charged; }
	
	EntityManager* GetEntityManager() { return &mEntityManager; }
	const EntityManager* GetEntityManager() const { return &mEntityManager; }

	float GetDefaultZoom() const { return mDefaulfZoom; }
	sf::Color const& GetBckgColor() const { return mBckgC; }
	b2Vec2 const& GetOriginView() const { return mOriginView; }
	bool const& GetLightning() const { return mLightning; }

private:
	bool mIsValid;
	bool mIsCharged;

	// Monde
	World *mWorld;

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

	// TODO: Virer ça
public:
	Entity *mPlayer;
};

