#include "EntityFactory.h"
#include "../Resources/ResourceManager.h"
#include "../Physics/Joints/RevoluteJoint.h"
#include "../Physics/Joints/WeldJoint.h"
#include "../Physics/PhysicManager.h"
#include "../Physics/Joint.h"
#include "EntityManager.h"
#include "BasicBody.h"
#include "PolyBody.h"
#include "PolyChain.h"
#include "Deco.h"
#include "PointLight.h"
#include "Hum.h"
#include "../Tools/utils.h"

namespace EntityFactory
{
	// Variables privées
	namespace
	{
		// Physique
		PhysicManager &mPhysicManager(PhysicManager::GetInstance());

		// Entities
		EntityManager &mEntityManager(EntityManager::GetInstance());

		// Resources
		ResourceManager &mResourceManager(ResourceManager::GetInstance());
	}
	
	// Crée une déco
	void CreateDeco(const b2Vec3 &posRot, const std::string &texture, int layer)
	{
		// Crée la déco
		new Deco(layer, mResourceManager.GetTexture(texture), b32sfVec(posRot, mPhysicManager.GetPPM()));

		// Trie les Entities
		mEntityManager.SortByLayer();
	}

	// Crée un BasicBody : Box
	void CreateBox(const b2Vec3 &posRot, b2BodyType type, const std::string &texture, int layer)
	{
		// Crée le BasicBody / la Box
		BasicBody *b = new BasicBody(layer);
		b->CreateBox(posRot, type, mResourceManager.GetTexture(texture));

		// Trie les Entities
		mEntityManager.SortByLayer();
	}

	// Crée un BasicBody : Circle
	void CreateCircle(const b2Vec3 &posRot, b2BodyType type, const std::string &texture, int layer)
	{
		// Crée le BasicBody / le cercle
		BasicBody *b = new BasicBody(layer);
		b->CreateCircle(posRot, type, mResourceManager.GetTexture(texture));

		// Trie les Entities
		mEntityManager.SortByLayer();
	}

	// Crée un PolyBody
	void CreatePolyBody(const vector_b2Vec2 &vectices, b2BodyType type, const std::string &texture, int layer)
	{
		// Crée le PolyBody
		PolyBody *b = new PolyBody(layer);
		b->Create(vectices.GetVector(), type, mResourceManager.GetTexture(texture));

		// Trie les Entities
		mEntityManager.SortByLayer();
	}

	// Crée un PolyChain
	void CreatePolyChain(const vector_b2Vec2 &vectices, PolyChain::Type type, const std::string &texture, int layer)
	{
		// Crée le PolyBody
		PolyChain *b = new PolyChain(layer);
		b->Create(vectices.GetVector(), type, mResourceManager.GetTexture(texture));

		// Trie les Entities
		mEntityManager.SortByLayer();
	}

	// Crée une PointLight
	void CreatePointLight(const b2Vec2 &pos, int lightRadius, sf::Color lightColor, int layer)
	{
		// Crée la PointLight
		PointLight *l = new PointLight(lightRadius, lightColor, layer);
		l->SetPosition(pos);

		// Trie les Entities
		mEntityManager.SortByLayer();
	}

