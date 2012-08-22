#pragma once
#include "LevelLoader.h"
#include "utils.h"
#include "Physics/World.h"
#include <SFML/Graphics.hpp>
#include <Thor/Resources.hpp>
#include <Box2D/Box2D.h>
#include <string>
#include <map>
#include <utility>
#include <list>

class LevelLoader;
class Level
{
	friend class LevelLoader;

public:
	// Ctor & dtor
	Level(World *world, thor::ResourceCache<sf::Texture> *textureCache, std::map<std::string, std::shared_ptr<sf::Texture>> *textureMap);
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

	std::map<int, std::list<std::pair<int, sf::Sprite*>>> const& GetDeco() const { return mDecoMap; }

private:
	bool mIsValid;
	bool mIsCharged;

	// Monde
	World *mWorld;
	
	// Textures
	thor::ResourceCache<sf::Texture> *mTextureCache;
	std::map<std::string, std::shared_ptr<sf::Texture>> *mTextureMap;

	// Déco (z-index // parallax),  <z-index, sf::Sprite>
	std::map<int, std::list<std::pair<int, sf::Sprite*>>> mDecoMap;
};

