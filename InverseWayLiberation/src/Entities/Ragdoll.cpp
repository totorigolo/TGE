#include "Ragdoll.h"
#include "../Physics/Joints/RevoluteJoint.h"
#include "../Physics/Joints/WeldJoint.h"
#include "../Tools/utils.h"
#include <iostream>

// Ctor
Ragdoll::Ragdoll(PhysicManager *physicMgr, b2Vec2 position, int layer)
	: Entity(layer), mIsValid(false), mPhysicMgr(physicMgr),
	mResourceManager(ResourceManager::GetInstance()),
	mTextureMap(mResourceManager.GetTextureMap())
{
	assert(mPhysicMgr && "n'est pas valide");

	mType = EntityType::Ragdoll;

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
	// Jambe, bras et pied gauches
	mBodies["mArmL1"] = new BasicBody(mPhysicMgr);
	mBodies["mArmL1"]->CreateDynBox(b2Vec3(0.f, 0.f, 0.f) + position, mTextureMap["hero_armL1"], 1.f, 0.2f, 0.f, -1);
	mBodies["mArmL2"] = new BasicBody(mPhysicMgr);
	mBodies["mArmL2"]->CreateDynBox(b2Vec3(0.f, -0.2f, 0.f) + position, mTextureMap["hero_armL2"], 1.f, 0.2f, 0.f, -1);
	mBodies["mLegL1"] = new BasicBody(mPhysicMgr);
	mBodies["mLegL1"]->CreateDynBox(b2Vec3(0.f, -0.3f, 0.f) + position, mTextureMap["hero_legL1"], 1.f, 0.2f, 0.f, -1);
	mBodies["mLegL2"] = new BasicBody(mPhysicMgr);
	mBodies["mLegL2"]->CreateDynBox(b2Vec3(0.f, -0.6f, 0.f) + position, mTextureMap["hero_legL2"], 1.f, 0.2f, 0.f, -1);
	mBodies["mFootL"] = new BasicBody(mPhysicMgr);
	mBodies["mFootL"]->CreateDynBox(b2Vec3(0.05f, -0.75f, 0.f) + position, mTextureMap["hero_footL"], 1.f, 0.2f, 0.f, -1);

	// Cou
	mBodies["mNeck"] = new BasicBody(mPhysicMgr);
	mBodies["mNeck"]->CreateDynBox(b2Vec3(0.f, 0.25f, 0.f) + position, mTextureMap["hero_neck"], 1.f, 0.2f, 0.f, -1);

	// Torse
	mBodies["mTorso3"] = new BasicBody(mPhysicMgr);
	mBodies["mTorso3"]->CreateDynBox(b2Vec3(0.f, -0.1f, 0.f) + position, mTextureMap["hero_torso3"], 1.f, 0.2f, 0.f, -1);
	mBodies["mTorso2"] = new BasicBody(mPhysicMgr);
	mBodies["mTorso2"]->CreateDynBox(b2Vec3(0.f, 0.f, 0.f) + position, mTextureMap["hero_torso2"], 1.f, 0.2f, 0.f, -1);
	mBodies["mTorso1"] = new BasicBody(mPhysicMgr);
	mBodies["mTorso1"]->CreateDynBox(b2Vec3(0.f, 0.15f, 0.f) + position, mTextureMap["hero_torso1"], 1.f, 0.2f, 0.f, -1);
	
	// Tête
	mBodies["mHead"] = new BasicBody(mPhysicMgr);
	mBodies["mHead"]->CreateDynCircle(b2Vec3(0.f, 0.5f, 0.f) + position, mTextureMap["hero_head"], 1.f, 0.2f, 0.f, -1);

	// Jambe, bras et pied droits
	mBodies["mLegR1"] = new BasicBody(mPhysicMgr);
	mBodies["mLegR1"]->CreateDynBox(b2Vec3(0.f, -0.3f, 0.f) + position, mTextureMap["hero_legR1"], 1.f, 0.2f, 0.f, -1);
	mBodies["mLegR2"] = new BasicBody(mPhysicMgr);
	mBodies["mLegR2"]->CreateDynBox(b2Vec3(0.f, -0.6f, 0.f) + position, mTextureMap["hero_legR2"], 1.f, 0.2f, 0.f, -1);
	mBodies["mArmR1"] = new BasicBody(mPhysicMgr);
	mBodies["mArmR1"]->CreateDynBox(b2Vec3(0.f, 0.f, 0.f) + position, mTextureMap["hero_armR1"], 1.f, 0.2f, 0.f, -1);
	mBodies["mArmR2"] = new BasicBody(mPhysicMgr);
	mBodies["mArmR2"]->CreateDynBox(b2Vec3(0.f, -0.2f, 0.f) + position, mTextureMap["hero_armR2"], 1.f, 0.2f, 0.f, -1);
	mBodies["mFootR"] = new BasicBody(mPhysicMgr);
	mBodies["mFootR"]->CreateDynBox(b2Vec3(0.05f, -0.75f, 0.f) + position, mTextureMap["hero_footR"], 1.f, 0.2f, 0.f, -1);

	/* Jointe les bodies entre eux */
	mJoints.push_back(new RevoluteJoint(mPhysicMgr, mBodies["mHead"]->GetBody(), mBodies["mTorso1"]->GetBody(), b2Vec2(0.f, -0.15f), true, -30.f, 50.f, false, 0.f, 0.004f));
	mJoints.push_back(new WeldJoint(mPhysicMgr, mBodies["mNeck"]->GetBody(), mBodies["mTorso1"]->GetBody(), b2Vec2(0.f, -0.05f), 0.f, 0.f));
		
	mJoints.push_back(new RevoluteJoint(mPhysicMgr, mBodies["mTorso1"]->GetBody(), mBodies["mTorso2"]->GetBody(), b2Vec2(0.f, -0.1f), true, -10.f, 10.f, true, 0.f, 0.004f));
	mJoints.push_back(new RevoluteJoint(mPhysicMgr, mBodies["mTorso2"]->GetBody(), mBodies["mTorso3"]->GetBody(), b2Vec2(0.f, -0.1f), true, -10.f, 10.f, true, 0.f, 0.004f));
		
	mJoints.push_back(new RevoluteJoint(mPhysicMgr, mBodies["mArmL1"]->GetBody(), mBodies["mTorso1"]->GetBody(), b2Vec2(0.f, 0.1f), true, -180.f, 0.f, true, 0.f, 0.004f));
	mJoints.push_back(new RevoluteJoint(mPhysicMgr, mBodies["mArmR1"]->GetBody(), mBodies["mTorso1"]->GetBody(), b2Vec2(0.f, 0.1f), true, -180.f, 0.f, true, 0.f, 0.004f));
		
	mJoints.push_back(new RevoluteJoint(mPhysicMgr, mBodies["mLegL1"]->GetBody(), mBodies["mTorso3"]->GetBody(), b2Vec2(0.f, 0.1f), true, -110.f, 30.f, true, 0.f, 0.004f));
	mJoints.push_back(new RevoluteJoint(mPhysicMgr, mBodies["mLegR1"]->GetBody(), mBodies["mTorso3"]->GetBody(), b2Vec2(0.f, 0.1f), true, -110.f, 30.f, true, 0.f, 0.004f));
		
	mJoints.push_back(new RevoluteJoint(mPhysicMgr, mBodies["mArmL1"]->GetBody(), mBodies["mArmL2"]->GetBody(), b2Vec2(0.f, -0.1f), true, 0.f, 150.f, true, 0.f, 0.004f));
	mJoints.push_back(new RevoluteJoint(mPhysicMgr, mBodies["mArmR1"]->GetBody(), mBodies["mArmR2"]->GetBody(), b2Vec2(0.f, -0.1f), true, 0.f, 150.f, true, 0.f, 0.004f));
		
	mJoints.push_back(new RevoluteJoint(mPhysicMgr, mBodies["mLegL1"]->GetBody(), mBodies["mLegL2"]->GetBody(), b2Vec2(0.f, -0.1f), true, -90.f, 0.f, true, 0.f, 0.004f));
	mJoints.push_back(new RevoluteJoint(mPhysicMgr, mBodies["mLegR1"]->GetBody(), mBodies["mLegR2"]->GetBody(), b2Vec2(0.f, -0.1f), true, -90.f, 0.f, true, 0.f, 0.004f));

	mJoints.push_back(new RevoluteJoint(mPhysicMgr, mBodies["mLegL2"]->GetBody(), mBodies["mFootL"]->GetBody(), b2Vec2(-0.035f, -0.15f), true, -90.f, 0.f, true, 50.f, 0.03f));
	mJoints.push_back(new RevoluteJoint(mPhysicMgr, mBodies["mLegR2"]->GetBody(), mBodies["mFootR"]->GetBody(), b2Vec2(-0.035f, -0.15f), true, -90.f, 0.f, true, 50.f, 0.03f));
	
	// Définit la force des joints
	mJoints.front()->SetBreakableByForce(true);
	mJoints.front()->SetMaxForce(100.f);

	// Valide le ragdoll
	mIsValid = true;
}

