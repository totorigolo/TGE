#pragma once
#include "../Tools/Singleton.h"
#include "../Tools/Error.h"
#include <Thor/Resources.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <memory>
#include <string>
#include <list>
#include <map>

typedef std::map<std::string, std::shared_ptr<sf::Font>> FontMap;
typedef std::map<std::string, std::shared_ptr<sf::Sound>> SoundMap;
typedef std::map<std::string, std::shared_ptr<sf::Texture>> TextureMap;

class ResourceManager : public thor::MultiResourceCache, public Singleton<ResourceManager>
{
	friend class Singleton<ResourceManager>;

public:
	// Obtient la FontMap
	FontMap& GetFontMap()
	{
		return mFontMap;
	}

	// Obtient la SoundMap
	SoundMap& GetSoundMap()
	{
		return mSoundMap;
	}

	// Obtient la TextureMap
	TextureMap& GetTextureMap()
	{
		return mTextureMap;
	}

private:
	// Conteneurs
	FontMap mFontMap;
	SoundMap mSoundMap;
	TextureMap mTextureMap;
};
