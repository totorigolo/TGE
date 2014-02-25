#pragma once
#include "NonCopyable.h"

#include <tinyxml2.h>
#include <string>

class Saver : public NonCopyable
{
public:
	// Ctor & dtor
	Saver(std::string const& path, bool check = true);
	virtual ~Saver(void);

	// Accesseurs
	bool Exists() const;
	bool IsValid() const;
	const std::string& GetPath() const;

protected:
	// Analyse tout le fichier
	virtual bool Process() = 0;

protected:
	bool mExists;
	bool mIsValid;
	std::string mPath;
	tinyxml2::XMLDocument mDoc;
};
