#pragma once
#include "Entity.h"
#include "../Physics/Bodies/Body.h"
#include "../Physics/Joints/Joint.h"
#include "../Resources/ResourceManager.h"
#include <list>
#include <string>
#include <unordered_map>

class Ragdoll : public Entity
{
public:
	Ragdoll(World *world, b2Vec2 position, int layer = 1);
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
	World *mWorld;

	// Textures
	ResourceManager &mResourceManager;
	TextureMap &mTextureMap;

	// Bodies
	std::unordered_map<std::string, Body*> mBodies;
	std::list<Joint*> mJoints;
};
