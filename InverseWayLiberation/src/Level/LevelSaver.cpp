#include "LevelSaver.h"
#include "../Tools/utils.h"
#include "../Tools/Error.h"
#include "../Tools/Dialog.h"
#include "../Tools/Parser.h"
#include "../Lua/TriggersManager.h"
#include "../Physics/PhysicManager.h"

#include "../Entities/Entity.h"
#include "../Entities/Deco.h"
#include "../Entities/Player.h"
#include "../Entities/BasicBody.h"
#include "../Entities/LivingBeing.h"
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
LevelSaver::LevelSaver(const LevelManager &level, const std::string& path)
	: Saver(path), mLevel(level), mPath(path),
	mInputManager(InputManager::GetInstance()), mPhysicManager(PhysicManager::GetInstance()),
	mTextureMap(ResourceManager::GetInstance().GetTextureMap())
{
	// On proc�de � la sauvegarde que si le fichier est valide
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
	// Cr�e la d�claration XML
	mDoc.LinkEndChild(mDoc.NewDeclaration());

	// Cr�e la balise <level>
	mDoc.LinkEndChild(mDoc.NewElement("level"));

	// Enregistre tout
	if (!ProcessWorld()) return false;
	if (!ProcessTextures()) return false;
	if (!ProcessBasicBodies()) return false;
	if (!ProcessEntities()) return false;
	if (!ProcessJoints()) return false;
	if (!ProcessDeco()) return false;
	if (!ProcessActions()) return false;
	if (!ProcessTriggers()) return false;

	// Enregistre dans le fichier
	mDoc.SaveFile(mPath.c_str());

	// On a fini
	return true;
}

