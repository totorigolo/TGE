#pragma once
#include "../Tools/Loader.h"

class LevelManager;
class InputManager;
class EntityManager;
class PhysicManager;
class LevelLoader : public Loader
{
public:
	// Ctor & dtor
	LevelLoader(const std::string &path);
	virtual ~LevelLoader(void);
	
private:
	// Analyse tout le fichier
	virtual bool Process();

	// Une fonction par catégorie
	bool ProcessWorld();
	bool ProcessTextures();
	bool ProcessPoly();
	bool ProcessBasicBodies();
	bool ProcessEntities();
	bool ProcessJoints();
	bool ProcessDeco();
	bool ProcessActions();
	bool ProcessTriggers();
	bool ProcessLights();

private:
	LevelManager &mLevel;
	InputManager &mInputManager;
	EntityManager &mEntityManager;
	PhysicManager &mPhysicManager;
	
	std::map<unsigned int, b2Joint*> mJointIDMap;
};
