#pragma once

class Font : public sf::Font
{
public:
	typedef std::shared_ptr<Font> Ptr;
	typedef std::shared_ptr<const Font> PtrConst;

public:
	Font(const std::string &name, const std::string &path)
		: mLoaded(false), mName(name), mPath(path)
	{
		mLoaded = sf::Font::loadFromFile(path);
		if (!mLoaded) {
			// Rescue
			mLoaded = sf::Font::loadFromFile("/usr/share/fonts/TTF/LiberationSans-Regular.ttf");
		}
		myCheckError_c(mLoaded, "Impossible de charger :\n" + path);
	}
	
	// Gestion de l'état
	bool IsLoaded()
	{
		return mLoaded;
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

protected:
	// Impossible de changer la police
	bool loadFromFile(const std::string& filename)
	{
		return sf::Font::loadFromFile(filename);
	}
	bool loadFromMemory(const void* data, std::size_t sizeInBytes)
	{
		return sf::Font::loadFromMemory(data, sizeInBytes);
	}
	bool loadFromStream(sf::InputStream& stream)
	{
		return sf::Font::loadFromStream(stream);
	}

private:
	bool mLoaded;
	std::string mName;
	std::string mPath;
};
