#pragma once
#include <Box2D/Box2D.h>
#include <unordered_map>
#include <string>
#include <list>
#include "Entity.h"
#include "BasicBody.h"
#include "../Physics/Joint.h"
#include "../Physics/PhysicManager.h"
#include "../Resources/ResourceManager.h"

class Ragdoll : public Entity
{
public:
	Ragdoll(PhysicManager *physicMgr, b2Vec2 position, int layer = 1);
	virtual ~Ragdoll(void);

	// Met à jour la position des sprites
	virtual void Update();
	
	// Pour le rendu
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

	// Accesseurs
	bool IsValid() const { return mIsValid; }

private:
	// Etat
	bool mIsValid;

	// Monde
	PhysicManager *mPhysicMgr;

	// Textures
	ResourceManager &mResourceManager;
	TextureMap &mTextureMap;

	// Bodies
	std::unordered_map<std::string, BasicBody*> mBodies;
	std::list<Joint*> mJoints;
};
