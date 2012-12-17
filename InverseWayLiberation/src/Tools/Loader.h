#pragma once
#include <string>
#include <tinyxml2.h>

class Loader
{
public:
	// Ctor & dtor
	Loader(std::string const& path);
	virtual ~Loader(void);

protected:
	// Analyse tout le fichier
	virtual bool Process() = 0;

protected:
	bool mIsValid;
	std::string mPath;
	tinyxml2::XMLDocument mFile;
};
