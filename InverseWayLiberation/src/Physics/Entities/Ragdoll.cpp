#include "Ragdoll.h"
#include <iostream>

// Ctor
Ragdoll::Ragdoll(World *world, b2Vec3 posRot)
	: mIsValid(false), mWorld(world),
	mResourceManager(ResourceManager::GetInstance()), mTextureMap(mResourceManager.GetTextureMap())
{
	if (mWorld)
	{
		// Charge les textures
		try {
			mTextureMap["hero_head"] = mResourceManager.acquire(thor::Resources::fromFile<sf::Texture>("tex/hero/head.png"));
			mTextureMap["hero_neck"] = mResourceManager.acquire(thor::Resources::fromFile<sf::Texture>("tex/hero/neck.png"));

			mTextureMap["hero_torso1"] = mResourceManager.acquire(thor::Resources::fromFile<sf::Texture>("tex/hero/torso1.png"));
			mTextureMap["hero_torso2"] = mResourceManager.acquire(thor::Resources::fromFile<sf::Texture>("tex/hero/torso2.png"));
			mTextureMap["hero_torso3"] = mResourceManager.acquire(thor::Resources::fromFile<sf::Texture>("tex/hero/torso3.png"));
			
			mTextureMap["hero_armL1"] = mResourceManager.acquire(thor::Resources::fromFile<sf::Texture>("tex/hero/armL1.png"));
			mTextureMap["hero_armL2"] = mResourceManager.acquire(thor::Resources::fromFile<sf::Texture>("tex/hero/armL2.png"));
			mTextureMap["hero_armR1"] = mResourceManager.acquire(thor::Resources::fromFile<sf::Texture>("tex/hero/armR1.png"));
			mTextureMap["hero_armR2"] = mResourceManager.acquire(thor::Resources::fromFile<sf::Texture>("tex/hero/armR2.png"));
			
			mTextureMap["hero_legL1"] = mResourceManager.acquire(thor::Resources::fromFile<sf::Texture>("tex/hero/legL1.png"));
			mTextureMap["hero_legL2"] = mResourceManager.acquire(thor::Resources::fromFile<sf::Texture>("tex/hero/legL2.png"));
			mTextureMap["hero_legR1"] = mResourceManager.acquire(thor::Resources::fromFile<sf::Texture>("tex/hero/legR1.png"));
			mTextureMap["hero_legR2"] = mResourceManager.acquire(thor::Resources::fromFile<sf::Texture>("tex/hero/legR2.png"));
			
			mTextureMap["hero_footL"] = mResourceManager.acquire(thor::Resources::fromFile<sf::Texture>("tex/hero/footL.png"));
			mTextureMap["hero_footR"] = mResourceManager.acquire(thor::Resources::fromFile<sf::Texture>("tex/hero/footR.png"));
		}
		catch (thor::ResourceLoadingException const& e)
		{
			std::cout << e.what() << std::endl;
			system("PAUSE");
			exit(223);
		}

		/* Crée les bodies (ordre d'apparition inverse) */
		// Jambe, bras et pieds gauches
		mLegL1 = new DynamicBox(mWorld, b2Vec3(0.f, -0.3f, 0.f) + posRot, mTextureMap["hero_legL1"], 1.f, 0.2f, 0.f, -1);
		mWorld->RegisterBody(mLegL1);
		mLegL2 = new DynamicBox(mWorld, b2Vec3(0.f, -0.6f, 0.f) + posRot, mTextureMap["hero_legL2"], 1.f, 0.2f, 0.f, -1);
		mWorld->RegisterBody(mLegL2);
		mArmL1 = new DynamicBox(mWorld, b2Vec3(0.f, 0.f, 0.f) + posRot, mTextureMap["hero_armL1"], 1.f, 0.2f, 0.f, -1);
		mWorld->RegisterBody(mArmL1);
		mArmL2 = new DynamicBox(mWorld, b2Vec3(0.f, -0.2f, 0.f) + posRot, mTextureMap["hero_armL2"], 1.f, 0.2f, 0.f, -1);
		mWorld->RegisterBody(mArmL2);
		mFootL = new DynamicBox(mWorld, b2Vec3(0.05f, -0.75f, 0.f) + posRot, mTextureMap["hero_footL"], 1.f, 0.2f, 0.f, -1);
		mWorld->RegisterBody(mFootL);

		// Cou
		mNeck = new DynamicBox(mWorld, b2Vec3(0.f, 0.25f, 0.f) + posRot, mTextureMap["hero_neck"], 1.f, 0.2f, 0.f, -1);
		mWorld->RegisterBody(mNeck);

		// Torse
		mTorso3 = new DynamicBox(mWorld, b2Vec3(0.f, -0.1f, 0.f) + posRot, mTextureMap["hero_torso3"], 1.f, 0.2f, 0.f, -1);
		mWorld->RegisterBody(mTorso3);
		mTorso2 = new DynamicBox(mWorld, b2Vec3(0.f, 0.f, 0.f) + posRot, mTextureMap["hero_torso2"], 1.f, 0.2f, 0.f, -1);
		mWorld->RegisterBody(mTorso2);
		mTorso1 = new DynamicBox(mWorld, b2Vec3(0.f, 0.15f, 0.f) + posRot, mTextureMap["hero_torso1"], 1.f, 0.2f, 0.f, -1);
		mWorld->RegisterBody(mTorso1);
	
		// Tête
		mHead = new DynamicCircle(mWorld, b2Vec3(0.f, 0.5f, 0.f) + posRot, mTextureMap["hero_head"], 1.f, 0.2f, 0.f, -1);
		mWorld->RegisterBody(mHead);

		// Bras, jambe et pieds droits
		mLegR1 = new DynamicBox(mWorld, b2Vec3(0.f, -0.3f, 0.f) + posRot, mTextureMap["hero_legR1"], 1.f, 0.2f, 0.f, -1);
		mWorld->RegisterBody(mLegR1);
		mLegR2 = new DynamicBox(mWorld, b2Vec3(0.f, -0.6f, 0.f) + posRot, mTextureMap["hero_legR2"], 1.f, 0.2f, 0.f, -1);
		mWorld->RegisterBody(mLegR2);
		mArmR1 = new DynamicBox(mWorld, b2Vec3(0.f, 0.f, 0.f) + posRot, mTextureMap["hero_armR1"], 1.f, 0.2f, 0.f, -1);
		mWorld->RegisterBody(mArmR1);
		mArmR2 = new DynamicBox(mWorld, b2Vec3(0.f, -0.2f, 0.f) + posRot, mTextureMap["hero_armR2"], 1.f, 0.2f, 0.f, -1);
		mWorld->RegisterBody(mArmR2);
		mFootR = new DynamicBox(mWorld, b2Vec3(0.05f, -0.75f, 0.f) + posRot, mTextureMap["hero_footR"], 1.f, 0.2f, 0.f, -1);
		mWorld->RegisterBody(mFootR);

		/* Jointe les bodies entre eux */
		mHead_Torso1 = new RevoluteJoint(mWorld, mHead, mTorso1, b2Vec2(0.f, -0.15f), true, -30.f, 50.f, false, 0.f, 0.004f);
		mWorld->RegisterJoint(mHead_Torso1);
		mNeck_Torso1 = new WeldJoint(mWorld, mNeck, mTorso1, b2Vec2(0.f, -0.05f), 0.f, 0.f);
		mWorld->RegisterJoint(mNeck_Torso1);
		
		mTorso1_2 = new RevoluteJoint(mWorld, mTorso1, mTorso2, b2Vec2(0.f, -0.1f), true, -10.f, 10.f, true, 0.f, 0.004f);
		mWorld->RegisterJoint(mTorso1_2);
		mTorso2_3 = new RevoluteJoint(mWorld, mTorso2, mTorso3, b2Vec2(0.f, -0.1f), true, -10.f, 10.f, true, 0.f, 0.004f);
		mWorld->RegisterJoint(mTorso2_3);
		
		mArmL_Torso1 = new RevoluteJoint(mWorld, mArmL1, mTorso1, b2Vec2(0.f, 0.1f), true, -180.f, 0.f, true, 0.f, 0.004f);
		mWorld->RegisterJoint(mArmL_Torso1);
		mArmR_Torso1 = new RevoluteJoint(mWorld, mArmR1, mTorso1, b2Vec2(0.f, 0.1f), true, -180.f, 0.f, true, 0.f, 0.004f);
		mWorld->RegisterJoint(mArmR_Torso1);
		
		mLegL_Torso3 = new RevoluteJoint(mWorld, mLegL1, mTorso3, b2Vec2(0.f, 0.1f), true, -110.f, 30.f, true, 0.f, 0.004f);
		mWorld->RegisterJoint(mLegL_Torso3);
		mLegR_Torso3 = new RevoluteJoint(mWorld, mLegR1, mTorso3, b2Vec2(0.f, 0.1f), true, -110.f, 30.f, true, 0.f, 0.004f);
		mWorld->RegisterJoint(mLegR_Torso3);
		
		mArmL1_2 = new RevoluteJoint(mWorld, mArmL1, mArmL2, b2Vec2(0.f, -0.1f), true, 0.f, 150.f, true, 0.f, 0.004f);
		mWorld->RegisterJoint(mArmL1_2);
		mArmR1_2 = new RevoluteJoint(mWorld, mArmR1, mArmR2, b2Vec2(0.f, -0.1f), true, 0.f, 150.f, true, 0.f, 0.004f);
		mWorld->RegisterJoint(mArmR1_2);
		
		mLegL1_2 = new RevoluteJoint(mWorld, mLegL1, mLegL2, b2Vec2(0.f, -0.1f), true, -90.f, 0.f, true, 0.f, 0.004f);
		mWorld->RegisterJoint(mArmL1_2);
		mLegR1_2 = new RevoluteJoint(mWorld, mLegR1, mLegR2, b2Vec2(0.f, -0.1f), true, -90.f, 0.f, true, 0.f, 0.004f);
		mWorld->RegisterJoint(mLegR1_2);

		mLegL2_FootL = new RevoluteJoint(mWorld, mLegL2, mFootL, b2Vec2(-0.035f, -0.15f), true, -90.f, 0.f, true, 50.f, 0.3f);
		mWorld->RegisterJoint(mLegL2_FootL);
		mLegR2_FootR = new RevoluteJoint(mWorld, mLegR2, mFootR, b2Vec2(-0.035f, -0.15f), true, -90.f, 0.f, true, 50.f, 0.3f);
		mWorld->RegisterJoint(mLegR2_FootR);

		// Valide le ragdoll
		mIsValid = true;
	}
}

