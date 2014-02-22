#pragma once
#include "LevelManager.h"
#include "../App/InputManager.h"
#include "../Physics/PhysicManager.h"
#include "../Resources/ResourceManager.h"
#include "../Tools/Saver.h"

class LevelSaver : public Saver
{
public:
	// Ctor & dtor
	LevelSaver(const LevelManager &level, const std::string &path);
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

private:
	const LevelManager &mLevel;
	const InputManager &mInputManager;
	const PhysicManager &mPhysicManager;
	const TextureMap &mTextureMap;

	std::string mPath;

	std::map<b2Joint*, int> mJointIDMap;
};
