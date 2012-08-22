#pragma once
#include "LevelLoader.h"
#include "Physics/World.h"
#include <SFML/Graphics.hpp>
#include <Thor/Resources.hpp>
#include <string>
#include <map>

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

	// Accesseurs
	bool IsValid() const { return mIsValid; }

	bool IsCharged() const { return mIsValid; }
	void SetCharged(bool charged) { mIsCharged = charged; }

private:
	bool mIsValid;
	bool mIsCharged;

	// Monde
	World *mWorld;
	
	// Textures
	thor::ResourceCache<sf::Texture> *mTextureCache;
	std::map<std::string, std::shared_ptr<sf::Texture>> *mTextureMap;
};