// Une fonction par cat�gorie
bool LevelSaver::ProcessWorld()
{
	// R�cup�re la balise <level>
	tinyxml2::XMLHandle handle(mDoc);
	tinyxml2::XMLNode *level = handle.FirstChildElement("level").ToNode();

	// Cr�e la balise <world>
	tinyxml2::XMLElement *world = mDoc.NewElement("world");

	// Vue et zoom enregistr�s
	//sf::Vector2f centreVue = mInputManager.GetDefaultCenter();
	sf::Vector2f centreVue = mInputManager.GetCurrentCenter();
	//float zoom = mInputManager.GetDefaultZoom();
	float zoom = mInputManager.GetCurrentZoom();

	// Cr�e les attributs
	world->SetAttribute("gravity", Parser::b2Vec2ToString(mPhysicManager.GetGravity()).c_str());
	world->SetAttribute("PPM", mLevel.mPhysicMgr.GetPPM());
	world->SetAttribute("bckgcolor", Parser::colorToString(mLevel.mBckgC).c_str());
	world->SetAttribute("originview", Parser::b2Vec2ToString(sf2b2Vec(centreVue, mPhysicManager.GetMPP())).c_str());
	world->SetAttribute("defaultzoom", zoom);
	
	// Ajoute <world> � <level>
	level->LinkEndChild(world);

	// Tout s'est bien pass�
	return true;
}
bool LevelSaver::ProcessTextures()
{
	// R�cup�re la balise <level>
	tinyxml2::XMLHandle handle(mDoc);
	tinyxml2::XMLNode *level = handle.FirstChildElement("level").ToNode();

	// Cr�e la balise <textures>
	tinyxml2::XMLElement *textures = mDoc.NewElement("textures");
	
	// Ajoute <textures> � <level>
	level->LinkEndChild(textures);

	// Ajoute toutes les textures
	for (auto it = mLevel.mResourceManager.GetTextureMap().begin(); it != mLevel.mResourceManager.GetTextureMap().end(); ++it)
	{
		// Cr�e la balise <img>
		tinyxml2::XMLElement *img = mDoc.NewElement("img");

		// Modifie les attributs
		img->SetAttribute("name", (*it).second->GetName().c_str());
		img->SetAttribute("src", (*it).second->GetPath().c_str());

		// Ajoute l'<img> � <textures>
		textures->LinkEndChild(img);
	}

	// Tout s'est bien pass�
	return true;
}
bool LevelSaver::ProcessBasicBodies()
{
	// R�cup�re la balise <level>
	tinyxml2::XMLHandle handle(mDoc);
	tinyxml2::XMLNode *level = handle.FirstChildElement("level").ToNode();

	// Cr�e la balise <basicbodies>
	tinyxml2::XMLElement *basicbodies = mDoc.NewElement("basicbodies");
	
	// Ajoute <basicbodies> � <level>
	level->LinkEndChild(basicbodies);

	// Parcours toutes les Entities
	for (auto it = mLevel.mEntityManager.GetEntities().begin(); it != mLevel.mEntityManager.GetEntities().end(); ++it)
	{
		// V�rifie le type de l'Entity
		if ((*it)->GetType() == EntityType::BasicBody)
		{
			BasicBody *bb = ((BasicBody*) *it);

			// D�finit le nom de la balise
			std::string type;
			if (bb->Getb2BodyType() == b2BodyType::b2_dynamicBody && bb->GetBasicBodyShape() == BasicBody::Shape::Box)
				type = "dynamicbox";
			else if (bb->Getb2BodyType() == b2BodyType::b2_dynamicBody && bb->GetBasicBodyShape() == BasicBody::Shape::Circle)
				type = "dynamiccircle";
			else if (bb->Getb2BodyType() == b2BodyType::b2_staticBody && bb->GetBasicBodyShape() == BasicBody::Shape::Box)
				type = "staticbox";
			else if (bb->Getb2BodyType() == b2BodyType::b2_staticBody && bb->GetBasicBodyShape() == BasicBody::Shape::Circle)
				type = "staticcircle";
			else
			{
				Dialog::Error("Erreur lors de la sauvegarde :\nBasicBody::Type == Null\nBasicBody ignor�.");
				continue;
			}
			
			// R�cup�re le nom de la texture
			std::string textureName(((Texture*) bb->GetSprite()->getTexture())->GetName());

			// R�cup�re l'ID
			unsigned int id = bb->GetID();

			// R�cup�re la Layer
			int layer = bb->GetLayer();

			// Cherche la position et la rotation
			b2Vec2 pos(bb->GetPosition());
			float rotation = bb->GetRotationD();
			if (abs(rotation - 90) < 0.05) rotation = 90.f;
			else if (abs(rotation) < 0.05) rotation = 0.f;

			// R�cup�re l'�tat
			b2Vec2 linvel = bb->GetBody()->GetLinearVelocity();
			float angvel = bb->GetBody()->GetAngularVelocity();

			// V�rifie que le BasicBody n'a qu'une Fixture
			b2Fixture *f = bb->GetBody()->GetFixtureList();
			if (f->GetNext())
			{
				Dialog::Error("Erreur lors de la sauvegarde : \nLe BasicBody a plusieurs fixtures.");
				continue;
			}

			// R�cup�re les propri�t�s
			float density = f->GetDensity();
			float friction = f->GetFriction();
			float restitution = f->GetRestitution();

			// R�cup�re les propri�t�s de groupe
			b2Filter filter = f->GetFilterData();
			int16 groupIndex = filter.groupIndex;
			uint16 categoryBits = filter.categoryBits;
			uint16 maskBits = filter.maskBits;

			// Cr�e la balise <name>
			tinyxml2::XMLElement *balise = mDoc.NewElement(type.c_str());
			if (id != 0) balise->SetAttribute("id", id);
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

			// Ajoute la balise � <basicbodies>
			basicbodies->LinkEndChild(balise);
		}
	}

	// Tout s'est bien pass�
	return true;
}
bool LevelSaver::ProcessEntities()
{
	// R�cup�re la balise <level>
	tinyxml2::XMLHandle handle(mDoc);
	tinyxml2::XMLNode *level = handle.FirstChildElement("level").ToNode();

	// Cr�e la balise <entities>
	tinyxml2::XMLElement *entities = mDoc.NewElement("entities");
	
	// Ajoute <entities> � <level>
	level->LinkEndChild(entities);

	// Parcours toutes les Entities
	for (auto it = mLevel.mEntityManager.GetEntities().begin(); it != mLevel.mEntityManager.GetEntities().end(); ++it)
	{
		// V�rifie le type de l'Entity
		if ((*it)->GetType() == EntityType::LivingBeing)
		{
			LivingBeing *lb = ((LivingBeing*) *it);
			
			// R�cup�re le nom de la texture
			std::string textureName(((Texture*) lb->GetSprite()->getTexture())->GetName());

			// R�cup�re la Layer
			int layer = lb->GetLayer();

			// Cherche la position
			b2Vec2 pos(lb->GetPosition());

			// Cr�e la balise <livingbeing>
			tinyxml2::XMLElement *balise = mDoc.NewElement("livingbeing");
			balise->SetAttribute("position", Parser::b2Vec2ToString(pos).c_str());
			balise->SetAttribute("texture", textureName.c_str());
			balise->SetAttribute("animation", "anim/TODO");
			if (layer != 1) balise->SetAttribute("layer", layer);

			// Ajoute la balise � <entities>
			entities->LinkEndChild(balise);
		}
		else if ((*it)->GetType() == EntityType::Player)
		{
			Player *pp = ((Player*) *it);
			
			// R�cup�re le nom de la texture
			std::string textureName(((Texture*) pp->GetSprite()->getTexture())->GetName());

			// R�cup�re la Layer
			int layer = pp->GetLayer();

			// Cherche la position
			b2Vec2 pos(pp->GetPosition());

			// Cr�e la balise <livingbeing>
			tinyxml2::XMLElement *balise = mDoc.NewElement("player");
			balise->SetAttribute("position", Parser::b2Vec2ToString(pos).c_str());
			balise->SetAttribute("texture", textureName.c_str());
			balise->SetAttribute("animation", "anim/TODO");
			if (layer != 1) balise->SetAttribute("layer", layer);

			// Ajoute la balise � <entities>
			entities->LinkEndChild(balise);
		}
	}

	// Tout s'est bien pass�
	return true;
}
bool LevelSaver::ProcessJoints()
{
	// R�cup�re la balise <level>
	tinyxml2::XMLHandle handle(mDoc);
	tinyxml2::XMLNode *level = handle.FirstChildElement("level").ToNode();

	// Cr�e la balise <joints>
	tinyxml2::XMLElement *joints = mDoc.NewElement("joints");
	
	// Ajoute <joints> � <level>
	level->LinkEndChild(joints);

	// Variable pour g�rer l'enregistrement des IDs des joints
	int lastID = -1;

	// Parcours tous les Joints
	for (auto it = mLevel.mPhysicMgr.GetJointList().begin(); it != mLevel.mPhysicMgr.GetJointList().end(); ++it)
	{
		Joint *j = ((Joint*) it->second.get());

		// Si le joint est li� � un/des autres, il doit �tre accessible (ID)
		int id = -1;
		if (j->GetLinkedJoints().size() > 0)
		{
			// Cr�e une ID (donc id != -1)
			id = ++lastID;

			// Ajoute le joint dans la liste
			mJointIDMap[j->GetJoint()] = id;
		}

		// R�cup�re les propri�t�s de cassure
		bool isBreakableMaxForce = j->IsBreakableMaxForce();
		float maxForce = j->GetMaxForce();
		b2Vec2 maxVecForce = j->GetMaxVecForce();
		bool isBreakableMaxTorque = j->IsBreakableMaxTorque();
		float maxTorque = j->GetMaxTorque();

		// R�cup�re les propri�t�s du joint
		myAssert(j->GetBodyA(), "Le Joint #" + Parser::intToString(j->GetID()) + " n'a pas de bodyA.");
		myAssert(j->GetBodyB(), "Le Joint #" + Parser::intToString(j->GetID()) + " n'a pas de bodyB.");
		myAssert(j->GetBodyA()->GetUserData(), "Le Joint #" + Parser::intToString(j->GetID()) + " a un b2BodyA sans Entity.");
		myAssert(j->GetBodyB()->GetUserData(), "Le Joint #" + Parser::intToString(j->GetID()) + " a un b2BodyB sans Entity.");
		unsigned int body1 = ((Entity*)j->GetBodyA()->GetUserData())->GetID();
		unsigned int body2 = ((Entity*)j->GetBodyB()->GetUserData())->GetID();
		bool collideConnected = j->IsCollideConnected();

		// Balise XML du joint
		tinyxml2::XMLElement *balise = nullptr;

		// G�re tous les types de Joints
		if (j->GetType() == JointType::DistanceJoint)
		{
			DistanceJoint *dj = ((DistanceJoint*) j);

			// R�cup�re les points d'ancrage
			b2Vec2 pt1 = dj->GetRelativeAnchorA();
			b2Vec2 pt2 = dj->GetRelativeAnchorB();

			// R�cup�re les propri�t�s
			float frequency = dj->GetFrequencyHz();
			float damping = dj->GetDampingRatio();

			// Cr�e la balise <distance>
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

			// R�cup�re les points d'ancrage
			b2Vec2 pt1 = fj->GetRelativeAnchorA();
			b2Vec2 pt2 = fj->GetRelativeAnchorB();

			// R�cup�re les propri�t�s
			float maxFrictionForce = fj->GetMaxFrictionForce();
			float maxFrictionTorque = fj->GetMaxFrictionTorque();

			// Cr�e la balise <friction>
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

			// R�cup�re les joints li�s
			if (mJointIDMap.find(gj->GetJoint1()) == mJointIDMap.end())
			{
				Joint *j1 = (Joint*)gj->GetJoint1()->GetUserData();
				myAssert(j1, "Le joint1 du GearJoint #" + Parser::intToString(gj->GetID()) + " n'est pas un Joint valide.");

				unsigned int IDj1 = j1->GetID();

				Dialog::Error("Le joint #" + Parser::uintToString(IDj1) + " est introuvable !\n"
					"Il doit �tre sauvegard� avant.");
				continue;
			}
			if (mJointIDMap.find(gj->GetJoint2()) == mJointIDMap.end())
			{
				Joint *j2 = (Joint*)gj->GetJoint2()->GetUserData();
				myAssert(j2, "Le joint2 du GearJoint #" + Parser::intToString(gj->GetID()) + " n'est pas un Joint valide.");

				unsigned int IDj2 = j2->GetID();

				Dialog::Error("Le joint #" + Parser::uintToString(IDj2) + " est introuvable !\n"
					"Il doit �tre sauvegard� avant.");
				continue;
			}
			int j1 = mJointIDMap[gj->GetJoint1()];
			int j2 = mJointIDMap[gj->GetJoint2()];

			// R�cup�re les propri�t�s
			float ratio = gj->GetRatio();

			// Cr�e la balise <gear>
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

			// R�cup�re le point d'ancrage
			b2Vec2 anchor = pj->GetAnchorRelativeToBodyA();

			// R�cup�re les propri�t�s
			b2Vec2 axis = pj->GetAxis();
			bool enableLimit = pj->IsLimitEnabled();
			float lowerTranslation = pj->GetLowerTranslation();
			float upperTranslation = pj->GetUpperTranslation();
			bool enableMotor = pj->IsMotorEnabled();
			float motorSpeed = pj->GetMotorSpeed();
			float maxMotorForce = pj->GetMaxMotorForce();

			// Cr�e la balise <prismatic>
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

			// R�cup�re les points d'ancrage
			b2Vec2 pt1 = pj->GetRelativeAnchorA();
			b2Vec2 pt2 = pj->GetRelativeAnchorB();
			b2Vec2 gpt1 = pj->GetGroundAnchorA();
			b2Vec2 gpt2 = pj->GetGroundAnchorB();

			// R�cup�re les propri�t�s
			float ratio = pj->GetRatio();

			// Cr�e la balise <pulley>
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

			// R�cup�re le point d'ancrage
			b2Vec2 anchor = rj->GetAnchorRelativeToBodyA();

			// R�cup�re les propri�t�s
			bool enableLimit = rj->IsLimitEnabled();
			float lowerAngle = rj->GetLowerAngle();
			float upperAngle = rj->GetUpperAngle();
			bool enableMotor = rj->IsMotorEnabled();
			float motorSpeed = rj->GetMotorSpeed();
			float maxMotorTorque = rj->GetMaxMotorTorque();

			// Cr�e la balise <revolute>
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

			// R�cup�re les points d'ancrage
			b2Vec2 pt1 = rj->GetRelativeAnchorA();
			b2Vec2 pt2 = rj->GetRelativeAnchorB();

			// R�cup�re les propri�t�s
			float maxLength = rj->GetMaxLength();

			// Cr�e la balise <rope>
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

			// R�cup�re les points d'ancrage
			b2Vec2 anchor = wj->GetAnchorRelativeToBodyA();

			// R�cup�re les propri�t�s
			float frequency = wj->GetFrequencyHz();
			float damping = wj->GetDampingRatio();

			// Cr�e la balise <weld>
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

			// R�cup�re les points d'ancrage
			b2Vec2 anchor = wj->GetAnchorRelativeToBodyA();

			// R�cup�re les propri�t�s
			b2Vec2 axis = wj->GetAxis();
			float frequency = wj->GetFrequencyHz();
			float damping = wj->GetDampingRatio();
			bool enableMotor = wj->IsMotorEnabled();
			float motorSpeed = wj->GetMotorSpeed();
			float maxMotorTorque = wj->GetMaxMotorTorque();

			// Cr�e la balise <wheel>
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

		// Si la balise a �t� cr��e
		if (balise)
		{
			// ID
			if (id >= 0) balise->SetAttribute("id", id);

			// Propri�t�s communes des joints
			if (isBreakableMaxForce != false) balise->SetAttribute("isBreakableMaxForce", "true");
			if (j->GetMaxForceType() == ForceType::Float && maxForce != 0.f) balise->SetAttribute("maxForce", maxForce);
			if (j->GetMaxForceType() == ForceType::Vector && maxVecForce.LengthSquared() != 0) balise->SetAttribute("maxVecForce", maxForce);
			if (isBreakableMaxTorque != false) balise->SetAttribute("isBreakableMaxTorque", "true");
			if (maxTorque != 0.f) balise->SetAttribute("maxTorque", maxTorque);

			// On ajoute la balise � <joints>
			joints->LinkEndChild(balise);
		}
	}

	// Tout s'est bien pass�
	return true;
}
bool LevelSaver::ProcessDeco()
{
	// R�cup�re la balise <level>
	tinyxml2::XMLHandle handle(mDoc);
	tinyxml2::XMLNode *level = handle.FirstChildElement("level").ToNode();

	// Cr�e la balise <deco>
	tinyxml2::XMLElement *deco = mDoc.NewElement("deco");
	
	// Ajoute <deco> � <level>
	level->LinkEndChild(deco);

	// Layer de la d�co
	bool firstDeco = true;
	int lastLayer = 0;
	tinyxml2::XMLElement *layer = nullptr;
	
	// Parcours toutes les Entities
	for (auto it = mLevel.mEntityManager.GetEntities().begin(); it != mLevel.mEntityManager.GetEntities().end(); ++it)
	{
		// V�rifie le type de l'Entity
		if ((*it)->GetType() == EntityType::Deco)
		{
			Deco *d = ((Deco*) *it);

			// R�cup�re le layer
			if (lastLayer != d->GetLayer() || firstDeco)
			{
				lastLayer = d->GetLayer();
				firstDeco = false;
				layer = nullptr;
			}

			// Si c'est un nouveau layer, on cr�e sa balise
			if (!layer)
			{
				layer = mDoc.NewElement("layer");
				layer->SetAttribute("z", lastLayer);

				// Ajoute la balise � <deco>
				deco->LinkEndChild(layer);
			}
			
			// R�cup�re le nom de la texture
			std::string textureName(((Texture*) d->GetSprite()->getTexture())->GetName());

			// Cherche la position
			b2Vec2 pos(d->GetPosition());
			float rotation = d->GetRotation();
			if (abs(rotation - 90) < 0.5) rotation = 90.f;
			else if (abs(rotation) < 0.5) rotation = 0.f;

			// Cr�e la balise <img>
			tinyxml2::XMLElement *balise = mDoc.NewElement("img");
			balise->SetAttribute("texture", textureName.c_str());
			balise->SetAttribute("position", Parser::b2Vec2ToString(pos).c_str());
			if (rotation != 0.f) balise->SetAttribute("rotation", rotation);

			// Ajoute la balise � <layer>
			layer->LinkEndChild(balise);
		}
	}

	// Tout s'est bien pass�
	return true;
}
bool LevelSaver::ProcessActions()
{
	// R�cup�re la balise <level>
	tinyxml2::XMLHandle handle(mDoc);
	tinyxml2::XMLNode *level = handle.FirstChildElement("level").ToNode();

	// Cr�e la balise <actions>
	tinyxml2::XMLElement *actions = mDoc.NewElement("actions");

	// Ajoute <actions> � <level>
	level->LinkEndChild(actions);

	// Ajoute toutes les actions
	for (auto it = mLevel.mTriggersManager.GetActionMap().begin(); it != mLevel.mTriggersManager.GetActionMap().end(); ++it)
	{
		// L'action est un fichier
		if (!it->second->HasFunction())
		{
			// Cr�e la balise <file>
			tinyxml2::XMLElement *file = mDoc.NewElement("file");

			// Cr�e les attributs
			file->SetAttribute("name", it->second->GetName().c_str());
			file->SetAttribute("path", it->second->GetFile().c_str());

			// Ajoute le <file> � <actions>
			actions->LinkEndChild(file);
		}
		// L'action est une fonction
		else
		{
			// Cr�e la balise <function>
			tinyxml2::XMLElement *function = mDoc.NewElement("function");

			// Cr�e les attributs
			function->SetAttribute("name", it->second->GetName().c_str());
			function->SetAttribute("func", it->second->GetFunction().c_str());
			function->SetAttribute("file", it->second->GetFile().c_str());

			// Ajoute la <function> � <actions>
			actions->LinkEndChild(function);
		}
	}

	// Tout s'est bien pass�
	return true;
}
bool LevelSaver::ProcessTriggers()
{
	// R�cup�re la balise <level>
	tinyxml2::XMLHandle handle(mDoc);
	tinyxml2::XMLNode *level = handle.FirstChildElement("level").ToNode();

	// Cr�e la balise <events>
	tinyxml2::XMLElement *events = mDoc.NewElement("events");
	level->LinkEndChild(events);

	// Cr�e la balise <triggers>
	tinyxml2::XMLElement *triggers = mDoc.NewElement("triggers");
	events->LinkEndChild(triggers);

	// Ajoute toutes les actions
	for (auto it = mLevel.mTriggersManager.GetAreas().begin(); it != mLevel.mTriggersManager.GetAreas().end(); ++it)
	{
		// Cr�e la balise <area>
		tinyxml2::XMLElement *area = mDoc.NewElement("area");

		// Cr�e les attributs
		area->SetAttribute("top", it->first.upperBound.y);
		area->SetAttribute("left", it->first.lowerBound.x);
		area->SetAttribute("bottom", it->first.lowerBound.y);
		area->SetAttribute("right", it->first.upperBound.x);
		area->SetAttribute("action", it->second.c_str());

		// Ajoute l'<area> � <triggers>
		triggers->LinkEndChild(area);
	}

	// Tout s'est bien pass�
	return true;
}
