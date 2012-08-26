#pragma once
#include <SFML/Graphics.hpp>
#include <Box2D/Box2D.h>
#include <Thor/Resources.hpp>
#include <string>
#include <map>
#include "World.h"
#include "DynamicBox.h"
#include "DynamicCircle.h"
#include "RevoluteJoint.h"

class World;
class DynamicBox;
class DynamicCircle;
class Ragdoll
{
public:
	Ragdoll(World *world, b2Vec3 posRot, thor::ResourceCache<sf::Texture> *textureCache, std::map<std::string, std::shared_ptr<sf::Texture>> *textureMap);
	virtual ~Ragdoll(void);

	// Met à jour la position des sprites
	void Update();

private:
	bool mIsValid;
	World *mWorld;
	thor::ResourceCache<sf::Texture> *mTextureCache;
	std::map<std::string, std::shared_ptr<sf::Texture>> *mTextureMap;

	/* Bodies */
	// Tête
	DynamicCircle *mHead;
	
	// Torse
	DynamicBox *mTorso1;
	DynamicBox *mTorso2;
	DynamicBox *mTorso3;
	
	// Bras
	DynamicBox *mArmL1;
	DynamicBox *mArmL2;
	DynamicBox *mArmR1;
	DynamicBox *mArmR2;

	// Jambes
	DynamicBox *mLegL1;
	DynamicBox *mLegL2;
	DynamicBox *mLegR1;
	DynamicBox *mLegR2;

	/* Joints */
	RevoluteJoint *mHead_Torso1;
	RevoluteJoint *mTorso1_2;
	RevoluteJoint *mTorso2_3;
	RevoluteJoint *mArmL_Torso1;
	RevoluteJoint *mArmR_Torso1;
	RevoluteJoint *mLegL_Torso3;
	RevoluteJoint *mLegR_Torso3;
	RevoluteJoint *mArmL1_2;
	RevoluteJoint *mArmR1_2;
	RevoluteJoint *mLegL1_2;
	RevoluteJoint *mLegR1_2;
};
