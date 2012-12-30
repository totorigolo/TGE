#pragma once
#include "Level.h"
#include "../Tools/Loader.h"
#include "../Physics/World.h"
#include "../Physics/Bodies/Body.h"
#include "../Physics/Joints/Joint.h"
#include <SFML/Graphics.hpp>
#include <string>
#include <map>

class Level;
class LevelLoader : public Loader
{
public:
	// Ctor & dtor
	LevelLoader(std::string const& path, Level *level);
	virtual ~LevelLoader(void);
	
private:
	// Analyse tout le fichier
	virtual bool Process();

	// Une fonction par catégorie
	bool ProcessWorld();
	bool ProcessTextures();
	bool ProcessBodies();
	bool ProcessEntities();
	bool ProcessJoints();
	bool ProcessDeco();
	bool ProcessLights();

private:
	Level *mLevel;
	
	std::map<unsigned int, Body*> mBodyIDMap;
	std::map<unsigned int, Joint*> mJointIDMap;
};
