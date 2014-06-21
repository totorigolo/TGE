#pragma once

class Texture : public sf::Texture
{
public:
	typedef std::shared_ptr<Texture> Ptr;
	typedef std::shared_ptr<const Texture> PtrConst;

public:
	Texture(const std::string &name, const std::string &path)
		: mLoaded(false), mName(name), mPath(path)
	{
		mLoaded = loadFromFile(path);
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

private:
	bool mLoaded;
	std::string mName;
	std::string mPath;
};
