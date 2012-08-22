#include "Level.h"

// Ctor
Level::Level(World *world, thor::ResourceCache<sf::Texture> *textureCache, std::map<std::string, std::shared_ptr<sf::Texture>> *textureMap)
	: mIsValid(false), mIsCharged(false), mWorld(world), mTextureCache(textureCache), mTextureMap(textureMap)
{
	if (mWorld && mTextureCache && mTextureMap)
	{
		mIsValid = true;
	}
}

// Dtor
Level::~Level(void)
{
	mIsValid = false;
	mIsCharged = false;
}

// Vide tout le niveau
void Level::Clear()
{
	// TODO: Comment faire pour ne pas vider tout le monde ? (pas grave)
	mWorld->DestroyAllJoints();
	mWorld->DestroyAllBody();
}
