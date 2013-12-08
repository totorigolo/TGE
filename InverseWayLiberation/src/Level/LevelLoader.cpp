#include "LevelLoader.h"
#include "LevelManager.h"

#include "../Tools/utils.h"
#include "../Tools/Error.h"
#include "../Tools/Dialog.h"
#include "../Tools/Parser.h"

#include "../App/InputManager.h"
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

		// Analyse le fichier du niveau et crée le Level
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
	// Vérifie que <level> existe dans le fichier
	tinyxml2::XMLHandle hdl(mFile);
	tinyxml2::XMLElement *elem = hdl.FirstChildElement("level").ToElement();
	myCheckError(elem, "Une erreur est survenue lors du chargement du niveau.\n"
					   "<level> n'existe pas, fichier invalide (" + mPath + ").");

	/* Charge tout */
	// Le Monde
	myCheckError(ProcessWorld(), "Une erreur est survenue lors du chargement du niveau.\n"
								 "Le chargement de World a échoué (" + mPath + ").");

	// Les textures
	myCheckError(ProcessTextures(), "Une erreur est survenue lors du chargement du niveau.\n"
									"Le chargement des textures a échoué (" + mPath + ").");

	// Les Bodies basiques
	myCheckError(ProcessBasicBodies(), "Une erreur est survenue lors du chargement du niveau.\n"
									   "Le chargement des BasicBodies a échoué (" + mPath + ").");

	// Les Entities
	myCheckError(ProcessEntities(), "Une erreur est survenue lors du chargement du niveau.\n"
									"Le chargement des Entities a échoué (" + mPath + ").");

	// Les joints
	myCheckError(ProcessJoints(), "Une erreur est survenue lors du chargement du niveau.\n"
								  "Le chargement des joints a échoué (" + mPath + ").");

	// La déco
	myCheckError(ProcessDeco(), "Une erreur est survenue lors du chargement du niveau.\n"
									"Le chargement de la déco a échoué (" + mPath + ").");

	// Les actions
	myCheckError(ProcessActions(), "Une erreur est survenue lors du chargement du niveau.\n"
									"Le chargement des actions a échoué (" + mPath + ").");

	// Les déclencheurs
	myCheckError(ProcessTriggers(), "Une erreur est survenue lors du chargement du niveau.\n"
									"Le chargement des triggers a échoué (" + mPath + ").");

	return true;
}

