#pragma once
#include <SFML/Graphics.hpp>
#include <Box2D/Box2D.h>
#include <Thor/Resources.hpp>
#include <string>
#include <map>
#include "../World.h"
#include "../Bodies/DynamicBox.h"
#include "../Bodies/DynamicCircle.h"
#include "../Joints/RevoluteJoint.h"
#include "../Joints/WeldJoint.h"
#include "../../Resources/ResourceManager.h"

class World;
class DynamicBox;
class DynamicCircle;
class Ragdoll
{
public:
	Ragdoll(World *world, b2Vec3 posRot);
	virtual ~Ragdoll(void);

	// Met à jour la position des sprites
	void Update();

	// Accesseurs
	bool IsValid() const { return mIsValid; }

private:
	bool mIsValid;
	World *mWorld;
	ResourceManager &mResourceManager;
	TextureMap &mTextureMap;

	/* Bodies */
	// Tête et cou
	DynamicCircle *mHead;
	DynamicBox *mNeck;
	
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

	// Pieds
	DynamicBox *mFootL;
	DynamicBox *mFootR;

	/* Joints */
	RevoluteJoint *mHead_Torso1;
	WeldJoint *mNeck_Torso1;
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
	RevoluteJoint *mLegL2_FootL;
	RevoluteJoint *mLegR2_FootR;
};
