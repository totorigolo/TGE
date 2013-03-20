#include "EntityFactory.h"
#include "../Resources/ResourceManager.h"
#include "EntityManager.h"
#include "BasicBody.h"
#include "../Physics/Joints/RevoluteJoint.h"
#include "../Physics/Joints/WeldJoint.h"
#include "../Physics/Joint.h"
#include "../Tools/utils.h"
#include <cassert>
#include <iostream>
#include <list>
#include <map>

namespace EntityFactory
{
	// Variables privés
	namespace
	{
		// Entities
		EntityManager &mEntityManager(EntityManager::GetInstance());

		// Resources
		ResourceManager &mResourceManager(ResourceManager::GetInstance());
		TextureMap &mTextureMap(mResourceManager.GetTextureMap());
	}

	// Crée un Ragdoll
	void CreateRagdoll(PhysicManager *physicMgr, const b2Vec2 &position, int layer)
	{
		assert(physicMgr && "n'est pas valide");

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
		bodies["11_armL1"] = new BasicBody(physicMgr, layer);
		bodies["11_armL1"]->CreateDynBox(b2Vec3(0.f, 0.f, 0.f) + position, mTextureMap["hero_armL1"], 1.f, 0.2f, 0.f, -1);
		bodies["11_armL2"] = new BasicBody(physicMgr, layer);
		bodies["11_armL2"]->CreateDynBox(b2Vec3(0.f, -0.2f, 0.f) + position, mTextureMap["hero_armL2"], 1.f, 0.2f, 0.f, -1);
		bodies["12_legL1"] = new BasicBody(physicMgr, layer);
		bodies["12_legL1"]->CreateDynBox(b2Vec3(0.f, -0.3f, 0.f) + position, mTextureMap["hero_legL1"], 1.f, 0.2f, 0.f, -1);
		bodies["12_legL2"] = new BasicBody(physicMgr, layer);
		bodies["12_legL2"]->CreateDynBox(b2Vec3(0.f, -0.6f, 0.f) + position, mTextureMap["hero_legL2"], 1.f, 0.2f, 0.f, -1);
		bodies["13_footL"] = new BasicBody(physicMgr, layer);
		bodies["13_footL"]->CreateDynBox(b2Vec3(0.05f, -0.75f, 0.f) + position, mTextureMap["hero_footL"], 1.f, 0.2f, 0.f, -1);

		// Cou
		bodies["20_neck"] = new BasicBody(physicMgr, layer);
		bodies["20_neck"]->CreateDynBox(b2Vec3(0.f, 0.25f, 0.f) + position, mTextureMap["hero_neck"], 1.f, 0.2f, 0.f, -1);

		// Torse
		bodies["31_torso3"] = new BasicBody(physicMgr, layer);
		bodies["31_torso3"]->CreateDynBox(b2Vec3(0.f, -0.1f, 0.f) + position, mTextureMap["hero_torso3"], 1.f, 0.2f, 0.f, -1);
		bodies["32_torso2"] = new BasicBody(physicMgr, layer);
		bodies["32_torso2"]->CreateDynBox(b2Vec3(0.f, 0.f, 0.f) + position, mTextureMap["hero_torso2"], 1.f, 0.2f, 0.f, -1);
		bodies["33_torso1"] = new BasicBody(physicMgr, layer);
		bodies["33_torso1"]->CreateDynBox(b2Vec3(0.f, 0.15f, 0.f) + position, mTextureMap["hero_torso1"], 1.f, 0.2f, 0.f, -1);
	
		// Tête
		bodies["40_head"] = new BasicBody(physicMgr, layer);
		bodies["40_head"]->CreateDynCircle(b2Vec3(0.f, 0.5f, 0.f) + position, mTextureMap["hero_head"], 1.f, 0.5f, 0.f, -1);

		// Jambe, bras et pied droit
		bodies["51_legR1"] = new BasicBody(physicMgr, layer);
		bodies["51_legR1"]->CreateDynBox(b2Vec3(0.f, -0.3f, 0.f) + position, mTextureMap["hero_legR1"], 1.f, 0.2f, 0.f, -1);
		bodies["51_legR2"] = new BasicBody(physicMgr, layer);
		bodies["51_legR2"]->CreateDynBox(b2Vec3(0.f, -0.6f, 0.f) + position, mTextureMap["hero_legR2"], 1.f, 0.2f, 0.f, -1);
		bodies["52_armR1"] = new BasicBody(physicMgr, layer);
		bodies["52_armR1"]->CreateDynBox(b2Vec3(0.f, 0.f, 0.f) + position, mTextureMap["hero_armR1"], 1.f, 0.2f, 0.f, -1);
		bodies["52_armR2"] = new BasicBody(physicMgr, layer);
		bodies["52_armR2"]->CreateDynBox(b2Vec3(0.f, -0.2f, 0.f) + position, mTextureMap["hero_armR2"], 1.f, 0.2f, 0.f, -1);
		bodies["53_footR"] = new BasicBody(physicMgr, layer);
		bodies["53_footR"]->CreateDynBox(b2Vec3(0.05f, -0.75f, 0.f) + position, mTextureMap["hero_footR"], 1.f, 0.2f, 0.f, -1);

		/* Jointe les bodies entre eux */
		std::list<Joint*> joints;
		joints.push_back(new RevoluteJoint(physicMgr, bodies["40_head"]->GetBody(), bodies["33_torso1"]->GetBody(), b2Vec2(0.f, -0.15f), true, -30.f, 50.f, false, 0.f, 0.004f));
		joints.push_back(new WeldJoint(physicMgr, bodies["20_neck"]->GetBody(), bodies["33_torso1"]->GetBody(), b2Vec2(0.f, -0.05f), 0.f, 0.f));
		
		joints.push_back(new RevoluteJoint(physicMgr, bodies["33_torso1"]->GetBody(), bodies["32_torso2"]->GetBody(), b2Vec2(0.f, -0.1f), true, -10.f, 10.f, true, 0.f, 0.004f));
		joints.push_back(new RevoluteJoint(physicMgr, bodies["32_torso2"]->GetBody(), bodies["31_torso3"]->GetBody(), b2Vec2(0.f, -0.1f), true, -10.f, 10.f, true, 0.f, 0.004f));
		
		joints.push_back(new RevoluteJoint(physicMgr, bodies["11_armL1"]->GetBody(), bodies["33_torso1"]->GetBody(), b2Vec2(0.f, 0.1f), true, -180.f, 0.f, true, 0.f, 0.004f));
		joints.push_back(new RevoluteJoint(physicMgr, bodies["52_armR1"]->GetBody(), bodies["33_torso1"]->GetBody(), b2Vec2(0.f, 0.1f), true, -180.f, 0.f, true, 0.f, 0.004f));
		
		joints.push_back(new RevoluteJoint(physicMgr, bodies["12_legL1"]->GetBody(), bodies["31_torso3"]->GetBody(), b2Vec2(0.f, 0.1f), true, -110.f, 30.f, true, 0.f, 0.004f));
		joints.push_back(new RevoluteJoint(physicMgr, bodies["51_legR1"]->GetBody(), bodies["31_torso3"]->GetBody(), b2Vec2(0.f, 0.1f), true, -110.f, 30.f, true, 0.f, 0.004f));
		
		joints.push_back(new RevoluteJoint(physicMgr, bodies["11_armL1"]->GetBody(), bodies["11_armL2"]->GetBody(), b2Vec2(0.f, -0.1f), true, 0.f, 150.f, true, 0.f, 0.004f));
		joints.push_back(new RevoluteJoint(physicMgr, bodies["52_armR1"]->GetBody(), bodies["52_armR2"]->GetBody(), b2Vec2(0.f, -0.1f), true, 0.f, 150.f, true, 0.f, 0.004f));
		
		joints.push_back(new RevoluteJoint(physicMgr, bodies["12_legL1"]->GetBody(), bodies["12_legL2"]->GetBody(), b2Vec2(0.f, -0.1f), true, -90.f, 0.f, true, 0.f, 0.004f));
		joints.push_back(new RevoluteJoint(physicMgr, bodies["51_legR1"]->GetBody(), bodies["51_legR2"]->GetBody(), b2Vec2(0.f, -0.1f), true, -90.f, 0.f, true, 0.f, 0.004f));

		joints.push_back(new RevoluteJoint(physicMgr, bodies["12_legL2"]->GetBody(), bodies["13_footL"]->GetBody(), b2Vec2(-0.035f, -0.15f), true, -90.f, 0.f, true, 50.f, 0.03f));
		joints.push_back(new RevoluteJoint(physicMgr, bodies["51_legR2"]->GetBody(), bodies["53_footR"]->GetBody(), b2Vec2(-0.035f, -0.15f), true, -90.f, 0.f, true, 50.f, 0.03f));
	
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
	void CreateBox(PhysicManager *physicMgr, const b2Vec3 &posRot, std::string list[], int list_size, int layer)
	{
		// Crée le BasicBody / la Box
		BasicBody *b = new BasicBody(physicMgr, layer);
		b->CreateDynBox(posRot, mTextureMap[randomElement(list, list_size)]);

		// L'enregistre et trie les Entities
		mEntityManager.RegisterEntity(b);
		mEntityManager.SortByLayer();
	}

	// Crée un cercle parmi la liste
	void CreateCircle(PhysicManager *physicMgr, const b2Vec3 &posRot, std::string list[], int list_size, int layer)
	{
		// Crée le BasicBody / le cercle
		BasicBody *b = new BasicBody(physicMgr, layer);
		b->CreateDynCircle(posRot, mTextureMap[randomElement(list, list_size)]);

		// L'enregistre et trie les Entities
		mEntityManager.RegisterEntity(b);
		mEntityManager.SortByLayer();
	}

	// Crée un lampadaire
	void CreateLamp(PhysicManager *physicMgr, const b2Vec3 &posRot, int layer)
	{
		// Crée le BasicBody / le lampadaire
		BasicBody *b = new BasicBody(physicMgr, layer);
		b->CreateStaticBox(posRot, mTextureMap["lampadere"], 0.1f, 0.05f);
		
		// L'enregistre et trie les Entities
		EntityManager::GetInstance().RegisterEntity(b);
		EntityManager::GetInstance().SortByLayer();

		// Crée la lumière
		//sf::Vector2f pos = mCurrentMousePosRV;
		//pos.y -= 1.2f * mPhysicMgr.GetPPM();
		//mLightManager.AddLight(new PointLight(pos, 100.2f, true, true));
	}

} // namespace EntityFactory