// Une fonction par catégorie
bool LevelLoader::ProcessWorld()
{
	// Récupère <world>
	tinyxml2::XMLHandle hdl(mFile);
	tinyxml2::XMLElement *world = hdl.FirstChildElement("level").FirstChildElement("world").ToElement();

	// Valeurs par défaut
	b2Vec2 gravity(0.f, -9.8f);
	float PPM = 100.f;
	sf::Color bckgColor = sf::Color::White;
	b2Vec2 originView(0.f, 0.f);
	float defaultZoom = 1.f;

	// Récupère les valeurs
	if (world)
	{
		if (world->Attribute("gravity")) gravity = Parser::stringToB2Vec2(world->Attribute("gravity"));
		world->QueryFloatAttribute("PPM", &PPM);
		if (world->Attribute("bckgcolor")) bckgColor = Parser::stringToColor(world->Attribute("bckgcolor"));
		if (world->Attribute("originview")) originView = Parser::stringToB2Vec2(world->Attribute("originview"));
		world->QueryFloatAttribute("defaultzoom", &defaultZoom);
	}

	// Si le monde n'est pas défini, on averti
	else
	{
		Dialog::Information("Les monde n'a pas été défini dans le niveau.\nLes valeurs par défaut ont été utilisées.");
	}

	// Change les attributs
	mLevel.SetBckgColor(bckgColor);
	mPhysicManager.SetGravity(gravity);
	mPhysicManager.SetPPM(PPM);
	mLevel.SetDefaultZoom(defaultZoom);
	mLevel.SetDefaultCenter(b22sfVec(originView, mPhysicManager.GetPPM()));

	return true;
}
bool LevelLoader::ProcessTextures()
{
	// Récupère <textures>
	tinyxml2::XMLHandle hdl(mFile);
	tinyxml2::XMLHandle textures = hdl.FirstChildElement("level").FirstChildElement("textures");

	// Vérifie que <textures> existe
	myCheckError(textures.ToElement(), "Une erreur est survenue lors du chargement du niveau.\n<textures> non trouvé (" + mPath + ").");
	
	// On crée les attributs
	std::string path, name;

	// Pour toutes les textures
	tinyxml2::XMLElement *texture = textures.FirstChildElement().ToElement();
	while (texture)
	{
		// Récupère les éléments
		if (texture->Attribute("name")) name = texture->Attribute("name");
		if (texture->Attribute("src")) path = texture->Attribute("src");

		// Charge la texture
		mLevel.mResourceManager.LoadTexture(name, path);

		// On récupère la prochaine texture
		texture = texture->NextSiblingElement();
	}

	return true;
}
bool LevelLoader::ProcessBasicBodies()
{
	// Récupère <basicbodies>
	tinyxml2::XMLHandle hdl(mFile);
	tinyxml2::XMLHandle bodies = hdl.FirstChildElement("level").FirstChildElement("basicbodies");

	// Vérifie que <basicbodies> existe
	myCheckError(bodies.ToElement(), "Une erreur est survenue lors du chargement du niveau.\n<basicbodies> non trouvé (" + mPath + ").");

	// On crée les attributs
	BasicBody *bb = nullptr;
	int layer = 1;
	bool bullet = false, osp = false;
	float density = 1.f, friction = 0.2f, restitution = 0.0f;
	int16 groupIndex = 0;
	uint16 categoryBits = 0x0001, maskBits = 0xFFFF;
	float rotation = 0.f;
	unsigned int id = 0U;
	std::string texture, type;
	b2Vec3 posRot;
	b2Vec2 linvel; // Linear velocity
	float angvel = 0.f; // Angular velocity

	// Pour tous les bodies
	tinyxml2::XMLElement *body = bodies.FirstChildElement().ToElement();
	while (body)
	{
		// Réinitialise les attributs
		bb = nullptr;
		layer = 1;
		id = 0U;
		rotation = 0.f;
		density = 1.f;
		friction = 0.2f;
		restitution = 0.0f;
		groupIndex = 0;
		categoryBits = 0x0001;
		maskBits = 0xFFFF;
		osp = false;
		bullet = false;
		linvel = b2Vec2_zero;
		angvel = 0.f;

		// Récupère le type
		type = body->Name();

		// Récupère la texture et vérifie si elle existe
		if (body->Attribute("texture")) texture = body->Attribute("texture");
		if (!mLevel.mResourceManager.TextureExists(texture))
		{
			Dialog::Error("Texture \"" + texture + "\" inconnue. Body ignoré !");

			// On passe au prochain body
			body = body->NextSiblingElement();
			continue;
		}

		// Récupère l'ID
		if (body->Attribute("id")) body->QueryUnsignedAttribute("id", &id);

		// Récupère la position et la rotation
		body->QueryFloatAttribute("rotation", &rotation);
		if (body->Attribute("pos")) posRot = Parser::stringToB2Vec3(body->Attribute("pos"));
		posRot.z = rotation;

		// Récupère l'état
		if (body->Attribute("linvel")) linvel = Parser::stringToB2Vec2(body->Attribute("linvel"));
		body->QueryFloatAttribute("angvel", &angvel);

		// Récupère le layer
		body->QueryIntAttribute("layer", &layer);

		// Récupère les propriétés
		body->QueryFloatAttribute("density", &density);
		body->QueryFloatAttribute("friction", &friction);
		body->QueryFloatAttribute("restitution", &restitution);

		// Récupère les propriétés de groupe
		int tmp = 0;
		body->QueryIntAttribute("groupIndex", &tmp);
		groupIndex = static_cast<int16>(tmp);
		tmp = 0x0001;
		body->QueryIntAttribute("categoryBits", &tmp);
		categoryBits = static_cast<uint16>(tmp);
		tmp = 0xFFFF;
		body->QueryIntAttribute("maskBits", &tmp);
		maskBits = static_cast<uint16>(tmp);

		// Crée le body
		if (type == "staticbox")
		{
			bb = new BasicBody(layer, id);
			bb->CreateStaticBox(posRot, mLevel.mResourceManager.GetTexture(texture), density, friction, restitution, groupIndex, categoryBits, maskBits);
		}
		else if (type == "staticcircle")
		{
			bb = new BasicBody(layer, id);
			bb->CreateStaticCircle(posRot, mLevel.mResourceManager.GetTexture(texture), density, friction, restitution, groupIndex, categoryBits, maskBits);
		}
		else if (type == "dynamicbox")
		{
			bb = new BasicBody(layer, id);
			bb->CreateDynBox(posRot, mLevel.mResourceManager.GetTexture(texture), density, friction, restitution, groupIndex, categoryBits, maskBits);

			// Restore l'état
			bb->GetBody()->SetLinearVelocity(linvel);
			bb->GetBody()->SetAngularVelocity(angvel);
		}
		else if (type == "dynamiccircle")
		{
			bb = new BasicBody(layer, id);
			bb->CreateDynCircle(posRot, mLevel.mResourceManager.GetTexture(texture), density, friction, restitution, groupIndex, categoryBits, maskBits);

			// Restore l'état
			bb->GetBody()->SetLinearVelocity(linvel);
			bb->GetBody()->SetAngularVelocity(angvel);
		}
		else
		{
			Dialog::Error("BasicBody type inconnu ("+ type +") !");
		}

		// Si le BasicBody a été créé
		if (bb)
		{
			// Propriétés de collision
			body->QueryBoolAttribute("osp", &osp);
			body->QueryBoolAttribute("bullet", &bullet);

			// Applique les propriétés
			if (osp && bullet)
				Dialog::Error("Impossible d'avoir osp et bullet en même temps !");
			else if (osp)
				bb->SetCollisionType(BasicBody::CollisionType::OneSidedPlatform);
			else if (bullet)
				bb->SetCollisionType(BasicBody::CollisionType::Bullet);

			// Enregiste l'ID
			if (id != 0)
			{
				// On regarde si l'ID n'est pas déjà utilisé
				if (mBodyIDMap.find(id) != mBodyIDMap.end())
					Dialog::Error("L'ID " + Parser::intToString(id) + " n'est pas unique !");
				mBodyIDMap[id] = bb->GetBody();
			}
		}

		// On récupère le prochain body
		body = body->NextSiblingElement();
	}

	return true;
}
bool LevelLoader::ProcessEntities()
{
	// Récupère <entities>
	tinyxml2::XMLHandle hdl(mFile);
	tinyxml2::XMLHandle entities = hdl.FirstChildElement("level").FirstChildElement("entities");

	// Vérifie que <entities> existe
	myCheckError(entities.ToElement(), "Une erreur est survenue lors du chargement du niveau.\n<entities> non trouvé (" + mPath + ").");

	// On crée les attributs
	Entity *e = nullptr;
	int layer = 1;
	b2Vec2 position;
	std::string type, animation;
	
	// Pour toutes les Entities
	tinyxml2::XMLElement *entity = entities.FirstChildElement().ToElement();
	while (entity)
	{
		// Réinitialise les attributs
		e = nullptr;
		layer = 1;
		type = "";
		animation = "";
		position = b2Vec2_zero;

		// Récupère le type
		type = entity->Name();

		// Récupère la position et la rotation
		if (entity->Attribute("position")) position = Parser::stringToB2Vec2(entity->Attribute("position"));
		
		// Récupère le layer
		entity->QueryIntAttribute("layer", &layer);

		// Crée l'Entity
		if (type == "ragdoll")
		{
			EntityFactory::CreateRagdoll(position, layer);
		}
		else if (type == "livingbeing")
		{
			// Récupère l'animation
			animation = entity->Attribute("animation");
			animation = entity->Attribute("texture");

			e = new LivingBeing(position, mLevel.mResourceManager.GetTexture(animation), layer);
		}
		else if (type == "player")
		{
			// Récupère l'animation
			animation = entity->Attribute("animation");
			animation = entity->Attribute("texture");

			// Enregistre le Player dans Level
			e = new Player(position, mLevel.mResourceManager.GetTexture(animation), layer);
			mLevel.mPlayer = (Player*) e;
		}
		else
		{
			Dialog::Error("Entity type inconnu ("+ type +") !");
		}

		// On récupère la prochaine entity
		entity = entity->NextSiblingElement();
	}

	return true;
}
bool LevelLoader::ProcessJoints()
{
	// Récupère <joints>
	tinyxml2::XMLHandle hdl(mFile);
	tinyxml2::XMLHandle joints = hdl.FirstChildElement("level").FirstChildElement("joints");

	// Vérifie que <joints> existe
	if (!joints.ToElement())
	{
		// Il n'y a tout simplement pas de joint.
		return true;
	}

	// On crée les attributs
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
		// Réinitialise les attributs
		j = nullptr;
		hasID = false;
		id = 0U;
		IDb1 = 0U, IDb2 = 0U;
		b1 = nullptr, b2 = nullptr;
		collision = true;

		// Récupère l'ID
		if (joint->Attribute("id"))
		{
			joint->QueryUnsignedAttribute("id", &id);
			hasID = true;
		}

		// Récupère le type
		type = joint->Name();

		// Change la valeur de collision par défaut
		if (type == "weld" || type == "prismatic" || type == "revolute" || type == "wheel")
		{
			collision = false;
		}

		// Récupère les attributs suivant le type
		joint->QueryUnsignedAttribute("body1", &IDb1);
		joint->QueryUnsignedAttribute("body2", &IDb2);
		if (joint->Attribute("pt1")) pt1 = Parser::stringToB2Vec2(joint->Attribute("pt1"));
		if (joint->Attribute("pt2")) pt2 = Parser::stringToB2Vec2(joint->Attribute("pt2"));
		if (joint->Attribute("anchor")) anchor = Parser::stringToB2Vec2(joint->Attribute("anchor"));
		joint->QueryBoolAttribute("collision", &collision);
		
		// Récupère les bodies
		if (mBodyIDMap.find(IDb1) == mBodyIDMap.end())
		{
			Dialog::Error("Le body #"+ Parser::uintToString(IDb1) +" est introuvable !");

			// On récupère le prochain body
			joint = joint->NextSiblingElement();
			continue;
		}
		if (mBodyIDMap.find(IDb2) == mBodyIDMap.end())
		{
			Dialog::Error("Le body #"+ Parser::uintToString(IDb2) +" est introuvable !");

			// On récupère le prochain body
			joint = joint->NextSiblingElement();
			continue;
		}
		b1 = mBodyIDMap[IDb1];
		b2 = mBodyIDMap[IDb2];

		// Crée le joint
		if (type == "gear")
		{
			b2Joint *j1 = nullptr, *j2 = nullptr;
			unsigned int IDj1 = 0U, IDj2 = 0U;
			GearJointDef def;
			def.body1 = b1;
			def.body2 = b2;
			def.collideconnected = collision;

			// Récupère les attributs
			joint->QueryUnsignedAttribute("joint1", &IDj1);
			joint->QueryUnsignedAttribute("joint2", &IDj2);
			joint->QueryFloatAttribute("ratio", &def.ratio);

			// Récupère les joints
			if (mJointIDMap.find(IDj1) == mJointIDMap.end())
			{
				Dialog::Error("Le joint #"+ Parser::uintToString(IDj1) +" est introuvable !\n"
							  "Il doit être construit avant.");

				// On récupère le prochain body
				joint = joint->NextSiblingElement();
				continue;
			}
			if (mJointIDMap.find(IDj2) == mJointIDMap.end())
			{
				Dialog::Error("Le joint #"+ Parser::uintToString(IDj2) +" est introuvable !\n"
							  "Il doit être construit avant.");

				// On récupère le prochain body
				joint = joint->NextSiblingElement();
				continue;
			}
			j1 = mJointIDMap[IDj1];
			j2 = mJointIDMap[IDj2];

			// Vérifie qu'il soient bien construits
			myAssert(j1->GetUserData(), "Le joint1 #" + Parser::intToString(IDj1) + " du Gear n'existe pas.");
			myAssert(j2->GetUserData(), "Le joint2 #" + Parser::intToString(IDj2) + " du Gear n'existe pas.");

			// Vérifie qu'il soient bien construits
			myAssert(j1->GetUserData(), "Le joint n'existe pas.");
			myAssert(j2->GetUserData(), "Le joint n'existe pas.");

			// Crée le joint
			def.joint1 = ((Joint*) j1->GetUserData())->GetID();
			def.joint2 = ((Joint*) j2->GetUserData())->GetID();
			j = new GearJoint(def);
		}
		else if (type == "distance")
		{
			DistanceJointDef def;
			def.body1 = b1;
			def.body2 = b2;
			def.point1 = pt1;
			def.point2 = pt2;
			def.collideconnected = collision;

			// Récupère les attributs
			joint->QueryFloatAttribute("frequency", &def.frequencyHz);
			joint->QueryFloatAttribute("damping", &def.damping);

			// Crée le joint
			j = new DistanceJoint(def);
		}
		else if (type == "friction")
		{
			FrictionJointDef def;
			def.body1 = b1;
			def.body2 = b2;
			def.point1 = pt1;
			def.point2 = pt2;
			def.collideconnected = collision;

			// Récupère les attributs
			joint->QueryFloatAttribute("maxFrictionForce", &def.maxFrictionForce);
			joint->QueryFloatAttribute("maxFrictionTorque", &def.maxFrictionTorque);

			// Crée le joint
			j = new FrictionJoint(def);
		}
		else if (type == "rope")
		{
			RopeJointDef def;
			def.body1 = b1;
			def.body2 = b2;
			def.point1 = pt1;
			def.point2 = pt2;
			def.collideconnected = collision;

			// Récupère les attributs
			joint->QueryFloatAttribute("maxlength", &def.maxLength);

			// Crée le joint
			j = new RopeJoint(def);
		}
		else if (type == "weld")
		{
			WeldJointDef def;
			def.body1 = b1;
			def.body2 = b2;
			def.anchor = anchor;
			def.collideconnected = collision;

			// Récupère les attributs
			joint->QueryFloatAttribute("frequency", &def.frequencyHz);
			joint->QueryFloatAttribute("damping", &def.damping);

			// Crée le joint
			j = new WeldJoint(def);
		}
		else if (type == "pulley")
		{
			PulleyJointDef def;
			def.body1 = b1;
			def.body2 = b2;
			def.point1 = pt1;
			def.point2 = pt2;
			def.collideconnected = collision;

			// Récupère les attributs
			joint->QueryFloatAttribute("ratio", &def.ratio);
			if (joint->Attribute("groundpt1")) def.groundP1 = Parser::stringToB2Vec2(joint->Attribute("groundpt1"));
			if (joint->Attribute("groundpt2")) def.groundP2 = Parser::stringToB2Vec2(joint->Attribute("groundpt2"));

			// Crée le joint
			j = new PulleyJoint(def);
		}
		else if (type == "prismatic")
		{
			PrismaticJointDef def;
			def.body1 = b1;
			def.body2 = b2;
			def.anchor = anchor;
			def.collideconnected = collision;

			// Récupère les attributs
			if (joint->Attribute("axis")) def.axis = Parser::stringToB2Vec2(joint->Attribute("axis"));
			joint->QueryBoolAttribute("enableLimit", &def.enableLimit);
			joint->QueryFloatAttribute("lower", &def.lowerTranslation);
			joint->QueryFloatAttribute("upper", &def.upperTranslation);
			joint->QueryBoolAttribute("enableMotor", &def.enableMotor);
			joint->QueryFloatAttribute("speed", &def.motorSpeed);
			joint->QueryFloatAttribute("maxMotorForce", &def.maxMotorForce);

			// Crée le joint
			j = new PrismaticJoint(def);
		}
		else if (type == "revolute")
		{
			RevoluteJointDef def;
			def.body1 = b1;
			def.body2 = b2;
			def.anchor = anchor;
			def.collideconnected = collision;

			// Récupère les attributs
			joint->QueryBoolAttribute("enableLimit", &def.enableLimit);
			joint->QueryFloatAttribute("lower", &def.lowerAngle);
			joint->QueryFloatAttribute("upper", &def.upperAngle);
			joint->QueryBoolAttribute("enableMotor", &def.enableMotor);
			joint->QueryFloatAttribute("speed", &def.motorSpeed);
			joint->QueryFloatAttribute("maxMotorTorque", &def.maxMotorTorque);

			// Crée le joint
			j = new RevoluteJoint(def);
		}
		else if (type == "wheel")
		{
			WheelJointDef def;
			def.car = b1;
			def.wheel = b2;
			def.pWheel = anchor;
			def.collideconnected = collision;

			// Récupère les attributs
			if (joint->Attribute("axis")) def.axis = Parser::stringToB2Vec2(joint->Attribute("axis"));
			joint->QueryFloatAttribute("frequency", &def.frequencyHz);
			joint->QueryFloatAttribute("damping", &def.damping);
			joint->QueryBoolAttribute("enableMotor", &def.enableMotor);
			joint->QueryFloatAttribute("speed", &def.motorSpeed);
			joint->QueryFloatAttribute("maxTorque", &def.maxTorque);

			// Crée le joint
			j = new WheelJoint(def);
		}

		// Propriétés de cassure communes aux Joints
		if (j)
		{
			JointDef breakDef;

			joint->QueryBoolAttribute("isBreakableMaxForce", &breakDef.isBreakableMaxForce);
			if (joint->Attribute("maxVecForce"))
			{
				breakDef.maxVecForce = Parser::stringToB2Vec2(joint->Attribute("maxVecForce"));
				breakDef.maxForceType = ForceType::Vector;
			}
			else if (joint->Attribute("maxForce"))
			{
				joint->QueryFloatAttribute("maxForce", &breakDef.maxForce);
				breakDef.maxForceType = ForceType::Float;
			}
			joint->QueryBoolAttribute("isBreakableMaxTorque", &breakDef.isBreakableMaxTorque);
			joint->QueryFloatAttribute("maxTorque", &breakDef.maxTorque);

			// Appliques les propriétés
			j->SetBreakableByForce(breakDef.isBreakableMaxForce);
			if (breakDef.maxForceType == ForceType::Float) j->SetMaxForce(breakDef.maxForce);
			if (breakDef.maxForceType == ForceType::Vector) j->SetMaxForce(breakDef.maxVecForce);
			j->SetBreakableByTorque(breakDef.isBreakableMaxTorque);
			j->SetMaxTorque(breakDef.maxTorque);
		}

		// Enregiste l'ID
		if (j != nullptr && hasID)
		{
			// On regarde si l'ID n'est pas déjà utilisé
			if (mJointIDMap.find(id) != mJointIDMap.end())
				Dialog::Error("L'ID du Joint " + Parser::intToString(id) + " n'est pas unique !", false);
			mJointIDMap[id] = j->GetJoint();
		}

		// On récupère le prochain body
		joint = joint->NextSiblingElement();
	}

	return true;
}
bool LevelLoader::ProcessDeco()
{
	// Récupère <deco>
	tinyxml2::XMLHandle hdl(mFile);
	tinyxml2::XMLHandle deco = hdl.FirstChildElement("level").FirstChildElement("deco");

	// Vérifie que <deco> existe
	if (!deco.ToElement())
	{
		// Il n'y a tout simplement pas de déco...
		return true;
	}
	
	// On crée les attributs
	Deco *d = nullptr;
	bool hasID = false;
	unsigned int id = 0U;
	std::string texture;
	b2Vec3 posRot;
	float rotation = 0.f;
	int zindex = 0;

	// Pour toutes les niveau
	int z = 0;
	tinyxml2::XMLHandle layer = deco.FirstChildElement("layer");
	while (layer.ToElement())
	{
		z = 0;
		layer.ToElement()->QueryIntAttribute("z", &z);

		// Pour chaque image
		tinyxml2::XMLElement *img = layer.FirstChildElement("img").ToElement();
		while (img)
		{
			// Réinitialise les attributs
			d = nullptr;
			hasID = false;
			id = 0U;
			rotation = 0.f;
			zindex = 0;
			
			// Récupère les attributs
			if (img->Attribute("texture")) texture = img->Attribute("texture");
			img->QueryIntAttribute("z-index", &zindex);
			if (img->Attribute("position")) posRot = Parser::stringToB2Vec3(img->Attribute("position"));
			img->QueryFloatAttribute("rotation", &rotation);
			posRot.z = rotation;
			
			// Ajoute la déco
			d = new Deco(z, mLevel.mResourceManager.GetTexture(texture), getVec3(b22sfVec(getVec2(posRot), mPhysicManager.GetPPM()), posRot.z));

			// On récupère la prochaine image
			img = img->NextSiblingElement();
		}
		// On récupère le prochain layer
		layer = layer.NextSiblingElement();
	}

	return true;
}
bool LevelLoader::ProcessActions()
{
	// Récupère <actions>
	tinyxml2::XMLHandle hdl(mFile);
	tinyxml2::XMLHandle actions = hdl.FirstChildElement("level").FirstChildElement("actions");
	// Vérifie que <actions> existe
	if (!actions.ToElement())
	{
		// Il n'y a tout simplement pas d'events...
		return true;
	}

	// On crée les attributs
	std::string name;
	std::string file;
	std::string function;
	std::string type;

	// Pour toutes les actions
	tinyxml2::XMLElement *action = actions.FirstChildElement().ToElement();
	while (action)
	{
		// Réinitialise les attributs
		name.clear();
		file.clear();
		function.clear();
		type.clear();

		// Obtient le type
		type = action->Name();
		
		// Traite les différents types
		if (type == "file")
		{
			// Récupère les attributs
			if (action->Attribute("name")) name = action->Attribute("name");
			if (action->Attribute("path")) file = action->Attribute("path");

			// Crée l'action
			LuaAction *a = new LuaAction(name, file);
			mLevel.mTriggersManager.AddAction(a);
		}
		else if (type == "function")
		{
			// Récupère les attributs
			// TODO: utiliser myCheck
			if (action->Attribute("name")) name = action->Attribute("name");
			if (action->Attribute("file")) file = action->Attribute("file");
			if (action->Attribute("func")) function = action->Attribute("func");

			// Crée l'action
			LuaAction *a = new LuaAction(name, file, function);
			mLevel.mTriggersManager.AddAction(a);
		}
		else
		{
			Dialog::Error("Action type inconnu ("+ type +") !");
		}

		// On récupère le prochain level
		action = action->NextSiblingElement();
	}

	return true;
}
bool LevelLoader::ProcessTriggers()
{
	// Récupère <events>
	tinyxml2::XMLHandle hdl(mFile);
	tinyxml2::XMLHandle events = hdl.FirstChildElement("level").FirstChildElement("events");
	// Vérifie que <events> existe
	if (!events.ToElement())
	{
		// Il n'y a tout simplement pas d'events...
		return true;
	}

	// Récupère <triggers>
	tinyxml2::XMLHandle triggers = events.FirstChildElement("triggers");
	// Vérifie que <triggers> existe
	if (!triggers.ToElement())
	{
		// Il n'y a tout simplement pas de triggers...
		return true;
	}
	
	// On crée les attributs
	b2AABB rect;
	std::string action;
	std::string type;

	// Pour toutes les triggers
	tinyxml2::XMLElement *trigger = triggers.FirstChildElement().ToElement();
	while (trigger)
	{
		// Réinitialise les attributs
		rect.lowerBound = b2Vec2_zero;
		rect.upperBound = b2Vec2_zero;
		action.clear();
		type.clear();

		// Obtient le type
		type = trigger->Name();

		// Récupère l'action
		if (trigger->Attribute("action")) action = trigger->Attribute("action");

		// Traite les différents types
		if (type == "area")
		{
			// Récupère la zone
			trigger->QueryFloatAttribute("top", &rect.upperBound.y);
			trigger->QueryFloatAttribute("bottom", &rect.lowerBound.y);
			trigger->QueryFloatAttribute("left", &rect.lowerBound.x);
			trigger->QueryFloatAttribute("right", &rect.upperBound.x);

			// Crée le trigger
			mLevel.mTriggersManager.CreateArea(rect, action);
		}

		/*/ Récupère les attributs
		if (img->Attribute("texture")) texture = img->Attribute("texture");
		img->QueryIntAttribute("z-index", &zindex);
		if (img->Attribute("position")) posRot = Parser::string2b2Vec3(img->Attribute("position"));
		img->QueryFloatAttribute("rotation", &rotation);
		posRot.z = rotation;*/

		// On récupère le prochain level
		trigger = trigger->NextSiblingElement();
	}

	return true;
}