// Dtor
Ragdoll::~Ragdoll(void)
{
	// Les bodies et les joints sont supprimés automatiquement

	mWorld = nullptr;
	mIsValid = false;

	// Supprime les joints (ils sont détruits automatiquement lors de la destruction des bodies)
	/*mWorld->DestroyJoint(mHead_Torso1);
	mWorld->DestroyJoint(mNeck_Torso1);
	mWorld->DestroyJoint(mTorso1_2);
	mWorld->DestroyJoint(mTorso2_3);
	mWorld->DestroyJoint(mArmL_Torso1);
	mWorld->DestroyJoint(mArmR_Torso1);
	mWorld->DestroyJoint(mLegL_Torso3);
	mWorld->DestroyJoint(mLegR_Torso3);
	mWorld->DestroyJoint(mArmL1_2);
	mWorld->DestroyJoint(mArmR1_2);
	mWorld->DestroyJoint(mLegL1_2);
	mWorld->DestroyJoint(mLegR1_2);
	mWorld->DestroyJoint(mLegL2_FootL);
	mWorld->DestroyJoint(mLegR2_FootR);*/

	// Supprime les bodies 
	/*mWorld->DestroyBody(mHead);
	mWorld->DestroyBody(mNeck);
	mWorld->DestroyBody(mTorso1);
	mWorld->DestroyBody(mTorso2);
	mWorld->DestroyBody(mTorso3);
	mWorld->DestroyBody(mArmL1);
	mWorld->DestroyBody(mArmL2);
	mWorld->DestroyBody(mArmR1);
	mWorld->DestroyBody(mArmR2);
	mWorld->DestroyBody(mLegL1);
	mWorld->DestroyBody(mLegL2);
	mWorld->DestroyBody(mLegR1);
	mWorld->DestroyBody(mLegR2);
	mWorld->DestroyBody(mFootL);
	mWorld->DestroyBody(mFootR);*/

}

// Met à jour la position des sprites
void Ragdoll::Update()
{
	// Rien à faire
}
