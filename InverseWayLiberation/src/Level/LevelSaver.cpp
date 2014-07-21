#include "stdafx.h"
#include "LevelSaver.h"
#include "LevelManager.h"
#include "../Lua/TriggersManager.h"
#include "../Physics/PhysicManager.h"

#include "../App/InputManager.h"
#include "../Physics/PhysicManager.h"
#include "../Resources/ResourceManager.h"

#include "../Entities/Entity.h"
#include "../Entities/Deco.h"
#include "../Entities/Player.h"
#include "../Entities/BaseBody.h"
#include "../Entities/PolyBody.h"
#include "../Entities/PolyChain.h"
#include "../Entities/BasicBody.h"
#include "../Entities/PointLight.h"
#include "../Entities/EntityManager.h"

#include "../Physics/Joint.h"
#include "../Physics/Joints/RopeJoint.h"
#include "../Physics/Joints/WeldJoint.h"
#include "../Physics/Joints/GearJoint.h"
#include "../Physics/Joints/WheelJoint.h"
#include "../Physics/Joints/PulleyJoint.h"
#include "../Physics/Joints/DistanceJoint.h"
#include "../Physics/Joints/FrictionJoint.h"
#include "../Physics/Joints/RevoluteJoint.h"
#include "../Physics/Joints/PrismaticJoint.h"

// Ctor
LevelSaver::LevelSaver(const std::string& path, bool check)
	: Saver(path, check), mLevel(LevelManager::GetInstance()), mPath(path),
	mInputManager(InputManager::GetInstance()),
	mPhysicManager(PhysicManager::GetInstance()),
	mTextureMap(ResourceManager::GetInstance().GetTextureMap())
{
	// On procède à la sauvegarde que si le fichier est valide
	if (mIsValid)
		Process();

	else
		Dialog::Error("Sauvegarde impossible :\nle fichier est invalide.");
}

// Dtor
LevelSaver::~LevelSaver(void)
{
}

// Sauvegarde tout le niveau
bool LevelSaver::Process()
{
	// Crée la déclaration XML
	mDoc.LinkEndChild(mDoc.NewDeclaration());

	// Crée la balise <level>
	mDoc.LinkEndChild(mDoc.NewElement("level"));

	/* Charge tout */
	// Le Monde
	myCheckError(ProcessWorld(), "Une erreur est survenue lors de la sauvegarde du niveau.\n"
		"La sauvegarde de World a échoué (" + mPath + ").");

	// Les textures
	myCheckError(ProcessTextures(), "Une erreur est survenue lors de la sauvegarde du niveau.\n"
		"La sauvegarde des textures a échoué (" + mPath + ").");

	// Les Bodies basiques
	myCheckError(ProcessBodies(), "Une erreur est survenue lors de la sauvegarde du niveau.\n"
		"La sauvegarde des ProcessBodies a échoué (" + mPath + ").");

	// Les Bodies polygones
	/*myCheckError(ProcessPoly(), "Une erreur est survenue lors de la sauvegarde du niveau.\n"
		"La sauvegarde des Poly a échoué (" + mPath + ").");*/

	// Les Entities
	myCheckError(ProcessEntities(), "Une erreur est survenue lors de la sauvegarde du niveau.\n"
		"La sauvegarde des Entities a échoué (" + mPath + ").");

	// Les joints
	myCheckError(ProcessJoints(), "Une erreur est survenue lors de la sauvegarde du niveau.\n"
		"La sauvegarde des joints a échoué (" + mPath + ").");

	// La déco
	myCheckError(ProcessDeco(), "Une erreur est survenue lors de la sauvegarde du niveau.\n"
		"La sauvegarde de la déco a échoué (" + mPath + ").");

	// Les actions
	myCheckError(ProcessActions(), "Une erreur est survenue lors de la sauvegarde du niveau.\n"
		"La sauvegarde des actions a échoué (" + mPath + ").");

	// Les déclencheurs
	myCheckError(ProcessTriggers(), "Une erreur est survenue lors de la sauvegarde du niveau.\n"
		"La sauvegarde des triggers a échoué (" + mPath + ").");

	// Les lumières
	myCheckError(ProcessLights(), "Une erreur est survenue lors de la sauvegarde du niveau.\n"
		"La sauvegarde des lumières a échoué (" + mPath + ").");

	// Enregistre dans le fichier
	mDoc.SaveFile(mPath.c_str());

	// On a fini
	return true;
}

