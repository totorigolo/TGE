#include "stdafx.h"
#include "ResourceManager.h"

// Vide toutes les ressources
void ResourceManager::Clear()
{
	mTextureMap.clear();
	mFontMap.clear();
	mSoundMap.clear();
}

// Gestion des textures
bool ResourceManager::LoadTexture(const std::string &name, const std::string &path)
{
	// Charge la texture
	Texture::Ptr texture(new Texture(name, path));

	// Si la texture n'est pas chargée, on la supprime
	if (!texture->IsLoaded())
	{
		texture.reset();
		return false;
	}

	// Ajoute la texture à la TextureMap
	mTextureMap[name] = std::move(texture);

	return true;
}
Texture::Ptr ResourceManager::GetTexture(const std::string &name)
{
	if (mTextureMap.find(name) == mTextureMap.end())
	{
		Dialog::Error("La texture [" + name + "] n'existe pas.");
		return Texture::Ptr();
	}

	return mTextureMap.find(name)->second;
}
bool ResourceManager::TextureExists(const std::string &name)
{
	return (mTextureMap.find(name) != mTextureMap.end());
}

// Obtient les maps
const FontMap& ResourceManager::GetFontMap()
{
	return mFontMap;
}
const SoundMap& ResourceManager::GetSoundMap()
{
	return mSoundMap;
}
const TextureMap& ResourceManager::GetTextureMap()
{
	return mTextureMap;
}
