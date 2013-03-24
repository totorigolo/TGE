#include "EntityFactory.h"
#include "../Resources/ResourceManager.h"
#include "EntityManager.h"
#include "BasicBody.h"
#include "../Physics/Joints/RevoluteJoint.h"
#include "../Physics/Joints/WeldJoint.h"
#include "../Physics/Joint.h"
#include "../Tools/utils.h"
#include <iostream>
#include <cassert>
#include <list>
#include <map>

namespace EntityFactory
{
	// Variables privés
	namespace
	{
		// Physique
		PhysicManager *mPhysicManager = nullptr;

		// Entities
		EntityManager &mEntityManager(EntityManager::GetInstance());

		// Resources
		ResourceManager &mResourceManager(ResourceManager::GetInstance());
		TextureMap &mTextureMap(mResourceManager.GetTextureMap());
	}
	
	// Parametres de la Factory
	void SetPhysicManager(PhysicManager *physicMgr)
	{
		mPhysicManager = physicMgr;
	}

	// Crée un Ragdoll
	void CreateRagdoll(const b2Vec2 &position, int layer)
	{
		if (!mPhysicManager) return;

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
		// std::map pour les stocker temporairement
		std::map<std::string, BasicBody*> bodies;

		// Jambe, bras et pied gauche
		bodies["11_armL1"] = new BasicBody(mPhysicManager, layer);
		bodies["11_armL1"]->CreateDynBox(b2Vec3(0.f, 0.f, 0.f) + position, mTextureMap["hero_armL1"], 1.f, 0.2f, 0.f, -1);
		bodies["11_armL2"] = new BasicBody(mPhysicManager, layer);
		bodies["11_armL2"]->CreateDynBox(b2Vec3(0.f, -0.2f, 0.f) + position, mTextureMap["hero_armL2"], 1.f, 0.2f, 0.f, -1);
		bodies["12_legL1"] = new BasicBody(mPhysicManager, layer);
		bodies["12_legL1"]->CreateDynBox(b2Vec3(0.f, -0.3f, 0.f) + position, mTextureMap["hero_legL1"], 1.f, 0.2f, 0.f, -1);
		bodies["12_legL2"] = new BasicBody(mPhysicManager, layer);
		bodies["12_legL2"]->CreateDynBox(b2Vec3(0.f, -0.6f, 0.f) + position, mTextureMap["hero_legL2"], 1.f, 0.2f, 0.f, -1);
		bodies["13_footL"] = new BasicBody(mPhysicManager, layer);
		bodies["13_footL"]->CreateDynBox(b2Vec3(0.05f, -0.75f, 0.f) + position, mTextureMap["hero_footL"], 1.f, 0.2f, 0.f, -1);

		// Cou
		bodies["20_neck"] = new BasicBody(mPhysicManager, layer);
		bodies["20_neck"]->CreateDynBox(b2Vec3(0.f, 0.25f, 0.f) + position, mTextureMap["hero_neck"], 1.f, 0.2f, 0.f, -1);

		// Torse
		bodies["31_torso3"] = new BasicBody(mPhysicManager, layer);
		bodies["31_torso3"]->CreateDynBox(b2Vec3(0.f, -0.1f, 0.f) + position, mTextureMap["hero_torso3"], 1.f, 0.2f, 0.f, -1);
		bodies["32_torso2"] = new BasicBody(mPhysicManager, layer);
		bodies["32_torso2"]->CreateDynBox(b2Vec3(0.f, 0.f, 0.f) + position, mTextureMap["hero_torso2"], 1.f, 0.2f, 0.f, -1);
		bodies["33_torso1"] = new BasicBody(mPhysicManager, layer);
		bodies["33_torso1"]->CreateDynBox(b2Vec3(0.f, 0.15f, 0.f) + position, mTextureMap["hero_torso1"], 1.f, 0.2f, 0.f, -1);
	
		// Tête
		bodies["40_head"] = new BasicBody(mPhysicManager, layer);
		bodies["40_head"]->CreateDynCircle(b2Vec3(0.f, 0.5f, 0.f) + position, mTextureMap["hero_head"], 1.f, 0.5f, 0.f, -1);

		// Jambe, bras et pied droit
		bodies["51_legR1"] = new BasicBody(mPhysicManager, layer);
		bodies["51_legR1"]->CreateDynBox(b2Vec3(0.f, -0.3f, 0.f) + position, mTextureMap["hero_legR1"], 1.f, 0.2f, 0.f, -1);
		bodies["51_legR2"] = new BasicBody(mPhysicManager, layer);
		bodies["51_legR2"]->CreateDynBox(b2Vec3(0.f, -0.6f, 0.f) + position, mTextureMap["hero_legR2"], 1.f, 0.2f, 0.f, -1);
		bodies["52_armR1"] = new BasicBody(mPhysicManager, layer);
		bodies["52_armR1"]->CreateDynBox(b2Vec3(0.f, 0.f, 0.f) + position, mTextureMap["hero_armR1"], 1.f, 0.2f, 0.f, -1);
		bodies["52_armR2"] = new BasicBody(mPhysicManager, layer);
		bodies["52_armR2"]->CreateDynBox(b2Vec3(0.f, -0.2f, 0.f) + position, mTextureMap["hero_armR2"], 1.f, 0.2f, 0.f, -1);
		bodies["53_footR"] = new BasicBody(mPhysicManager, layer);
		bodies["53_footR"]->CreateDynBox(b2Vec3(0.05f, -0.75f, 0.f) + position, mTextureMap["hero_footR"], 1.f, 0.2f, 0.f, -1);

		/* Jointe les bodies entre eux */
		std::list<Joint*> joints;
		joints.push_back(new RevoluteJoint(mPhysicManager, bodies["40_head"]->GetBody(), bodies["33_torso1"]->GetBody(), b2Vec2(0.f, -0.15f), true, -30.f, 50.f, false, 0.f, 0.004f));
		joints.push_back(new WeldJoint(mPhysicManager, bodies["20_neck"]->GetBody(), bodies["33_torso1"]->GetBody(), b2Vec2(0.f, -0.05f), 0.f, 0.f));
		
		joints.push_back(new RevoluteJoint(mPhysicManager, bodies["33_torso1"]->GetBody(), bodies["32_torso2"]->GetBody(), b2Vec2(0.f, -0.1f), true, -10.f, 10.f, true, 0.f, 0.004f));
		joints.push_back(new RevoluteJoint(mPhysicManager, bodies["32_torso2"]->GetBody(), bodies["31_torso3"]->GetBody(), b2Vec2(0.f, -0.1f), true, -10.f, 10.f, true, 0.f, 0.004f));
		
		joints.push_back(new RevoluteJoint(mPhysicManager, bodies["11_armL1"]->GetBody(), bodies["33_torso1"]->GetBody(), b2Vec2(0.f, 0.1f), true, -180.f, 0.f, true, 0.f, 0.004f));
		joints.push_back(new RevoluteJoint(mPhysicManager, bodies["52_armR1"]->GetBody(), bodies["33_torso1"]->GetBody(), b2Vec2(0.f, 0.1f), true, -180.f, 0.f, true, 0.f, 0.004f));
		
		joints.push_back(new RevoluteJoint(mPhysicManager, bodies["12_legL1"]->GetBody(), bodies["31_torso3"]->GetBody(), b2Vec2(0.f, 0.1f), true, -110.f, 30.f, true, 0.f, 0.004f));
		joints.push_back(new RevoluteJoint(mPhysicManager, bodies["51_legR1"]->GetBody(), bodies["31_torso3"]->GetBody(), b2Vec2(0.f, 0.1f), true, -110.f, 30.f, true, 0.f, 0.004f));
		
		joints.push_back(new RevoluteJoint(mPhysicManager, bodies["11_armL1"]->GetBody(), bodies["11_armL2"]->GetBody(), b2Vec2(0.f, -0.1f), true, 0.f, 150.f, true, 0.f, 0.004f));
		joints.push_back(new RevoluteJoint(mPhysicManager, bodies["52_armR1"]->GetBody(), bodies["52_armR2"]->GetBody(), b2Vec2(0.f, -0.1f), true, 0.f, 150.f, true, 0.f, 0.004f));
		
		joints.push_back(new RevoluteJoint(mPhysicManager, bodies["12_legL1"]->GetBody(), bodies["12_legL2"]->GetBody(), b2Vec2(0.f, -0.1f), true, -90.f, 0.f, true, 0.f, 0.004f));
		joints.push_back(new RevoluteJoint(mPhysicManager, bodies["51_legR1"]->GetBody(), bodies["51_legR2"]->GetBody(), b2Vec2(0.f, -0.1f), true, -90.f, 0.f, true, 0.f, 0.004f));

		joints.push_back(new RevoluteJoint(mPhysicManager, bodies["12_legL2"]->GetBody(), bodies["13_footL"]->GetBody(), b2Vec2(-0.035f, -0.15f), true, -90.f, 0.f, true, 50.f, 0.03f));
		joints.push_back(new RevoluteJoint(mPhysicManager, bodies["51_legR2"]->GetBody(), bodies["53_footR"]->GetBody(), b2Vec2(-0.035f, -0.15f), true, -90.f, 0.f, true, 50.f, 0.03f));
	
		// Définit la force des joint
		for (auto it = bodies.begin(); it != bodies.end(); ++it)
		{
			mEntityManager.RegisterEntity(it->second);
		}
		for (auto it = joints.begin(); it != joints.end(); ++it)
		{
			(*it)->SetBreakableByForce(true);
			(*it)->SetMaxForce(80.f);
		}
	
		// Prévient l'EntityManager qu'on a ajouté des Entities
		mEntityManager.SortByLayer();
	}