// Une fonction par catégorie
bool LevelSaver::ProcessWorld()
{
	// Récupère la balise <level>
	tinyxml2::XMLHandle handle(mDoc);
	tinyxml2::XMLNode *level = handle.FirstChildElement("level").ToNode();

	// Crée la balise <world>
	tinyxml2::XMLElement *world = mDoc.NewElement("world");

	// Vue et zoom enregistrés
	//sf::Vector2f centreVue = mInputManager.GetDefaultCenter();
	sf::Vector2f centreVue = mInputManager.GetCurrentCenter();
	//float zoom = mInputManager.GetDefaultZoom();
	float zoom = mInputManager.GetCurrentZoom();

	// Crée les attributs
	world->SetAttribute("gravity", Parser::b2Vec2ToString(mPhysicManager.GetGravity()).c_str());
	world->SetAttribute("PPM", mLevel.mPhysicMgr.GetPPM());
	world->SetAttribute("bckgcolor", Parser::colorToString(mLevel.mBckgC).c_str());
	world->SetAttribute("originview", Parser::b2Vec2ToString(sf2b2Vec(centreVue, mPhysicManager.GetMPP())).c_str());
	world->SetAttribute("defaultzoom", zoom);
	
	// Ajoute <world> à <level>
	level->LinkEndChild(world);

	// Tout s'est bien passé
	return true;
}
bool LevelSaver::ProcessTextures()
{
	// Récupère la balise <level>
	tinyxml2::XMLHandle handle(mDoc);
	tinyxml2::XMLNode *level = handle.FirstChildElement("level").ToNode();

	// Crée la balise <textures>
	tinyxml2::XMLElement *textures = mDoc.NewElement("textures");
	
	// Ajoute <textures> à <level>
	level->LinkEndChild(textures);

	// Ajoute toutes les textures
	for (auto it = mLevel.mResourceManager.GetTextureMap().begin(); it != mLevel.mResourceManager.GetTextureMap().end(); ++it)
	{
		// Crée la balise <img>
		tinyxml2::XMLElement *img = mDoc.NewElement("img");

		// Modifie les attributs
		img->SetAttribute("name", (*it).second->GetName().c_str());
		img->SetAttribute("src", (*it).second->GetPath().c_str());

		// Ajoute l'<img> à <textures>
		textures->LinkEndChild(img);
	}

	// Tout s'est bien passé
	return true;
}
bool LevelSaver::ProcessBodies()
{
	// Récupère la balise <level>
	tinyxml2::XMLHandle handle(mDoc);
	tinyxml2::XMLNode *level = handle.FirstChildElement("level").ToNode();

	// Crée les balise <basicbodies>, <polybodies> et <polychains>
	tinyxml2::XMLElement *basicbodies = mDoc.NewElement("basicbodies");
	tinyxml2::XMLElement *polybodies = mDoc.NewElement("polybodies");
	tinyxml2::XMLElement *polychains = mDoc.NewElement("polychains");
	
	// Ajoute <basicbodies> et <polybodies> à <level>
	level->LinkEndChild(basicbodies);
	level->LinkEndChild(polybodies);
	level->LinkEndChild(polychains);

	// Parcours toutes les Entities
	for (auto it = mLevel.mEntityManager.GetEntities().begin(); it != mLevel.mEntityManager.GetEntities().end(); ++it)
	{
		// Vérifie le type de l'Entity
		if ((*it)->GetType() == EntityType::BasicBody || (*it)->GetType() == EntityType::PolyBody || (*it)->GetType() == EntityType::PolyChain)
		{
			BaseBody *bb = ((BaseBody*) *it);

			// Vérifie que l'Entity est créée
			if (!bb->IsCreated())
			{
				Dialog::Error("Erreur lors de la sauvegarde :\nBaseBody non créé\nBaseBody ignoré.");
				continue;
			}

			// Récupère la Forme
			std::string forme;
			if ((*it)->GetType() == EntityType::BasicBody)
			{
				BasicBody *bbtmp = ((BasicBody*) bb);

				// Forme
				if (bbtmp->GetBasicBodyShape() == BasicBody::Shape::Box)
					forme = "box";
				else if (bbtmp->GetBasicBodyShape() == BasicBody::Shape::Circle)
					forme = "circle";
				else
				{
					Dialog::Error("Erreur lors de la sauvegarde :\nBasicBody::Shape == Null\nBasicBody ignoré.");
					continue;
				}
			}
			else if ((*it)->GetType() == EntityType::PolyChain)
			{
				PolyChain *bbtmp = ((PolyChain*) bb);

				// Forme
				if (bbtmp->GetChainType() == PolyChain::Chain)
					forme = "chain";
				else if (bbtmp->GetChainType() == PolyChain::Loop)
					forme = "loop";
				else
				{
					Dialog::Error("Erreur lors de la sauvegarde :\nPolyChain::Shape == Null\nPolyChain ignoré.");
					continue;
				}

			}
			else
			{
				forme = "poly";
			}

			// Récupère le Type
			std::string type;
			if (bb->Getb2BodyType() == b2BodyType::b2_dynamicBody)
				type = "dynamic";
			else if (bb->Getb2BodyType() == b2BodyType::b2_staticBody)
				type = "static";
			else if (bb->Getb2BodyType() == b2BodyType::b2_kinematicBody)
				type = "kinematic";
			else
			{
				Dialog::Error("Erreur lors de la sauvegarde :\nBody::Type == Null\nBody ignoré.");
				continue;
			}
			
			// Récupère le nom de la texture
			std::string textureName(((Texture*) bb->GetSprite()->getTexture())->GetName());

			// Récupère l'ID
			unsigned int id = bb->GetID();

			// Récupère la Layer
			int layer = bb->GetLayer();

			// Cherche la position et la rotation
			b2Vec2 pos(bb->GetPosition());
			float rotation = bb->GetRotationD();
			if (abs(rotation - 90) < 0.05) rotation = 90.f;
			else if (abs(rotation) < 0.05) rotation = 0.f;

			// Récupère l'état
			b2Vec2 linvel = bb->GetBody()->GetLinearVelocity();
			float angvel = bb->GetBody()->GetAngularVelocity();

			// Vérifie que le Body n'a qu'une Fixture
			b2Fixture *f = bb->GetBody()->GetFixtureList();
			if (f->GetNext())
			{
				Dialog::Error("Erreur lors de la sauvegarde : \nLe BaseBody a plusieurs fixtures.");
				continue;
			}

			// Récupère les propriétés
			float density = f->GetDensity();
			float friction = f->GetFriction();
			float restitution = f->GetRestitution();

			// Récupère les propriétés de groupe
			b2Filter filter = f->GetFilterData();
			int16 groupIndex = filter.groupIndex;
			uint16 categoryBits = filter.categoryBits;
			uint16 maskBits = filter.maskBits;

			// Crée la balise <type>
			tinyxml2::XMLElement *balise = mDoc.NewElement(forme.c_str());
			if (id != 0) balise->SetAttribute("id", id);
			balise->SetAttribute("type", type.c_str());
			balise->SetAttribute("texture", textureName.c_str());
			balise->SetAttribute("pos", Parser::b2Vec2ToString(pos).c_str());
			if (rotation != 0.f) balise->SetAttribute("rotation", rotation);
			if (linvel.LengthSquared() != 0.f) balise->SetAttribute("linvel", Parser::b2Vec2ToString(linvel).c_str());
			if (angvel != 0.f) balise->SetAttribute("angvel", angvel);
			if (density != 1.f) balise->SetAttribute("density", density);
			if (friction != 0.2f) balise->SetAttribute("friction", friction);
			if (restitution != 0.f) balise->SetAttribute("restitution", restitution);
			if (layer != 1) balise->SetAttribute("layer", layer);

			if (bb->GetCollisionType() == BasicBody::CollisionType::Bullet)
				balise->SetAttribute("bullet", true);
			else if (bb->GetCollisionType() == BasicBody::CollisionType::OneSidedPlatform)
				balise->SetAttribute("osp", true);

			if (groupIndex != 0) balise->SetAttribute("groupIndex", groupIndex);
			if (categoryBits != 0x0001) balise->SetAttribute("categoryBits", categoryBits);
			if (maskBits != 0xFFFF) balise->SetAttribute("maskBits", maskBits);

			if (bb->IsActiveShadows())
				balise->SetAttribute("shadows", "true"); // Ombres

			// Ajoute la position des points pour les PolyBodies et PolyChains
			if ((*it)->GetType() == EntityType::PolyBody || (*it)->GetType() == EntityType::PolyChain)
			{
				const std::vector<b2Vec2> *pts = nullptr;
				if ((*it)->GetType() == EntityType::PolyBody)
					pts = &((PolyBody*) (*it))->GetPoints();
				else
					pts = &((PolyChain*) (*it))->GetPoints();

				for (unsigned int p = 0; p < pts->size(); ++p)
				{
					tinyxml2::XMLElement *point = mDoc.NewElement("point");
					point->SetAttribute("pos", Parser::b2Vec2ToString((*pts)[p]).c_str());
					balise->LinkEndChild(point);
				}
			}
			
			// Ajoute la balise à <basicbodies>, <polybodies> ou <polychains>
			if ((*it)->GetType() == EntityType::BasicBody)
				basicbodies->LinkEndChild(balise);
			else if((*it)->GetType() == EntityType::PolyBody)
				polybodies->LinkEndChild(balise);
			else
				polychains->LinkEndChild(balise);
		}
	}

	// Tout s'est bien passé
	return true;
}
bool LevelSaver::ProcessEntities()
{
	// Récupère la balise <level>
	tinyxml2::XMLHandle handle(mDoc);
	tinyxml2::XMLNode *level = handle.FirstChildElement("level").ToNode();

	// Crée la balise <entities>
	tinyxml2::XMLElement *entities = mDoc.NewElement("entities");
	
	// Ajoute <entities> à <level>
	level->LinkEndChild(entities);

	// Parcours toutes les Entities
	for (auto it = mLevel.mEntityManager.GetEntities().begin(); it != mLevel.mEntityManager.GetEntities().end(); ++it)
	{
		// Vérifie le type de l'Entity
		if ((*it)->GetType() == EntityType::Player)
		{
			/*Player *pp = ((Player*) *it);
			
			// Récupère le nom de la texture
			std::string textureName(((Texture*) pp->GetSprite()->getTexture())->GetName());

			// Récupère la Layer
			int layer = pp->GetLayer();

			// Cherche la position
			b2Vec2 pos(pp->GetPosition());

			// Crée la balise <livingbeing>
			tinyxml2::XMLElement *balise = mDoc.NewElement("player");
			balise->SetAttribute("position", Parser::b2Vec2ToString(pos).c_str());
			balise->SetAttribute("texture", textureName.c_str());
			balise->SetAttribute("animation", "anim/TODO");
			if (layer != 1) balise->SetAttribute("layer", layer);

			// Ajoute la balise à <entities>
			entities->LinkEndChild(balise);*/
		}
	}

	// Tout s'est bien passé
	return true;
}
bool LevelSaver::ProcessJoints()
{
	// Récupère la balise <level>
	tinyxml2::XMLHandle handle(mDoc);
	tinyxml2::XMLNode *level = handle.FirstChildElement("level").ToNode();

	// Crée la balise <joints>
	tinyxml2::XMLElement *joints = mDoc.NewElement("joints");
	
	// Ajoute <joints> à <level>
	level->LinkEndChild(joints);

	// Variable pour gérer l'enregistrement des IDs des joints
	int lastID = -1;

	// Parcours tous les Joints
	for (auto it = mLevel.mPhysicMgr.GetJointList().begin(); it != mLevel.mPhysicMgr.GetJointList().end(); ++it)
	{
		Joint *j = ((Joint*) it->second.get());

		// Si le joint est lié à un/des autres, il doit être accessible (ID)
		int id = -1;
		if (j->GetLinkedJoints().size() > 0)
		{
			// Crée une ID (donc id != -1)
			id = ++lastID;

			// Ajoute le joint dans la liste
			mJointIDMap[j->GetJoint()] = id;
		}

		// Récupère les propriétés de cassure
		bool isBreakableMaxForce = j->IsBreakableMaxForce();
		float maxForce = j->GetMaxForce();
		b2Vec2 maxVecForce = j->GetMaxVecForce();
		bool isBreakableMaxTorque = j->IsBreakableMaxTorque();
		float maxTorque = j->GetMaxTorque();

		// Récupère les propriétés du joint
		myAssert(j->GetBodyA(), "Le Joint #" + Parser::intToString(j->GetID()) + " n'a pas de bodyA.");
		myAssert(j->GetBodyB(), "Le Joint #" + Parser::intToString(j->GetID()) + " n'a pas de bodyB.");
		myAssert(j->GetBodyA()->GetUserData(), "Le Joint #" + Parser::intToString(j->GetID()) + " a un b2BodyA sans Entity.");
		myAssert(j->GetBodyB()->GetUserData(), "Le Joint #" + Parser::intToString(j->GetID()) + " a un b2BodyB sans Entity.");
		unsigned int body1 = ((Entity*)j->GetBodyA()->GetUserData())->GetID();
		unsigned int body2 = ((Entity*)j->GetBodyB()->GetUserData())->GetID();
		bool collideConnected = j->IsCollideConnected();

		// Balise XML du joint
		tinyxml2::XMLElement *balise = nullptr;

		// Gère tous les types de Joints
		if (j->GetType() == JointType::DistanceJoint)
		{
			DistanceJoint *dj = ((DistanceJoint*) j);

			// Récupère les points d'ancrage
			b2Vec2 pt1 = dj->GetRelativeAnchorA();
			b2Vec2 pt2 = dj->GetRelativeAnchorB();

			// Récupère les propriétés
			float frequency = dj->GetFrequencyHz();
			float damping = dj->GetDampingRatio();

			// Crée la balise <distance>
			balise = mDoc.NewElement("distance");
			balise->SetAttribute("body1", body1);
			balise->SetAttribute("pt1", Parser::b2Vec2ToString(pt1).c_str());
			balise->SetAttribute("body2", body2);
			balise->SetAttribute("pt2", Parser::b2Vec2ToString(pt2).c_str());
			if (frequency != 4.0f) balise->SetAttribute("frequency", frequency);
			if (damping != 0.5f) balise->SetAttribute("damping", damping);
			if (collideConnected != true) balise->SetAttribute("collision", "false");
		}
		else if (j->GetType() == JointType::FrictionJoint)
		{
			FrictionJoint *fj = ((FrictionJoint*)j);

			// Récupère les points d'ancrage
			b2Vec2 pt1 = fj->GetRelativeAnchorA();
			b2Vec2 pt2 = fj->GetRelativeAnchorB();

			// Récupère les propriétés
			float maxFrictionForce = fj->GetMaxFrictionForce();
			float maxFrictionTorque = fj->GetMaxFrictionTorque();

			// Crée la balise <friction>
			balise = mDoc.NewElement("friction");
			balise->SetAttribute("body1", body1);
			balise->SetAttribute("pt1", Parser::b2Vec2ToString(pt1).c_str());
			balise->SetAttribute("body2", body2);
			balise->SetAttribute("pt2", Parser::b2Vec2ToString(pt2).c_str());
			if (maxFrictionForce != 0.f) balise->SetAttribute("maxFrictionForce", maxFrictionForce);
			if (maxFrictionTorque != 0.f) balise->SetAttribute("maxFrictionTorque", maxFrictionTorque);
			if (collideConnected != true) balise->SetAttribute("collision", "false");
		}
		else if (j->GetType() == JointType::GearJoint)
		{
			GearJoint *gj = ((GearJoint*)j);

			// Récupère les joints liés
			if (mJointIDMap.find(gj->GetJoint1()) == mJointIDMap.end())
			{
				Joint *j1 = (Joint*)gj->GetJoint1()->GetUserData();
				myAssert(j1, "Le joint1 du GearJoint #" + Parser::intToString(gj->GetID()) + " n'est pas un Joint valide.");

				unsigned int IDj1 = j1->GetID();

				Dialog::Error("Le joint #" + Parser::uintToString(IDj1) + " est introuvable !\n"
					"Il doit être sauvegardé avant.");
				continue;
			}
			if (mJointIDMap.find(gj->GetJoint2()) == mJointIDMap.end())
			{
				Joint *j2 = (Joint*)gj->GetJoint2()->GetUserData();
				myAssert(j2, "Le joint2 du GearJoint #" + Parser::intToString(gj->GetID()) + " n'est pas un Joint valide.");

				unsigned int IDj2 = j2->GetID();

				Dialog::Error("Le joint #" + Parser::uintToString(IDj2) + " est introuvable !\n"
					"Il doit être sauvegardé avant.");
				continue;
			}
			int j1 = mJointIDMap[gj->GetJoint1()];
			int j2 = mJointIDMap[gj->GetJoint2()];

			// Récupère les propriétés
			float ratio = gj->GetRatio();

			// Crée la balise <gear>
			balise = mDoc.NewElement("gear");
			balise->SetAttribute("body1", body1);
			balise->SetAttribute("body2", body2);
			balise->SetAttribute("joint1", j1);
			balise->SetAttribute("joint2", j2);
			if (ratio != 1.f) balise->SetAttribute("ratio", ratio);
			if (collideConnected != true) balise->SetAttribute("collision", "false");
		}
		else if (j->GetType() == JointType::PrismaticJoint)
		{
			PrismaticJoint *pj = ((PrismaticJoint*)j);

			// Récupère le point d'ancrage
			b2Vec2 anchor = pj->GetAnchorRelativeToBodyA();

			// Récupère les propriétés
			b2Vec2 axis = pj->GetAxis();
			bool enableLimit = pj->IsLimitEnabled();
			float lowerTranslation = pj->GetLowerTranslation();
			float upperTranslation = pj->GetUpperTranslation();
			bool enableMotor = pj->IsMotorEnabled();
			float motorSpeed = pj->GetMotorSpeed();
			float maxMotorForce = pj->GetMaxMotorForce();

			// Crée la balise <prismatic>
			balise = mDoc.NewElement("prismatic");
			balise->SetAttribute("body1", body1);
			balise->SetAttribute("anchor", Parser::b2Vec2ToString(anchor).c_str());
			balise->SetAttribute("body2", body2);
			balise->SetAttribute("axis", Parser::b2Vec2ToString(axis).c_str());
			if (enableLimit != false) balise->SetAttribute("enableLimit", "true");
			if (lowerTranslation != 0.f) balise->SetAttribute("lower", lowerTranslation);
			if (upperTranslation != 0.f) balise->SetAttribute("upper", upperTranslation);
			if (enableMotor != false) balise->SetAttribute("enableMotor", "true");
			if (motorSpeed != 0.f) balise->SetAttribute("speed", motorSpeed);
			if (maxMotorForce != 10.f) balise->SetAttribute("maxMotorForce", maxMotorForce);
			if (collideConnected != false) balise->SetAttribute("collision", "true");
		}
		else if (j->GetType() == JointType::PulleyJoint)
		{
			PulleyJoint *pj = ((PulleyJoint*)j);

			// Récupère les points d'ancrage
			b2Vec2 pt1 = pj->GetRelativeAnchorA();
			b2Vec2 pt2 = pj->GetRelativeAnchorB();
			b2Vec2 gpt1 = pj->GetGroundAnchorA();
			b2Vec2 gpt2 = pj->GetGroundAnchorB();

			// Récupère les propriétés
			float ratio = pj->GetRatio();

			// Crée la balise <pulley>
			balise = mDoc.NewElement("pulley");
			balise->SetAttribute("body1", body1);
			balise->SetAttribute("pt1", Parser::b2Vec2ToString(pt1).c_str());
			balise->SetAttribute("body2", body2);
			balise->SetAttribute("pt2", Parser::b2Vec2ToString(pt2).c_str());
			balise->SetAttribute("groundpt1", Parser::b2Vec2ToString(gpt1).c_str());
			balise->SetAttribute("groundpt2", Parser::b2Vec2ToString(gpt2).c_str());
			if (ratio != 1.0f) balise->SetAttribute("ratio", ratio);
			if (collideConnected != true) balise->SetAttribute("collision", "false");
		}
		else if (j->GetType() == JointType::RevoluteJoint)
		{
			RevoluteJoint *rj = ((RevoluteJoint*)j);

			// Récupère le point d'ancrage
			b2Vec2 anchor = rj->GetAnchorRelativeToBodyA();

			// Récupère les propriétés
			bool enableLimit = rj->IsLimitEnabled();
			float lowerAngle = rj->GetLowerAngle();
			float upperAngle = rj->GetUpperAngle();
			bool enableMotor = rj->IsMotorEnabled();
			float motorSpeed = rj->GetMotorSpeed();
			float maxMotorTorque = rj->GetMaxMotorTorque();

			// Crée la balise <revolute>
			balise = mDoc.NewElement("revolute");
			balise->SetAttribute("body1", body1);
			balise->SetAttribute("anchor", Parser::b2Vec2ToString(anchor).c_str());
			balise->SetAttribute("body2", body2);
			if (enableLimit != false) balise->SetAttribute("enableLimit", "true");
			if (lowerAngle != 0.f) balise->SetAttribute("lower", lowerAngle);
			if (upperAngle != 0.f) balise->SetAttribute("upper", upperAngle);
			if (enableMotor != false) balise->SetAttribute("enableMotor", "true");
			if (motorSpeed != 0.f) balise->SetAttribute("speed", motorSpeed);
			if (maxMotorTorque != 10.f) balise->SetAttribute("maxMotorTorque", maxMotorTorque);
			if (collideConnected != false) balise->SetAttribute("collision", "true");
		}
		else if (j->GetType() == JointType::RopeJoint)
		{
			RopeJoint *rj = ((RopeJoint*)j);

			// Récupère les points d'ancrage
			b2Vec2 pt1 = rj->GetRelativeAnchorA();
			b2Vec2 pt2 = rj->GetRelativeAnchorB();

			// Récupère les propriétés
			float maxLength = rj->GetMaxLength();

			// Crée la balise <rope>
			balise = mDoc.NewElement("rope");
			balise->SetAttribute("body1", body1);
			balise->SetAttribute("pt1", Parser::b2Vec2ToString(pt1).c_str());
			balise->SetAttribute("body2", body2);
			balise->SetAttribute("pt2", Parser::b2Vec2ToString(pt2).c_str());
			balise->SetAttribute("maxLength", maxLength);
			if (collideConnected != true) balise->SetAttribute("collision", "false");
		}
		else if (j->GetType() == JointType::WeldJoint)
		{
			WeldJoint *wj = ((WeldJoint*)j);

			// Récupère les points d'ancrage
			b2Vec2 anchor = wj->GetAnchorRelativeToBodyA();

			// Récupère les propriétés
			float frequency = wj->GetFrequencyHz();
			float damping = wj->GetDampingRatio();

			// Crée la balise <weld>
			balise = mDoc.NewElement("weld");
			balise->SetAttribute("body1", body1);
			balise->SetAttribute("anchor", Parser::b2Vec2ToString(anchor).c_str());
			balise->SetAttribute("body2", body2);
			if (frequency != 4.0f) balise->SetAttribute("frequency", frequency);
			if (damping != 0.5f) balise->SetAttribute("damping", damping);
			if (collideConnected != false) balise->SetAttribute("collision", "true");
		}
		else if (j->GetType() == JointType::WheelJoint)
		{
			WheelJoint *wj = ((WheelJoint*)j);

			// Récupère les points d'ancrage
			b2Vec2 anchor = wj->GetAnchorRelativeToBodyA();

			// Récupère les propriétés
			b2Vec2 axis = wj->GetAxis();
			float frequency = wj->GetFrequencyHz();
			float damping = wj->GetDampingRatio();
			bool enableMotor = wj->IsMotorEnabled();
			float motorSpeed = wj->GetMotorSpeed();
			float maxMotorTorque = wj->GetMaxMotorTorque();

			// Crée la balise <wheel>
			balise = mDoc.NewElement("wheel");
			balise->SetAttribute("body1", body1);
			balise->SetAttribute("anchor", Parser::b2Vec2ToString(anchor).c_str());
			balise->SetAttribute("body2", body2);
			balise->SetAttribute("axis", Parser::b2Vec2ToString(axis).c_str());
			if (frequency != 4.0f) balise->SetAttribute("frequency", frequency);
			if (damping != 0.5f) balise->SetAttribute("damping", damping);
			if (enableMotor != false) balise->SetAttribute("enableMotor", "true");
			if (motorSpeed != 0.f) balise->SetAttribute("speed", motorSpeed);
			if (maxMotorTorque != 10.f) balise->SetAttribute("maxMotorForce", maxMotorTorque);
			if (collideConnected != false) balise->SetAttribute("collision", "true");
		}

		// Si la balise a été créée
		if (balise)
		{
			// ID
			if (id >= 0) balise->SetAttribute("id", id);

			// Propriétés communes des joints
			if (isBreakableMaxForce != false) balise->SetAttribute("isBreakableMaxForce", "true");
			if (j->GetMaxForceType() == ForceType::Float && maxForce != 0.f) balise->SetAttribute("maxForce", maxForce);
			if (j->GetMaxForceType() == ForceType::Vector && maxVecForce.LengthSquared() != 0) balise->SetAttribute("maxVecForce", maxForce);
			if (isBreakableMaxTorque != false) balise->SetAttribute("isBreakableMaxTorque", "true");
			if (maxTorque != 0.f) balise->SetAttribute("maxTorque", maxTorque);

			// On ajoute la balise à <joints>
			joints->LinkEndChild(balise);
		}
	}

	// Tout s'est bien passé
	return true;
}
bool LevelSaver::ProcessDeco()
{
	// Récupère la balise <level>
	tinyxml2::XMLHandle handle(mDoc);
	tinyxml2::XMLNode *level = handle.FirstChildElement("level").ToNode();

	// Crée la balise <deco>
	tinyxml2::XMLElement *deco = mDoc.NewElement("deco");
	
	// Ajoute <deco> à <level>
	level->LinkEndChild(deco);

	// Layer de la déco
	bool firstDeco = true;
	int lastLayer = 0;
	tinyxml2::XMLElement *layer = nullptr;
	
	// Parcours toutes les Entities
	for (auto it = mLevel.mEntityManager.GetEntities().begin(); it != mLevel.mEntityManager.GetEntities().end(); ++it)
	{
		// Vérifie le type de l'Entity
		if ((*it)->GetType() == EntityType::Deco)
		{
			Deco *d = ((Deco*) *it);

			// Récupère le layer
			if (lastLayer != d->GetLayer() || firstDeco)
			{
				lastLayer = d->GetLayer();
				firstDeco = false;
				layer = nullptr;
			}

			// Si c'est un nouveau layer, on crée sa balise
			if (!layer)
			{
				layer = mDoc.NewElement("layer");
				layer->SetAttribute("z", lastLayer);

				// Ajoute la balise à <deco>
				deco->LinkEndChild(layer);
			}
			
			// Récupère le nom de la texture
			std::string textureName(((Texture*) d->GetSprite()->getTexture())->GetName());

			// Cherche la position
			b2Vec2 pos(d->GetPosition());
			float rotation = d->GetRotationD();
			if (abs(rotation - 90) < 0.5) rotation = 90.f;
			else if (abs(rotation) < 0.5) rotation = 0.f;

			// Crée la balise <img>
			tinyxml2::XMLElement *balise = mDoc.NewElement("img");
			balise->SetAttribute("texture", textureName.c_str());
			balise->SetAttribute("position", Parser::b2Vec2ToString(pos).c_str());
			if (rotation != 0.f) balise->SetAttribute("rotation", rotation);
			if (d->IsActiveShadows())
				balise->SetAttribute("shadows", "true"); // Ombres

			// Ajoute la balise à <layer>
			layer->LinkEndChild(balise);
		}
	}

	// Tout s'est bien passé
	return true;
}
bool LevelSaver::ProcessActions()
{
	// Récupère la balise <level>
	tinyxml2::XMLHandle handle(mDoc);
	tinyxml2::XMLNode *level = handle.FirstChildElement("level").ToNode();

	// Crée la balise <actions>
	tinyxml2::XMLElement *actions = mDoc.NewElement("actions");

	// Ajoute <actions> à <level>
	level->LinkEndChild(actions);

	// Ajoute toutes les actions
	for (auto it = mLevel.mTriggersManager.GetActionMap().begin(); it != mLevel.mTriggersManager.GetActionMap().end(); ++it)
	{
		// L'action est un fichier
		if (!it->second->HasFunction())
		{
			// Crée la balise <file>
			tinyxml2::XMLElement *file = mDoc.NewElement("file");

			// Crée les attributs
			file->SetAttribute("name", it->second->GetName().c_str());
			file->SetAttribute("path", it->second->GetFile().c_str());

			// Ajoute le <file> à <actions>
			actions->LinkEndChild(file);
		}
		// L'action est une fonction
		else
		{
			// Crée la balise <function>
			tinyxml2::XMLElement *function = mDoc.NewElement("function");

			// Crée les attributs
			function->SetAttribute("name", it->second->GetName().c_str());
			function->SetAttribute("func", it->second->GetFunction().c_str());
			function->SetAttribute("file", it->second->GetFile().c_str());

			// Ajoute la <function> à <actions>
			actions->LinkEndChild(function);
		}
	}

	// Tout s'est bien passé
	return true;
}
bool LevelSaver::ProcessTriggers()
{
	// Récupère la balise <level>
	tinyxml2::XMLHandle handle(mDoc);
	tinyxml2::XMLNode *level = handle.FirstChildElement("level").ToNode();

	// Crée la balise <events>
	tinyxml2::XMLElement *events = mDoc.NewElement("events");
	level->LinkEndChild(events);

	// Crée la balise <triggers>
	tinyxml2::XMLElement *triggers = mDoc.NewElement("triggers");
	events->LinkEndChild(triggers);

	// Ajoute toutes les actions
	for (auto it = mLevel.mTriggersManager.GetAreas().begin(); it != mLevel.mTriggersManager.GetAreas().end(); ++it)
	{
		// Crée la balise <area>
		tinyxml2::XMLElement *area = mDoc.NewElement("area");

		// Crée les attributs
		area->SetAttribute("top", it->first.upperBound.y);
		area->SetAttribute("left", it->first.lowerBound.x);
		area->SetAttribute("bottom", it->first.lowerBound.y);
		area->SetAttribute("right", it->first.upperBound.x);
		area->SetAttribute("action", it->second.c_str());

		// Ajoute l'<area> à <triggers>
		triggers->LinkEndChild(area);
	}

	// Tout s'est bien passé
	return true;
}
bool LevelSaver::ProcessLights()
{
	// Récupère la balise <level>
	tinyxml2::XMLHandle handle(mDoc);
	tinyxml2::XMLNode *level = handle.FirstChildElement("level").ToNode();

	// Crée la balise <lights>
	tinyxml2::XMLElement *lights = mDoc.NewElement("lights");

	// Ajoute <lights> à <level>
	level->LinkEndChild(lights);

	// Parcours toutes les Entities à la recherche de lumières
	for (auto it = mLevel.mEntityManager.GetEntities().begin(); it != mLevel.mEntityManager.GetEntities().end(); ++it)
	{
		// Vérifie le type de l'Entity
		if ((*it)->GetType() == EntityType::PointLight)
		{
			// Récupère la lumière
			PointLight *l = ((PointLight*) *it);

			// Récupère les propriétés
			b2Vec2 pos = l->GetPosition();
			sf::Color color = l->GetLightColor();
			unsigned int radius = l->GetLightRadius();
			int layer = l->GetLayer();

			// Crée la balise <point>
			tinyxml2::XMLElement *balise = mDoc.NewElement("point");
			balise->SetAttribute("layer", layer);
			balise->SetAttribute("pos", Parser::b2Vec2ToString(pos).c_str());
			balise->SetAttribute("radius", radius);
			balise->SetAttribute("color", Parser::colorToString(color).c_str());

			// Ajoute la balise à <layer>
			lights->LinkEndChild(balise);
		}
	}

	// Tout s'est bien passé
	return true;
}