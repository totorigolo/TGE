#include "Ragdoll.h"
#include <iostream>

// Ctor
Ragdoll::Ragdoll(World *world, b2Vec3 posRot, thor::ResourceCache<sf::Texture> *textureCache, std::map<std::string, std::shared_ptr<sf::Texture>> *textureMap)
	: mIsValid(false), mWorld(world), mTextureMap(textureMap), mTextureCache(textureCache)
{
	if (mWorld)
	{
		// Charge les textures
		try {
			(*mTextureMap)["hero_head"] = mTextureCache->acquire(thor::Resources::fromFile<sf::Texture>("tex/hero/head.png"));

			(*mTextureMap)["hero_torso1"] = mTextureCache->acquire(thor::Resources::fromFile<sf::Texture>("tex/hero/torso1.png"));
			(*mTextureMap)["hero_torso2"] = mTextureCache->acquire(thor::Resources::fromFile<sf::Texture>("tex/hero/torso2.png"));
			(*mTextureMap)["hero_torso3"] = mTextureCache->acquire(thor::Resources::fromFile<sf::Texture>("tex/hero/torso3.png"));
			
			(*mTextureMap)["hero_armL1"] = mTextureCache->acquire(thor::Resources::fromFile<sf::Texture>("tex/hero/armL1.png"));
			(*mTextureMap)["hero_armL2"] = mTextureCache->acquire(thor::Resources::fromFile<sf::Texture>("tex/hero/armL2.png"));
			(*mTextureMap)["hero_armR1"] = mTextureCache->acquire(thor::Resources::fromFile<sf::Texture>("tex/hero/armR1.png"));
			(*mTextureMap)["hero_armR2"] = mTextureCache->acquire(thor::Resources::fromFile<sf::Texture>("tex/hero/armR2.png"));
			
			(*mTextureMap)["hero_legL1"] = mTextureCache->acquire(thor::Resources::fromFile<sf::Texture>("tex/hero/legL1.png"));
			(*mTextureMap)["hero_legL2"] = mTextureCache->acquire(thor::Resources::fromFile<sf::Texture>("tex/hero/legL2.png"));
			(*mTextureMap)["hero_legR1"] = mTextureCache->acquire(thor::Resources::fromFile<sf::Texture>("tex/hero/legR1.png"));
			(*mTextureMap)["hero_legR2"] = mTextureCache->acquire(thor::Resources::fromFile<sf::Texture>("tex/hero/legR2.png"));
		}
		catch (thor::ResourceLoadingException const& e)
		{
			std::cout << e.what() << std::endl;
			system("PAUSE");
			exit(223);
		}

		/* Crée les bodies (ordre d'apparition inverse) */
		// Jambe et bras gauches
		mLegL1 = new DynamicBox(mWorld, b2Vec3(0.f, -0.3f, 0.f) + posRot, (*mTextureMap)["hero_legL1"], 1.f, 0.2f, 0.f, -1);
		mWorld->RegisterBody(mLegL1);
		mLegL2 = new DynamicBox(mWorld, b2Vec3(0.f, -0.6f, 0.f) + posRot, (*mTextureMap)["hero_legL2"], 1.f, 0.2f, 0.f, -1);
		mWorld->RegisterBody(mLegL2);
		mArmL1 = new DynamicBox(mWorld, b2Vec3(0.f, 0.f, 0.f) + posRot, (*mTextureMap)["hero_armL1"], 1.f, 0.2f, 0.f, -1);
		mWorld->RegisterBody(mArmL1);
		mArmL2 = new DynamicBox(mWorld, b2Vec3(0.f, -0.2f, 0.f) + posRot, (*mTextureMap)["hero_armL2"], 1.f, 0.2f, 0.f, -1);
		mWorld->RegisterBody(mArmL2);

		// Torse
		mTorso3 = new DynamicBox(mWorld, b2Vec3(0.f, -0.1f, 0.f) + posRot, (*mTextureMap)["hero_torso3"], 1.f, 0.2f, 0.f, -1);
		mWorld->RegisterBody(mTorso3);
		mTorso2 = new DynamicBox(mWorld, b2Vec3(0.f, 0.f, 0.f) + posRot, (*mTextureMap)["hero_torso2"], 1.f, 0.2f, 0.f, -1);
		mWorld->RegisterBody(mTorso2);
		mTorso1 = new DynamicBox(mWorld, b2Vec3(0.f, 0.15f, 0.f) + posRot, (*mTextureMap)["hero_torso1"], 1.f, 0.2f, 0.f, -1);
		mWorld->RegisterBody(mTorso1);
	
		// Tête
		mHead = new DynamicCircle(mWorld, b2Vec3(0.f, 0.45f, 0.f) + posRot, (*mTextureMap)["hero_head"], 1.f, 0.2f, 0.f, -1, 0x0002);
		mWorld->RegisterBody(mHead);

		// Bras et jambe droits
		mLegR1 = new DynamicBox(mWorld, b2Vec3(0.f, -0.3f, 0.f) + posRot, (*mTextureMap)["hero_legR1"], 1.f, 0.2f, 0.f, -1);
		mWorld->RegisterBody(mLegR1);
		mLegR2 = new DynamicBox(mWorld, b2Vec3(0.f, -0.6f, 0.f) + posRot, (*mTextureMap)["hero_legR2"], 1.f, 0.2f, 0.f, -1);
		mWorld->RegisterBody(mLegR2);
		mArmR1 = new DynamicBox(mWorld, b2Vec3(0.f, 0.f, 0.f) + posRot, (*mTextureMap)["hero_armR1"], 1.f, 0.2f, 0.f, -1);
		mWorld->RegisterBody(mArmR1);
		mArmR2 = new DynamicBox(mWorld, b2Vec3(0.f, -0.2f, 0.f) + posRot, (*mTextureMap)["hero_armR2"], 1.f, 0.2f, 0.f, -1);
		mWorld->RegisterBody(mArmR2);

		/* Jointe les bodies entre eux */
		mHead_Torso1 = new RevoluteJoint(mWorld, mHead, mTorso1, b2Vec2(0.f, -0.1f), true, -60.f, 60.f, true, 0.f, 0.002f);
		mWorld->RegisterJoint(mHead_Torso1);
		
		mTorso1_2 = new RevoluteJoint(mWorld, mTorso1, mTorso2, b2Vec2(0.f, -0.1f), true, -20.f, 20.f, true, 0.f, 0.002f);
		mWorld->RegisterJoint(mTorso1_2);
		mTorso2_3 = new RevoluteJoint(mWorld, mTorso2, mTorso3, b2Vec2(0.f, -0.1f), true, -20.f, 20.f, true, 0.f, 0.002f);
		mWorld->RegisterJoint(mTorso2_3);
		
		mArmL_Torso1 = new RevoluteJoint(mWorld, mArmL1, mTorso1, b2Vec2(0.f, 0.1f), true, -180.f, 0.f, true, 0.f, 0.002f);
		mWorld->RegisterJoint(mArmL_Torso1);
		mArmR_Torso1 = new RevoluteJoint(mWorld, mArmR1, mTorso1, b2Vec2(0.f, 0.1f), true, -180.f, 0.f, true, 0.f, 0.002f);
		mWorld->RegisterJoint(mArmR_Torso1);
		
		mLegL_Torso3 = new RevoluteJoint(mWorld, mLegL1, mTorso3, b2Vec2(0.f, 0.1f), true, -110.f, 20.f, true, 0.f, 0.002f);
		mWorld->RegisterJoint(mLegL_Torso3);
		mLegR_Torso3 = new RevoluteJoint(mWorld, mLegR1, mTorso3, b2Vec2(0.f, 0.1f), true, -110.f, 20.f, true, 0.f, 0.002f);
		mWorld->RegisterJoint(mLegR_Torso3);
		
		mArmL1_2 = new RevoluteJoint(mWorld, mArmL1, mArmL2, b2Vec2(0.f, -0.1f), true, 0.f, 180.f, true, 0.f, 0.002f);
		mWorld->RegisterJoint(mArmL1_2);
		mArmR1_2 = new RevoluteJoint(mWorld, mArmR1, mArmR2, b2Vec2(0.f, -0.1f), true, 0.f, 180.f, true, 0.f, 0.002f);
		mWorld->RegisterJoint(mArmR1_2);
		
		mLegL1_2 = new RevoluteJoint(mWorld, mLegL1, mLegL2, b2Vec2(0.f, -0.1f), true, -90.f, 0.f, true, 0.f, 0.002f);
		mWorld->RegisterJoint(mArmL1_2);
		mLegR1_2 = new RevoluteJoint(mWorld, mLegR1, mLegR2, b2Vec2(0.f, -0.1f), true, -90.f, 0.f, true, 0.f, 0.002f);
		mWorld->RegisterJoint(mLegR1_2);

		// Valide le ragdoll
		mIsValid = true;
	}
}

// Dtor
Ragdoll::~Ragdoll(void)
{
	// Les bodies et les joints sont supprimés automatiquement

	mTextureCache = nullptr;
	mTextureMap = nullptr;

	mWorld = nullptr;
	mIsValid = false;
}

// Met à jour la position des sprites
void Ragdoll::Update()
{
	// Rien pour le moment
}
