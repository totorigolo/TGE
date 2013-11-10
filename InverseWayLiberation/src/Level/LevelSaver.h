#pragma once
#include "LevelManager.h"
#include "../Game/InputManager.h"
#include "../Physics/PhysicManager.h"
#include "../Resources/ResourceManager.h"
#include "../Tools/Saver.h"

#include <string>

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
	bool ProcessBasicBodies();
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
};
