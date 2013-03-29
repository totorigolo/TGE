#pragma once
#include "../Tools/Singleton.h"
#include <Thor/Resources.hpp>
#include <SFML/Graphics.hpp>
#include <string>
#include <map>

typedef std::map<std::string, std::shared_ptr<sf::Texture>> TextureMap;

class ResourceManager : public thor::MultiResourceCache, public Singleton<ResourceManager>
{
	friend class Singleton<ResourceManager>;

public:
	// Obtient la TextureMap
	TextureMap &GetTextureMap()
	{
		return mTextureMap;
	}

private:
	TextureMap mTextureMap;
};
