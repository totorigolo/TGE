#pragma once
#include <SFML/Graphics.hpp>
#include <Thor/Resources.hpp>
#include <Box2D/Box2D.h>
#include <string>
#include <map>
#include <utility>
#include <list>
#include "LevelLoader.h"
#include "utils.h"
#include "Physics/World.h"
#include "Resources/ResourceManager.h"

class LevelLoader;
class Level
{
	friend class LevelLoader;

public:
	// Ctor & dtor
	Level(World *world);
	virtual ~Level(void);

	// Vide tout le niveau
	void Clear();

	// Gestion de la déco
	void DestroyDecoLevel(int zindex);
	void AddDeco(int level_zindex, sf::Sprite *sprite, int zindex);
	void DestroyDeco(int level_zindex, sf::Sprite *sprite, int zindex);
	void AddDeco(int level_zindex, b2Vec3 posRot, std::string const& texture, int zindex);
	void DestroyDeco(int level_zindex, b2Vec3 posRot, std::string const& texture, int zindex);
	void DestroyAllDeco();

	void SortDecoByzIndex();

	// Accesseurs
	bool IsValid() const { return mIsValid; }

	bool IsCharged() const { return mIsValid; }
	void SetCharged(bool charged) { mIsCharged = charged; }
	
	float GetDefaultZoom() const { return mDefaulfZoom; }
	sf::Color const& GetBckgColor() const { return mBckgC; }
	b2Vec2 const& GetOriginView() const { return mOriginView; }

	std::map<int, std::list<std::pair<int, sf::Sprite*>>> const& GetDeco() const { return mDecoMap; }

private:
	bool mIsValid;
	bool mIsCharged;

	// Monde
	World *mWorld;

	// Config de la fenêtre de rendu
	sf::Color mBckgC;
	float mDefaulfZoom;
	b2Vec2 mOriginView;
	
	// Textures
	ResourceManager &mResourceManager;
	TextureMap &mTextureMap;

	// Déco (z-index // parallax),  <z-index, sf::Sprite>
	std::map<int, std::list<std::pair<int, sf::Sprite*>>> mDecoMap;
};

