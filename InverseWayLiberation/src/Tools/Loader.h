#pragma once
#include <string>
#include <tinyxml2.h>
#include "NonCopyable.h"

class Loader : public NonCopyable
{
public:
	// Ctor & dtor
	Loader(std::string const& path);
	virtual ~Loader(void);

	// Accesseurs
	bool IsValid() const;
	const std::string& GetPath() const;

protected:
	// Analyse tout le fichier
	virtual bool Process() = 0;

protected:
	bool mIsValid;
	std::string mPath;
	tinyxml2::XMLDocument mFile;
};
