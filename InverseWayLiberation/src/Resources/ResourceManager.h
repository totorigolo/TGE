#pragma once
#include "Texture.h"


typedef std::map<std::string, std::shared_ptr<sf::Font>> FontMap;
typedef std::map<std::string, std::shared_ptr<sf::Sound>> SoundMap;
typedef std::map<std::string, std::shared_ptr<Texture>> TextureMap;

class ResourceManager : public Singleton<ResourceManager>
{
	friend class Singleton<ResourceManager>;

public:
	// Vide toutes les ressources
	void Clear();

	// Gestion des textures
	bool LoadTexture(const std::string &name, const std::string &path);
	std::shared_ptr<Texture> GetTexture(const std::string &name);
	bool TextureExists(const std::string &name);

	// Obtient les maps
	const FontMap& GetFontMap();
	const SoundMap& GetSoundMap();
	const TextureMap& GetTextureMap();

private:
	// Conteneurs
	FontMap mFontMap;
	SoundMap mSoundMap;
	TextureMap mTextureMap;
};