// Dtor
Ragdoll::~Ragdoll(void)
{
	// Les joints sont détruits automatiquement lors de la destruction des bodies

	// Supprime les bodies 
	for (std::unordered_map<std::string, BasicBody*>::iterator it = mBodies.begin(); it != mBodies.end(); ++it)
	{
		it->second->Destroy();
	}
	mBodies.clear();
	mJoints.clear();

	mPhysicMgr = nullptr;
	mIsValid = false;
}

// Met à jour la position des sprites
void Ragdoll::Update()
{
	// Met à jour les Bodies
	for (std::unordered_map<std::string, BasicBody*>::iterator it = mBodies.begin(); it != mBodies.end(); ++it)
	{
		it->second->Update();
	}

	// Met à jour les joints 
	for (std::list<Joint*>::iterator it = mJoints.begin(); it != mJoints.end(); )
	{
		(*it)->Update();
		if ((*it)->IsNull())
		{
			it = mJoints.erase(it);
		}
		else
			++it;
	}
}

// Pour le rendu
void Ragdoll::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	// Met à jour les Bodies
	for (std::unordered_map<std::string, BasicBody*>::const_iterator it = mBodies.begin(); it != mBodies.end(); ++it)
	{
		target.draw(*it->second, states);
	}

	// Met à jour les joints 
	for (std::list<Joint*>::const_iterator it = mJoints.begin(); it != mJoints.end(); ++it)
	{
		target.draw(**it, states);
	}
}