	// Crée une Box parmi la liste
	void CreateBox(const b2Vec3 &posRot, const std::string &texture, int layer)
	{
		if (!mPhysicManager) return;

		// Crée le BasicBody / la Box
		BasicBody *b = new BasicBody(mPhysicManager, layer);
		b->CreateDynBox(posRot, mTextureMap[texture]);

		// L'enregistre et trie les Entities
		mEntityManager.RegisterEntity(b);
		mEntityManager.SortByLayer();
	}

	// Crée un cercle parmi la liste
	void CreateCircle(const b2Vec3 &posRot, const std::string &texture, int layer)
	{
		if (!mPhysicManager) return;

		// Crée le BasicBody / le cercle
		BasicBody *b = new BasicBody(mPhysicManager, layer);
		b->CreateDynCircle(posRot, mTextureMap[texture]);

		// L'enregistre et trie les Entities
		mEntityManager.RegisterEntity(b);
		mEntityManager.SortByLayer();
	}

	// Crée un lampadaire
	void CreateLamp(const b2Vec3 &posRot, int layer)
	{
		if (!mPhysicManager) return;

		// Crée le BasicBody / le lampadaire
		BasicBody *b = new BasicBody(mPhysicManager, layer);
		b->CreateStaticBox(posRot, mTextureMap["lampadere"], 0.1f, 0.05f);
		
		// L'enregistre et trie les Entities
		EntityManager::GetInstance().RegisterEntity(b);
		EntityManager::GetInstance().SortByLayer();
	}

} // namespace EntityFactory