	// Crée un Ragdoll
	void CreateRagdoll(const b2Vec2 &position, int layer)
	{
		// Charge les textures
		mResourceManager.LoadTexture("hero_head", "tex/hero/head.png");
		mResourceManager.LoadTexture("hero_neck", "tex/hero/neck.png");
		mResourceManager.LoadTexture("hero_torso1", "tex/hero/torso1.png");
		mResourceManager.LoadTexture("hero_torso2", "tex/hero/torso2.png");
		mResourceManager.LoadTexture("hero_torso3", "tex/hero/torso3.png");
		mResourceManager.LoadTexture("hero_armL1", "tex/hero/armL1.png");
		mResourceManager.LoadTexture("hero_armL2", "tex/hero/armL2.png");
		mResourceManager.LoadTexture("hero_armR1", "tex/hero/armR1.png");
		mResourceManager.LoadTexture("hero_armR2", "tex/hero/armR2.png");
		mResourceManager.LoadTexture("hero_legL1", "tex/hero/legL1.png");
		mResourceManager.LoadTexture("hero_legL2", "tex/hero/legL2.png");
		mResourceManager.LoadTexture("hero_legR1", "tex/hero/legR1.png");
		mResourceManager.LoadTexture("hero_legR2", "tex/hero/legR2.png");
		mResourceManager.LoadTexture("hero_footL", "tex/hero/footL.png");
		mResourceManager.LoadTexture("hero_footR", "tex/hero/footR.png");

		/* Crée les bodies (ordre d'apparition inverse) */
		// std::map pour les stocker temporairement
		std::map<std::string, BasicBody*> bodies;

		// Variables tmp pour raccourcir
		b2BodyType td = b2BodyType::b2_dynamicBody;

		// Jambe, bras et pied gauche
		bodies["11_armL1"] = new BasicBody(layer);
		bodies["11_armL1"]->CreateBox(b2Vec3(0.f, 0.f, 0.f) + position, td, mResourceManager.GetTexture("hero_armL1"), 1.f, 0.2f, 0.f, -1);
		bodies["11_armL2"] = new BasicBody(layer);
		bodies["11_armL2"]->CreateBox(b2Vec3(0.f, -0.2f, 0.f) + position, td, mResourceManager.GetTexture("hero_armL2"), 1.f, 0.2f, 0.f, -1);
		bodies["12_legL1"] = new BasicBody(layer);
		bodies["12_legL1"]->CreateBox(b2Vec3(0.f, -0.3f, 0.f) + position, td, mResourceManager.GetTexture("hero_legL1"), 1.f, 0.2f, 0.f, -1);
		bodies["12_legL2"] = new BasicBody(layer);
		bodies["12_legL2"]->CreateBox(b2Vec3(0.f, -0.6f, 0.f) + position, td, mResourceManager.GetTexture("hero_legL2"), 1.f, 0.2f, 0.f, -1);
		bodies["13_footL"] = new BasicBody(layer);
		bodies["13_footL"]->CreateBox(b2Vec3(0.05f, -0.75f, 0.f) + position, td, mResourceManager.GetTexture("hero_footL"), 1.f, 0.2f, 0.f, -1);

		// Cou
		bodies["20_neck"] = new BasicBody(layer);
		bodies["20_neck"]->CreateBox(b2Vec3(0.f, 0.25f, 0.f) + position, td, mResourceManager.GetTexture("hero_neck"), 1.f, 0.2f, 0.f, -1);

		// Torse
		bodies["31_torso3"] = new BasicBody(layer);
		bodies["31_torso3"]->CreateBox(b2Vec3(0.f, -0.1f, 0.f) + position, td, mResourceManager.GetTexture("hero_torso3"), 1.f, 0.2f, 0.f, -1);
		bodies["32_torso2"] = new BasicBody(layer);
		bodies["32_torso2"]->CreateBox(b2Vec3(0.f, 0.f, 0.f) + position, td, mResourceManager.GetTexture("hero_torso2"), 1.f, 0.2f, 0.f, -1);
		bodies["33_torso1"] = new BasicBody(layer);
		bodies["33_torso1"]->CreateBox(b2Vec3(0.f, 0.15f, 0.f) + position, td, mResourceManager.GetTexture("hero_torso1"), 1.f, 0.2f, 0.f, -1);
	
		// Tête
		bodies["40_head"] = new BasicBody(layer);
		bodies["40_head"]->CreateCircle(b2Vec3(0.f, 0.5f, 0.f) + position, td, mResourceManager.GetTexture("hero_head"), 1.f, 0.5f, 0.f, -1);

		// Jambe, bras et pied droit
		bodies["51_legR1"] = new BasicBody(layer);
		bodies["51_legR1"]->CreateBox(b2Vec3(0.f, -0.3f, 0.f) + position, td, mResourceManager.GetTexture("hero_legR1"), 1.f, 0.2f, 0.f, -1);
		bodies["51_legR2"] = new BasicBody(layer);
		bodies["51_legR2"]->CreateBox(b2Vec3(0.f, -0.6f, 0.f) + position, td, mResourceManager.GetTexture("hero_legR2"), 1.f, 0.2f, 0.f, -1);
		bodies["52_armR1"] = new BasicBody(layer);
		bodies["52_armR1"]->CreateBox(b2Vec3(0.f, 0.f, 0.f) + position, td, mResourceManager.GetTexture("hero_armR1"), 1.f, 0.2f, 0.f, -1);
		bodies["52_armR2"] = new BasicBody(layer);
		bodies["52_armR2"]->CreateBox(b2Vec3(0.f, -0.2f, 0.f) + position, td, mResourceManager.GetTexture("hero_armR2"), 1.f, 0.2f, 0.f, -1);
		bodies["53_footR"] = new BasicBody(layer);
		bodies["53_footR"]->CreateBox(b2Vec3(0.05f, -0.75f, 0.f) + position, td, mResourceManager.GetTexture("hero_footR"), 1.f, 0.2f, 0.f, -1);

		/* Jointe les bodies entre eux */
		// Liste des joints
		std::list<Joint*> joints;

		// typedef pour raccourcir
		typedef WeldJoint wj;
		typedef WeldJointDef wjd;
		typedef RevoluteJoint rj;
		typedef RevoluteJointDef rjd;

		// Création des joints
		joints.push_back(new rj(rjd(bodies["40_head"]->GetBody(), bodies["33_torso1"]->GetBody(), b2Vec2(0.f, -0.15f), true, -30.f, 50.f, false, 0.f, 0.004f)));
		joints.push_back(new wj(wjd(bodies["20_neck"]->GetBody(), bodies["33_torso1"]->GetBody(), b2Vec2(0.f, -0.05f), 0.f, 0.f)));
		
		joints.push_back(new rj(rjd(bodies["33_torso1"]->GetBody(), bodies["32_torso2"]->GetBody(), b2Vec2(0.f, -0.1f), true, -10.f, 10.f, true, 0.f, 0.004f)));
		joints.push_back(new rj(rjd(bodies["32_torso2"]->GetBody(), bodies["31_torso3"]->GetBody(), b2Vec2(0.f, -0.1f), true, -10.f, 10.f, true, 0.f, 0.004f)));
		
		joints.push_back(new rj(rjd(bodies["11_armL1"]->GetBody(), bodies["33_torso1"]->GetBody(), b2Vec2(0.f, 0.1f), true, -180.f, 0.f, true, 0.f, 0.004f)));
		joints.push_back(new rj(rjd(bodies["52_armR1"]->GetBody(), bodies["33_torso1"]->GetBody(), b2Vec2(0.f, 0.1f), true, -180.f, 0.f, true, 0.f, 0.004f)));
		
		joints.push_back(new rj(rjd(bodies["12_legL1"]->GetBody(), bodies["31_torso3"]->GetBody(), b2Vec2(0.f, 0.1f), true, -110.f, 30.f, true, 0.f, 0.004f)));
		joints.push_back(new rj(rjd(bodies["51_legR1"]->GetBody(), bodies["31_torso3"]->GetBody(), b2Vec2(0.f, 0.1f), true, -110.f, 30.f, true, 0.f, 0.004f)));
		
		joints.push_back(new rj(rjd(bodies["11_armL1"]->GetBody(), bodies["11_armL2"]->GetBody(), b2Vec2(0.f, -0.1f), true, 0.f, 150.f, true, 0.f, 0.004f)));
		joints.push_back(new rj(rjd(bodies["52_armR1"]->GetBody(), bodies["52_armR2"]->GetBody(), b2Vec2(0.f, -0.1f), true, 0.f, 150.f, true, 0.f, 0.004f)));
		
		joints.push_back(new rj(rjd(bodies["12_legL1"]->GetBody(), bodies["12_legL2"]->GetBody(), b2Vec2(0.f, -0.1f), true, -90.f, 0.f, true, 0.f, 0.004f)));
		joints.push_back(new rj(rjd(bodies["51_legR1"]->GetBody(), bodies["51_legR2"]->GetBody(), b2Vec2(0.f, -0.1f), true, -90.f, 0.f, true, 0.f, 0.004f)));

		joints.push_back(new rj(rjd(bodies["12_legL2"]->GetBody(), bodies["13_footL"]->GetBody(), b2Vec2(-0.035f, -0.15f), true, -90.f, 0.f, true, 50.f, 0.03f)));
		joints.push_back(new rj(rjd(bodies["51_legR2"]->GetBody(), bodies["53_footR"]->GetBody(), b2Vec2(-0.035f, -0.15f), true, -90.f, 0.f, true, 50.f, 0.03f)));
	
		// Définit la force des joint
		for (auto it = joints.begin(); it != joints.end(); ++it)
		{
			(*it)->SetBreakableByForce(true);
			(*it)->SetMaxForce(80.f);
		}
	
		// Prévient l'EntityManager qu'on a ajouté des Entities
		mEntityManager.SortByLayer();
	}

	// Crée un Hum
	void CreateHum(b2Vec2 pos, double age, double strengh, sf::Color color, int layer)
	{
		// Crée le Hum
		Hum *h = new Hum(layer);
		h->Create(pos, age, strengh, color);

		// Trie les Entities
		mEntityManager.SortByLayer();
	}

} // namespace EntityFactory
