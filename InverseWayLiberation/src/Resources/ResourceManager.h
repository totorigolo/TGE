#pragma once

#include <SFML/Audio.hpp>
#include "../Tools/Singleton.h"
#include "Texture.h"
#include "Font.h"

using FontMap = std::map<std::string, Font::Ptr>;
using SoundMap = std::map<std::string, std::shared_ptr<sf::Sound>>;
using TextureMap = std::map<std::string, Texture::Ptr>;

class ResourceManager : public Singleton<ResourceManager>
{
	friend class Singleton<ResourceManager>;

public:
	// Vide toutes les ressources
	void Clear();

	// Gestion des textures
	bool LoadTexture(const std::string &name, const std::string &path);
	Texture::Ptr GetTexture(const std::string &name);
	bool TextureExists(const std::string &name);

	// Gestion des fonts
	bool LoadFont(const std::string &name, const std::string &path);
	Font::Ptr GetFont(const std::string &name);
	bool FontExists(const std::string &name);

	// Obtient les maps
	const FontMap& GetFontMap() const;
	const SoundMap& GetSoundMap() const;
	const TextureMap& GetTextureMap() const;

private:
	// Conteneurs
	FontMap mFontMap;
	SoundMap mSoundMap;
	TextureMap mTextureMap;
};
