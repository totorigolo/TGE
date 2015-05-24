#include "stdafx.h"
#include "ResourceManager.h"

// Vide toutes les ressources
void ResourceManager::Clear()
{
	mTextureMap.clear();
	//mFontMap.clear();
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

// Gestion des fonts
bool ResourceManager::LoadFont(const std::string &name, const std::string &path)
{
	// Regarde si la resource est déjà chargée
	auto it = mFontMap.find(name);
	if (it != mFontMap.end())
	{
		// C'est la même
		if (it->second->GetPath() == path)
		{
			// Chargée, on ne fait rien
			if (it->second->IsLoaded())
				return true;

			// Non chargée, on la re-charge dans la même Font (limitation de compatibilité avec la SFML
			else
			{
				myCheckError(it->second->loadFromFile(path), "Impossible de charger :\n" + path);
				return true;
			}
		}
	}

	// Charge la font
	Font::Ptr font(new Font(name, path));

	// Si la texture n'est pas chargée, on la supprime
	if (!font->IsLoaded())
	{
		font.reset();
		std::cout << "Police \"" << name << "\" (" << path << ") non chargee." << std::endl;
		return false;
	}

	// Ajoute la texture à la TextureMap
	mFontMap[name] = std::move(font);

	return true;
}
Font::Ptr ResourceManager::GetFont(const std::string &name)
{
	if (mFontMap.find(name) == mFontMap.end())
	{
		Dialog::Error("La font [" + name + "] n'existe pas.");
		return Font::Ptr();
	}

	return mFontMap.find(name)->second;
}
bool ResourceManager::FontExists(const std::string &name)
{
	return (mFontMap.find(name) != mFontMap.end());
}

// Obtient les maps
const FontMap& ResourceManager::GetFontMap() const
{
	return mFontMap;
}
const SoundMap& ResourceManager::GetSoundMap() const
{
	return mSoundMap;
}
const TextureMap& ResourceManager::GetTextureMap() const
{
	return mTextureMap;
}
