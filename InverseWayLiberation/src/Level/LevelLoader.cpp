#include "LevelLoader.h"
#include "LevelManager.h"

#include "../Tools/utils.h"
#include "../Tools/Error.h"
#include "../Tools/Dialog.h"
#include "../Tools/Parser.h"

#include "../Game/InputManager.h"
#include "../Physics/PhysicManager.h"

#include "../Physics/Joints/RopeJoint.h"
#include "../Physics/Joints/WeldJoint.h"
#include "../Physics/Joints/GearJoint.h"
#include "../Physics/Joints/WheelJoint.h"
#include "../Physics/Joints/PulleyJoint.h"
#include "../Physics/Joints/DistanceJoint.h"
#include "../Physics/Joints/FrictionJoint.h"
#include "../Physics/Joints/RevoluteJoint.h"
#include "../Physics/Joints/PrismaticJoint.h"

#include "../Entities/Entity.h"
#include "../Entities/Deco.h"
#include "../Entities/Player.h"
#include "../Entities/BasicBody.h"
#include "../Entities/LivingBeing.h"
#include "../Entities/EntityFactory.h"
#include "../Entities/EntityManager.h"

#include <Thor/Resources.hpp>

// Ctor
LevelLoader::LevelLoader(const std::string& path)
	: Loader(path),
	mLevel(LevelManager::GetInstance()),
	mInputManager(InputManager::GetInstance()),
	mPhysicManager(PhysicManager::GetInstance())
{
	// Si loader n'est pas valide, on ne fait rien
	if (mIsValid)
	{
		// Vide le niveau
		mLevel.Clear();

		// Analyse le fichier du niveau et cr�e le Level
		mLevel.mIsCharged = Process();
		mLevel.PrepareForGame();
	}
}

// Dtor
LevelLoader::~LevelLoader(void)
{
}

// Analyse tout le fichier
bool LevelLoader::Process()
{
	// V�rifie que <level> existe dans le fichier
	tinyxml2::XMLHandle hdl(mFile);
	tinyxml2::XMLElement *elem = hdl.FirstChildElement("level").ToElement();
	myCheckError(elem, "Une erreur est survenue lors du chargement du niveau.\n"
					   "<level> n'existe pas, fichier invalide (" + mPath + ").");

	/* Charge tout */
	// Le Monde
	myCheckError(ProcessWorld(), "Une erreur est survenue lors du chargement du niveau.\n"
								 "Le chargement de World a �chou� (" + mPath + ").");

	// Les textures
	myCheckError(ProcessTextures(), "Une erreur est survenue lors du chargement du niveau.\n"
									"Le chargement des textures a �chou� (" + mPath + ").");

	// Les Bodies basiques
	myCheckError(ProcessBasicBodies(), "Une erreur est survenue lors du chargement du niveau.\n"
									   "Le chargement des BasicBodies a �chou� (" + mPath + ").");

	// Les Entities
	myCheckError(ProcessEntities(), "Une erreur est survenue lors du chargement du niveau.\n"
									"Le chargement des Entities a �chou� (" + mPath + ").");

	// Les joints
	myCheckError(ProcessJoints(), "Une erreur est survenue lors du chargement du niveau.\n"
								  "Le chargement des joints a �chou� (" + mPath + ").");

	// La d�co
	myCheckError(ProcessDeco(), "Une erreur est survenue lors du chargement du niveau.\n"
									"Le chargement de la d�co a �chou� (" + mPath + ").");

	// Les actions
	myCheckError(ProcessActions(), "Une erreur est survenue lors du chargement du niveau.\n"
									"Le chargement des actions a �chou� (" + mPath + ").");

	// Les d�clencheurs
	myCheckError(ProcessTriggers(), "Une erreur est survenue lors du chargement du niveau.\n"
									"Le chargement des triggers a �chou� (" + mPath + ").");

	return true;
}

