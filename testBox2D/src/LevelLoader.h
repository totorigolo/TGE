#pragma once
#include "Physics/World.h"
#include "Level.h"
#include "ConfigReader.h"
#include <SFML/Graphics.hpp>
#include <ostream>
#include <fstream>
#include <cassert>
#include <string>
#include <tuple>
#include <map>

class Level;
class LevelLoader
{
public:
	// Ctor & dtor
	LevelLoader(std::string const& path, Level *level);
	virtual ~LevelLoader(void);

private:
	// Analyse tout le fichier
	bool Process();

	// Parse la ligne
	const std::string ParseLine(std::string& line, std::string& section, int lineNumber);

	// Crée le contenu d'une ligne
	bool CreateLine(std::string& section, std::string& name, std::string& value);

private:
	Level *mLevel;
	std::string mPath;
	std::fstream* mFile;
};
