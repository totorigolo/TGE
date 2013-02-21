#pragma once
#include <Box2D/Box2D.h>
#include <string>
#include <map>
#include "Level.h"
#include "../Tools/Loader.h"

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
	bool ProcessBasicBodies();
	bool ProcessEntities();
	bool ProcessJoints();
	bool ProcessDeco();
	//bool ProcessLights();

private:
	Level *mLevel;
	
	std::map<unsigned int, b2Body*> mBodyIDMap;
	std::map<unsigned int, b2Joint*> mJointIDMap;
};