// Une fonction par cat�gorie
bool LevelLoader::ProcessWorld()
{
	// R�cup�re <world>
	tinyxml2::XMLHandle hdl(mFile);
	tinyxml2::XMLElement *world = hdl.FirstChildElement("level").FirstChildElement("world").ToElement();

	// Valeurs par d�faut
	b2Vec2 gravity(0.f, -9.8f);
	float PPM = 100.f;
	sf::Color bckgColor = sf::Color::White;
	b2Vec2 originView(0.f, 0.f);
	float defaultZoom = 1.f;

	// R�cup�re les valeurs
	if (world)
	{
		if (world->Attribute("gravity")) gravity = Parser::string2b2Vec2(world->Attribute("gravity"));
		world->QueryFloatAttribute("PPM", &PPM);
		if (world->Attribute("bckgcolor")) bckgColor = Parser::string2color(world->Attribute("bckgcolor"));
		if (world->Attribute("originview")) originView = Parser::string2b2Vec2(world->Attribute("originview"));
		world->QueryFloatAttribute("defaultzoom", &defaultZoom);
	}

	// Si le monde n'est pas d�fini, on averti
	else
	{
		Dialog::Information("Les monde n'a pas �t� d�fini dans le niveau.\nLes valeurs par d�faut ont �t� utilis�es.");
	}

	// Change les attributs
	mLevel.mBckgC = bckgColor;
	mPhysicManager.SetGravity(gravity);
	mPhysicManager.SetPPM(PPM);
	mInputManager.SetDefaultZoom(defaultZoom);
	mInputManager.SetDefaultCenter(b22sfVec(originView, mPhysicManager.GetPPM()));

	return true;
}
bool LevelLoader::ProcessTextures()
{
	// R�cup�re <textures>
	tinyxml2::XMLHandle hdl(mFile);
	tinyxml2::XMLHandle textures = hdl.FirstChildElement("level").FirstChildElement("textures");

	// V�rifie que <textures> existe
	myCheckError(textures.ToElement(), "Une erreur est survenue lors du chargement du niveau.\n<textures> non trouv� (" + mPath + ").");
	
	// On cr�e les attributs
	std::string path, name;

	// Pour toutes les textures
	tinyxml2::XMLElement *texture = textures.FirstChildElement().ToElement();
	while (texture)
	{
		// R�cup�re les �l�ments
		if (texture->Attribute("name")) name = texture->Attribute("name");
		if (texture->Attribute("src")) path = texture->Attribute("src");

		// Charge la texture
		try {
			mLevel.mTextureMap[name] = mLevel.mResourceManager.acquire(thor::Resources::fromFile<sf::Texture>(path));
		}
		catch (thor::ResourceLoadingException const& e)
		{
			Dialog::Error("Erreur lors du chargement d'une texture du niveau !\n" + std::string(e.what()), true);
		}

		// On r�cup�re la prochaine texture
		texture = texture->NextSiblingElement();
	}

	return true;
}
bool LevelLoader::ProcessBasicBodies()
{
	// R�cup�re <basicbodies>
	tinyxml2::XMLHandle hdl(mFile);
	tinyxml2::XMLHandle bodies = hdl.FirstChildElement("level").FirstChildElement("basicbodies");

	// V�rifie que <basicbodies> existe
	myCheckError(bodies.ToElement(), "Une erreur est survenue lors du chargement du niveau.\n<basicbodies> non trouv� (" + mPath + ").");

	// On cr�e les attributs
	BasicBody *bb = nullptr;
	int layer = 1;
	bool hasID = false;
	bool bullet = false, osp = false;
	float density = 1.f, friction = 0.2f, restitution = 0.0f;
	float rotation = 0.f;
	unsigned int id = 0U;
	std::string texture, type;
	b2Vec3 posRot;

	// Pour tous les bodies
	tinyxml2::XMLElement *body = bodies.FirstChildElement().ToElement();
	while (body)
	{
		// R�initialise les attributs
		bb = nullptr;
		layer = 1;
		id = 0U;
		rotation = 0.f;
		density = 1.f;
		friction = 0.2f;
		restitution = 0.0f;
		hasID = false;
		osp = false;
		bullet = false;

		// R�cup�re le type
		type = body->Name();

		// R�cup�re la texture et v�rifie si elle existe
		if (body->Attribute("texture")) texture = body->Attribute("texture");
		if (mLevel.mTextureMap.find(texture) == mLevel.mTextureMap.end())
		{
			Dialog::Error("Texture \"" + texture + "\" inconnue. Body ignor� !");

			// On passe au prochain body
			body = body->NextSiblingElement();
			continue;
		}

		// R�cup�re l'ID
		if (body->Attribute("id"))
		{
			body->QueryUnsignedAttribute("id", &id);
			hasID = true;
		}

		// R�cup�re la position et la rotation
		body->QueryFloatAttribute("rotation", &rotation);
		if (body->Attribute("pos")) posRot = Parser::string2b2Vec3(body->Attribute("pos"));
		posRot.z = rotation;

		// R�cup�re le layer
		body->QueryIntAttribute("layer", &layer);

		// R�cup�re les propri�t�s
		body->QueryFloatAttribute("density", &density);
		body->QueryFloatAttribute("friction", &friction);
		body->QueryFloatAttribute("restitution", &restitution);

		// Cr�e le body
		if (type == "staticbox")
		{
			bb = new BasicBody(layer);
			bb->CreateStaticBox(posRot, mLevel.mTextureMap[texture], friction, restitution);
		}
		else if (type == "dynamicbox")
		{
			bb = new BasicBody(layer);
			bb->CreateDynBox(posRot, mLevel.mTextureMap[texture], density, friction, restitution);
		}
		else if (type == "dynamiccircle")
		{
			bb = new BasicBody(layer);
			bb->CreateDynCircle(posRot, mLevel.mTextureMap[texture], density, friction, restitution);
		}
		else
		{
			Dialog::Error("BasicBody type inconnu ("+ type +") !");
		}

		// Si le BasicBody a �t� cr��
		if (bb)
		{
			// Cr�e l'Entity correspondante
			mLevel.mEntityManager.RegisterEntity(bb);

			// Propri�t�s de collision
			body->QueryBoolAttribute("osp", &osp);
			body->QueryBoolAttribute("bullet", &bullet);

			// Applique les propri�t�s
			if (osp && bullet)
				Dialog::Error("Impossible d'avoir osp et bullet en m�me temps !");
			else if (osp)
				bb->SetCollisionType(BasicBody::CollisionType::OneSidedPlatform);
			else if (bullet)
				bb->SetCollisionType(BasicBody::CollisionType::Bullet);

			// Enregiste l'ID
			if (hasID)
			{
				// On regarde si l'ID n'est pas d�j� utilis�
				if (mBodyIDMap.find(id) != mBodyIDMap.end())
					Dialog::Error("L'ID " + Parser::int2string(id) + " n'est pas unique !", false);
				mBodyIDMap[id] = bb->GetBody();
			}
		}

		// On r�cup�re le prochain body
		body = body->NextSiblingElement();
	}

	return true;
}
bool LevelLoader::ProcessEntities()
{
	// R�cup�re <entities>
	tinyxml2::XMLHandle hdl(mFile);
	tinyxml2::XMLHandle entities = hdl.FirstChildElement("level").FirstChildElement("entities");

	// V�rifie que <entities> existe
	myCheckError(entities.ToElement(), "Une erreur est survenue lors du chargement du niveau.\n<entities> non trouv� (" + mPath + ").");

	// On cr�e les attributs
	Entity *e = nullptr;
	int layer = 1;
	b2Vec2 position;
	std::string type, animation;
	
	// Pour toutes les Entities
	tinyxml2::XMLElement *entity = entities.FirstChildElement().ToElement();
	while (entity)
	{
		// R�initialise les attributs
		e = nullptr;
		layer = 1;
		type = "";
		animation = "";
		position = b2Vec2_zero;

		// R�cup�re le type
		type = entity->Name();

		// R�cup�re la position et la rotation
		if (entity->Attribute("position")) position = Parser::string2b2Vec2(entity->Attribute("position"));
		
		// R�cup�re le layer
		entity->QueryIntAttribute("layer", &layer);

		// Cr�e l'Entity
		if (type == "ragdoll")
		{
			EntityFactory::CreateRagdoll(position, layer);
		}
		else if (type == "livingbeing")
		{
			// R�cup�re l'animation
			animation = entity->Attribute("animation");
			animation = entity->Attribute("texture");

			e = new LivingBeing(position, mLevel.mTextureMap[animation], layer);
		}
		else if (type == "player")
		{
			// R�cup�re l'animation
			animation = entity->Attribute("animation");
			animation = entity->Attribute("texture");

			// Enregistre le Player dans Level
			e = new Player(position, mLevel.mTextureMap[animation], layer);
			mLevel.mPlayer = (Player*) e;
		}
		else
		{
			Dialog::Error("Entity type inconnu ("+ type +") !");
		}

		// Cr�e l'Entity correspondante
		if (e)
			mLevel.mEntityManager.RegisterEntity(e);

		// On r�cup�re la prochaine entity
		entity = entity->NextSiblingElement();
	}

	return true;
}
bool LevelLoader::ProcessJoints()
{
	// R�cup�re <joints>
	tinyxml2::XMLHandle hdl(mFile);
	tinyxml2::XMLHandle joints = hdl.FirstChildElement("level").FirstChildElement("joints");

	// V�rifie que <joints> existe
	if (!joints.ToElement())
	{
		// Il n'y a tout simplement pas de joint...
		return true;
	}

	// On cr�e les attributs
	Joint *j = nullptr;
	bool hasID = false;
	unsigned int id = 0U;
	std::string type;
	unsigned int IDb1 = 0U, IDb2 = 0U;
	b2Body *b1 = nullptr, *b2 = nullptr;
	b2Vec2 pt1, pt2, anchor;
	bool collision = true;

	// Pour tous les joints
	tinyxml2::XMLElement *joint = joints.FirstChildElement().ToElement();
	while (joint)
	{
		// R�initialise les attributs
		j = nullptr;
		hasID = false;
		id = 0U;
		IDb1 = 0U, IDb2 = 0U;
		b1 = nullptr, b2 = nullptr;
		collision = true;

		// R�cup�re l'ID
		if (joint->Attribute("id"))
		{
			joint->QueryUnsignedAttribute("id", &id);
			hasID = true;
		}

		// R�cup�re le type
		type = joint->Name();

		// Change la valeur de collision par d�faut
		if (type == "weld" || type == "prismatic" || type == "revolute" || type == "wheel")
		{
			collision = false;
		}

		// R�cup�re les attributs suivant le type
		joint->QueryUnsignedAttribute("body1", &IDb1);
		joint->QueryUnsignedAttribute("body2", &IDb2);
		if (joint->Attribute("pt1")) pt1 = Parser::string2b2Vec2(joint->Attribute("pt1"));
		if (joint->Attribute("pt2")) pt2 = Parser::string2b2Vec2(joint->Attribute("pt2"));
		if (joint->Attribute("anchor")) anchor = Parser::string2b2Vec2(joint->Attribute("anchor"));
		joint->QueryBoolAttribute("collision", &collision);
		
		// R�cup�re les bodies
		if (mBodyIDMap.find(IDb1) == mBodyIDMap.end())
		{
			Dialog::Error("Le body #"+ Parser::uint2string(IDb1) +" est introuvable !");
			continue;
		}
		if (mBodyIDMap.find(IDb2) == mBodyIDMap.end())
		{
			Dialog::Error("Le body #"+ Parser::uint2string(IDb1) +" est introuvable !");
			continue;
		}
		b1 = mBodyIDMap[IDb1];
		b2 = mBodyIDMap[IDb2];

		// Cr�e le joint
		if (type == "gear")
		{
			b2Joint *j1 = nullptr, *j2 = nullptr;
			unsigned int IDj1 = 0U, IDj2 = 0U;
			GearJointDef def;
			def.body1 = b1;
			def.body2 = b2;
			def.collideconnected = collision;

			// R�cup�re les attributs
			joint->QueryUnsignedAttribute("joint1", &IDj1);
			joint->QueryUnsignedAttribute("joint2", &IDj2);
			joint->QueryFloatAttribute("ratio", &def.ratio);

			// R�cup�re les joints
			if (mJointIDMap.find(IDj1) == mJointIDMap.end())
			{
				Dialog::Error("Le joint #"+ Parser::uint2string(IDj1) +" est introuvable !\n"
							  "Il doit �tre construit avant.");
				continue;
			}
			if (mJointIDMap.find(IDj2) == mJointIDMap.end())
			{
				Dialog::Error("Le joint #"+ Parser::uint2string(IDj2) +" est introuvable !\n"
							  "Il doit �tre construit avant.");
				continue;
			}
			j1 = mJointIDMap[IDj1];
			j2 = mJointIDMap[IDj2];

			// V�rifie qu'il soient bien construits
			myAssert(j1->GetUserData(), "Le joint n'existe pas.");
			myAssert(j2->GetUserData(), "Le joint n'existe pas.");

			// Cr�e le joint
			def.joint1 = ((Joint*) j1->GetUserData())->GetID();
			def.joint2 = ((Joint*) j2->GetUserData())->GetID();
			j = new GearJoint(def);
		}
		else if (type == "distance")
		{
			DistanceJointDef def;
			def.body1 = b1;
			def.body2 = b2;
			def.collideconnected = collision;

			// R�cup�re les attributs
			joint->QueryFloatAttribute("frequency", &def.frequencyHz);
			joint->QueryFloatAttribute("damping", &def.damping);

			// Cr�e le joint
			j = new DistanceJoint(def);
		}
		else if (type == "friction")
		{
			FrictionJointDef def;
			def.body1 = b1;
			def.body2 = b2;
			def.collideconnected = collision;

			// R�cup�re les attributs
			joint->QueryFloatAttribute("maxForce", &def.maxForce);
			joint->QueryFloatAttribute("maxTorque", &def.maxTorque);

			// Cr�e le joint
			j = new FrictionJoint(def);
		}
		else if (type == "rope")
		{
			RopeJointDef def;
			def.body1 = b1;
			def.body2 = b2;
			def.collideconnected = collision;

			// R�cup�re les attributs
			joint->QueryFloatAttribute("maxlength", &def.maxLength);

			// Cr�e le joint
			j = new RopeJoint(def);
		}
		else if (type == "weld")
		{
			WeldJointDef def;
			def.body1 = b1;
			def.body2 = b2;
			def.collideconnected = collision;

			// R�cup�re les attributs
			joint->QueryFloatAttribute("frequency", &def.frequencyHz);
			joint->QueryFloatAttribute("damping", &def.damping);

			// Cr�e le joint
			j = new WeldJoint(def);
		}
		else if (type == "pulley")
		{
			PulleyJointDef def;
			def.body1 = b1;
			def.body2 = b2;
			def.collideconnected = collision;

			// R�cup�re les attributs
			joint->QueryFloatAttribute("ratio", &def.ratio);
			if (joint->Attribute("groundpt1")) def.groundP1 = Parser::string2b2Vec2(joint->Attribute("groundpt1"));
			if (joint->Attribute("groundpt2")) def.groundP2 = Parser::string2b2Vec2(joint->Attribute("groundpt2"));

			// Cr�e le joint
			j = new PulleyJoint(def);
		}
		else if (type == "prismatic")
		{
			PrismaticJointDef def;
			def.body1 = b1;
			def.body2 = b2;
			def.collideconnected = collision;

			// R�cup�re les attributs
			if (joint->Attribute("axis")) def.axis = Parser::string2b2Vec2(joint->Attribute("axis"));
			joint->QueryBoolAttribute("enableLimits", &def.enableLimit);
			joint->QueryFloatAttribute("lower", &def.lowerTranslation);
			joint->QueryFloatAttribute("upper", &def.upperTranslation);
			joint->QueryBoolAttribute("enableMotor", &def.enableMotor);
			joint->QueryFloatAttribute("speed", &def.motorSpeed);
			joint->QueryFloatAttribute("maxForce", &def.maxForce);

			// Cr�e le joint
			j = new PrismaticJoint(def);
		}
		else if (type == "revolute")
		{
			RevoluteJointDef def;
			def.body1 = b1;
			def.body2 = b2;
			def.collideconnected = collision;

			// R�cup�re les attributs
			joint->QueryBoolAttribute("enableLimits", &def.enableLimit);
			joint->QueryFloatAttribute("lower", &def.lowerAngle);
			joint->QueryFloatAttribute("upper", &def.upperAngle);
			joint->QueryBoolAttribute("enableMotor", &def.enableMotor);
			joint->QueryFloatAttribute("speed", &def.motorSpeed);
			joint->QueryFloatAttribute("maxTorque", &def.maxTorque);

			// Cr�e le joint
			j = new RevoluteJoint(def);
		}
		else if (type == "wheel")
		{
			WheelJointDef def;
			def.car = b1;
			def.wheel = b2;
			def.collideconnected = collision;

			// R�cup�re les attributs
			if (joint->Attribute("axis")) def.axis = Parser::string2b2Vec2(joint->Attribute("axis"));
			joint->QueryFloatAttribute("frequency", &def.frequencyHz);
			joint->QueryFloatAttribute("damping", &def.damping);
			joint->QueryBoolAttribute("enableMotor", &def.enableMotor);
			joint->QueryFloatAttribute("speed", &def.motorSpeed);
			joint->QueryFloatAttribute("maxTorque", &def.maxTorque);

			// Cr�e le joint
			j = new WheelJoint(def);
		}

		// Enregiste l'ID
		if (j != nullptr && hasID)
		{
			// On regarde si l'ID n'est pas d�j� utilis�
			if (mBodyIDMap.find(id) != mBodyIDMap.end())
				Dialog::Error("L'ID " + Parser::int2string(id) + " n'est pas unique !", false);
			mJointIDMap[id] = j->GetJoint();
		}

		// On r�cup�re le prochain body
		joint = joint->NextSiblingElement();
	}

	return true;
}
bool LevelLoader::ProcessDeco()
{
	// R�cup�re <deco>
	tinyxml2::XMLHandle hdl(mFile);
	tinyxml2::XMLHandle deco = hdl.FirstChildElement("level").FirstChildElement("deco");

	// V�rifie que <deco> existe
	if (!deco.ToElement())
	{
		// Il n'y a tout simplement pas de d�co...
		return true;
	}
	
	// On cr�e les attributs
	Deco *d = nullptr;
	bool hasID = false;
	unsigned int id = 0U;
	std::string texture;
	b2Vec3 posRot;
	float rotation = 0.f;
	int zindex = 0;

	// Pour toutes les niveau
	int z = 0;
	tinyxml2::XMLHandle level = deco.FirstChildElement("level");
	while (level.ToElement())
	{
		z = 0;
		level.ToElement()->QueryIntAttribute("z", &z);

		// Pour chaque image
		tinyxml2::XMLElement *img = level.FirstChildElement("img").ToElement();
		while (img)
		{
			// R�initialise les attributs
			d = nullptr;
			hasID = false;
			id = 0U;
			rotation = 0.f;
			zindex = 0;
			
			// R�cup�re les attributs
			if (img->Attribute("texture")) texture = img->Attribute("texture");
			img->QueryIntAttribute("z-index", &zindex);
			if (img->Attribute("position")) posRot = Parser::string2b2Vec3(img->Attribute("position"));
			img->QueryFloatAttribute("rotation", &rotation);
			posRot.z = rotation;
			
			// Ajoute la d�co
			d = new Deco(z, mLevel.mTextureMap[texture], getVec3(b22sfVec(getVec2(posRot), mPhysicManager.GetPPM()), posRot.z));
			mLevel.mEntityManager.RegisterEntity(d);

			// On r�cup�re la prochaine image
			img = img->NextSiblingElement();
		}
		// On r�cup�re le prochain level
		level = level.NextSiblingElement();
	}

	return true;
}
bool LevelLoader::ProcessActions()
{
	// R�cup�re <actions>
	tinyxml2::XMLHandle hdl(mFile);
	tinyxml2::XMLHandle actions = hdl.FirstChildElement("level").FirstChildElement("actions");
	// V�rifie que <actions> existe
	if (!actions.ToElement())
	{
		// Il n'y a tout simplement pas d'events...
		return true;
	}

	// On cr�e les attributs
	std::string name;
	std::string file;
	std::string function;
	std::string type;

	// Pour toutes les triggers
	tinyxml2::XMLElement *action = actions.FirstChildElement().ToElement();
	while (action)
	{
		// R�initialise les attributs
		name.clear();
		file.clear();
		function.clear();
		type.clear();

		// Obtient le type
		type = action->Name();
		
		// Traite les diff�rents types
		if (type == "file")
		{
			// R�cup�re les attributs
			if (action->Attribute("name")) name = action->Attribute("name");
			if (action->Attribute("path")) file = action->Attribute("path");

			// Cr�e l'action
			LuaAction *a = new LuaAction(name, file);
			mLevel.mTriggersManager.AddAction(a);
		}
		else if (type == "function")
		{
			// R�cup�re les attributs
			// TODO: utiliser myCheck
			if (action->Attribute("name")) name = action->Attribute("name");
			if (action->Attribute("file")) file = action->Attribute("file");
			if (action->Attribute("func")) function = action->Attribute("func");

			// Cr�e l'action
			LuaAction *a = new LuaAction(name, file, function);
			mLevel.mTriggersManager.AddAction(a);
		}
		else
		{
			// TODO: myCheck -> Affiche un message, mais ne quitte pas
			Dialog::Error("Action type inconnu ("+ type +") !");
		}

		// On r�cup�re le prochain level
		action = action->NextSiblingElement();
	}

	return true;
}
bool LevelLoader::ProcessTriggers()
{
	// R�cup�re <events>
	tinyxml2::XMLHandle hdl(mFile);
	tinyxml2::XMLHandle events = hdl.FirstChildElement("level").FirstChildElement("events");
	// V�rifie que <events> existe
	if (!events.ToElement())
	{
		// Il n'y a tout simplement pas d'events...
		return true;
	}

	// R�cup�re <triggers>
	tinyxml2::XMLHandle triggers = events.FirstChildElement("triggers");
	// V�rifie que <triggers> existe
	if (!triggers.ToElement())
	{
		// Il n'y a tout simplement pas de triggers...
		return true;
	}
	
	// On cr�e les attributs
	b2AABB rect;
	std::string action;
	std::string type;

	// Pour toutes les triggers
	tinyxml2::XMLElement *trigger = triggers.FirstChildElement().ToElement();
	while (trigger)
	{
		// R�initialise les attributs
		rect.lowerBound = b2Vec2_zero;
		rect.upperBound = b2Vec2_zero;
		action.clear();
		type.clear();

		// Obtient le type
		type = trigger->Name();

		// R�cup�re l'action
		if (trigger->Attribute("action")) action = trigger->Attribute("action");

		// Traite les diff�rents types
		if (type == "area")
		{
			// R�cup�re la zone
			trigger->QueryFloatAttribute("top", &rect.upperBound.y);
			trigger->QueryFloatAttribute("bottom", &rect.lowerBound.y);
			trigger->QueryFloatAttribute("left", &rect.lowerBound.x);
			trigger->QueryFloatAttribute("right", &rect.upperBound.x);

			// Cr�e le trigger
			mLevel.mTriggersManager.CreateArea(rect, action);
		}

		/*/ R�cup�re les attributs
		if (img->Attribute("texture")) texture = img->Attribute("texture");
		img->QueryIntAttribute("z-index", &zindex);
		if (img->Attribute("position")) posRot = Parser::string2b2Vec3(img->Attribute("position"));
		img->QueryFloatAttribute("rotation", &rotation);
		posRot.z = rotation;*/

		// On r�cup�re le prochain level
		trigger = trigger->NextSiblingElement();
	}

	return true;
}
