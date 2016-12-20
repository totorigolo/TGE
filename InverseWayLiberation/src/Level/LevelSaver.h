#pragma once
#include "../Resources/ResourceManager.h"
#include "../Tools/Saver.h"

class LevelManager;
class InputManager;
class EntityManager;
class PhysicManager;
class TriggersManager;
class ResourceManager;
class LevelSaver : public Saver
{
public:
	// Ctor & dtor
	LevelSaver(const std::string &path, bool check = true);
	virtual ~LevelSaver(void);
	
private:
	// Sauvegarde tout le niveau
	virtual bool Process();

	// Une fonction par catégorie
	bool ProcessWorld();
	bool ProcessTextures();
	bool ProcessBodies();
	bool ProcessEntities();
	bool ProcessJoints();
	bool ProcessDeco();
	bool ProcessActions();
	bool ProcessTriggers();
	bool ProcessLights();

private:
	const LevelManager &mLevel;
	const InputManager &mInputManager;
	const EntityManager &mEntityManager;
	const PhysicManager &mPhysicManager;
	const TriggersManager &mTriggersManager;
	const ResourceManager &mResourceManager;

	std::string mPath;

	std::map<b2Joint*, int> mJointIDMap;
};
