#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include "../Tools/Error.h"

class Texture : public sf::Texture
{
public:
	Texture(const std::string &name, const std::string &path)
		: mName(name), mPath(path)
	{
		myCheckError_c(loadFromFile(path), "Impossible de charger :\n" + path);
	}

	// Gestion du nom
	const std::string& GetName()
	{
		return mName;
	}

	// Gestion du chemin
	const std::string& GetPath()
	{
		return mPath;
	}

protected:
	friend class ResourceManager;

	void SetName(const std::string &name)
	{
		mName = name;
	}
	void SetPath(const std::string &path)
	{
		mPath = path;
	}

private:
	std::string mName;
	std::string mPath;
};
