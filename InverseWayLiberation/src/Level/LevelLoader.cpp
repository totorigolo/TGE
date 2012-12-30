#include "LevelLoader.h"

#include "../Tools/utils.h"
#include "../Tools/Dialog.h"
#include "../Tools/Parser.h"

#include "../Physics/World.h"

#include "../Physics/Bodies/StaticBox.h"
#include "../Physics/Bodies/DynamicBox.h"
#include "../Physics/Bodies/KinematicBox.h"
#include "../Physics/Bodies/DynamicCircle.h"

#include "../Physics/Joints/DistanceJoint.h"
#include "../Physics/Joints/FrictionJoint.h"
#include "../Physics/Joints/GearJoint.h"
#include "../Physics/Joints/PrismaticJoint.h"
#include "../Physics/Joints/PulleyJoint.h"
#include "../Physics/Joints/RevoluteJoint.h"
#include "../Physics/Joints/RopeJoint.h"
#include "../Physics/Joints/WeldJoint.h"
#include "../Physics/Joints/WheelJoint.h"

#include "../Entities/Entity.h"
#include "../Entities/Deco.h"
#include "../Entities/Ragdoll.h"
#include "../Entities/RawBody.h"

#include "../Lights/PointLight.h"
#include "../Lights/SpotLight.h"
#include "../Lights/LightManager.h"

#include <Thor/Resources.hpp>
#include <vector>

// Ctor
LevelLoader::LevelLoader(std::string const& path, Level *level)
	: Loader(path), mLevel(level)
{
	// Si le niveau ou le loader n'est pas valide, on ne fait rien
	if (mLevel->IsValid() && mIsValid)
	{
		// Vide le niveau si il est déjà chargé
		if (mLevel->IsCharged())
		{
			mLevel->Clear();
			LightManager::GetInstance().DeleteAllLights();
		}

		// Analyse le fichier du niveau et crée le Level
		mLevel->mIsCharged = Process();
		mLevel->PrepareForGame();
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
	if (!elem)
	{
		Dialog::Error("Une erreur grave est survenue lors du chargement du niveau.\nFichier invalide (" + mPath + ").");
		return false;
	}

	// Charge tout
	if (!ProcessWorld())
	{
		Dialog::Error("Une erreur grave est survenue lors du chargement du niveau.\nMonde non trouvé (" + mPath + ").");
		return false;
	}
	if (!ProcessTextures())
	{
		Dialog::Error("Une erreur grave est survenue lors du chargement du niveau.\nTextures invalides (" + mPath + ").");
		return false;
	}
	if (!ProcessBodies())
	{
		Dialog::Error("Une erreur grave est survenue lors du chargement du niveau.\nBodies invalides (" + mPath + ").");
		return false;
	}
	if (!ProcessEntities())
	{
		Dialog::Error("Une erreur grave est survenue lors du chargement du niveau.\nEntities invalide (" + mPath + ").");
		return false;
	}
	if (!ProcessJoints())
	{
		Dialog::Error("Une erreur grave est survenue lors du chargement du niveau.\nJoints invalides (" + mPath + ").");
		return false;
	}
	if (!ProcessDeco())
	{
		Dialog::Error("Une erreur grave est survenue lors du chargement du niveau.\nDeco invalide (" + mPath + ").");
		return false;
	}
	if (!ProcessLights())
	{
		Dialog::Error("Une erreur grave est survenue lors du chargement du niveau.\nLumières non valides (" + mPath + ").");
		return false;
	}

	// TODO: Essayer ça -> return false;
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
	bool defaultLightning = false;

	// Récupère les valeurs
	if (world)
	{
		if (world->Attribute("gravity")) gravity = Parser::string2b2Vec2(world->Attribute("gravity"));
		world->QueryFloatAttribute("PPM", &PPM);
		if (world->Attribute("bckgcolor")) bckgColor = Parser::string2color(world->Attribute("bckgcolor"));
		if (world->Attribute("originview")) originView = Parser::string2b2Vec2(world->Attribute("originview"));
		world->QueryFloatAttribute("defaultzoom", &defaultZoom);
		world->QueryBoolAttribute("defaultlightning", &defaultLightning);
	}
	// Si le monde n'est pas défini, on averti
	else
	{
		Dialog::Information("Les monde n'a pas été défini dans le niveau.\nLes valeurs par défaut ont été utilisées.");
	}

	// Change les attributs
	mLevel->mWorld->SetGravity(gravity);
	mLevel->mWorld->SetPPM(PPM);
	mLevel->mBckgC = bckgColor;
	mLevel->mOriginView = originView;
	mLevel->mDefaulfZoom = defaultZoom;
	mLevel->mLightning = defaultLightning;

	return true;
}
bool LevelLoader::ProcessTextures()
{
	// Récupère <textures>
	tinyxml2::XMLHandle hdl(mFile);
	tinyxml2::XMLHandle textures = hdl.FirstChildElement("level").FirstChildElement("textures");

	// Vérifie que <textures> existe
	if (!textures.ToElement())
	{
		Dialog::Error("Une erreur fatale est survenue lors du chargement du niveau.\n<textures> non trouvé (" + mPath + ").", true);
		return false;
	}
	
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
		try {
			mLevel->mTextureMap[name] = mLevel->mResourceManager.acquire(thor::Resources::fromFile<sf::Texture>(path));
		}
		catch (thor::ResourceLoadingException const& e)
		{
			Dialog::Error("Erreur lors du chargement d'une texture du niveau !\n" + std::string(e.what()), true);
		}

		// On récupère la prochaine texture
		texture = texture->NextSiblingElement();
	}

	return true;
}
bool LevelLoader::ProcessBodies()
{
	// Récupère <bodies>
	tinyxml2::XMLHandle hdl(mFile);
	tinyxml2::XMLHandle bodies = hdl.FirstChildElement("level").FirstChildElement("bodies");

	// Vérifie que <bodies> existe
	if (!bodies.ToElement())
	{
		Dialog::Error("Une erreur fatale est survenue lors du chargement du niveau.\n<bodies> non trouvé (" + mPath + ").", true);
		return false;
	}

	// On crée les attributs
	Body *b = nullptr;
	RawBody *e = nullptr;
	bool hasID = false, bullet = false, osp = false;
	BodyType bodyType = BodyType::FullySimulated;
	float density = 1.f, friction = 0.2f, restitution = 0.0f;
	float rotation = 0.f;
	unsigned int id = 0U;
	std::string texture, type;
	b2Vec3 posRot;

	// Pour tous les bodies
	tinyxml2::XMLElement *body = bodies.FirstChildElement().ToElement();
	while (body)
	{
		// Réinitialise les attributs
		b = nullptr;
		e = nullptr;
		id = 0U;
		rotation = 0.f;
		density = 1.f;
		friction = 0.2f;
		restitution = 0.0f;
		osp = false;
		hasID = false;
		bullet = false;
		bodyType = BodyType::FullySimulated;

		// Récupère le type
		if (body->Attribute("type")) type = body->Attribute("type");

		// Récupère la texture et vérifie si elle existe
		if (body->Attribute("texture")) texture = body->Attribute("texture");
		if (mLevel->mTextureMap.find(texture) == mLevel->mTextureMap.end())
		{
			Dialog::Error("Texture \"" + texture + "\" inconnue. Body ignoré !");

			// On passe au prochain body
			body = body->NextSiblingElement();
			continue;
		}

		// Récupère l'ID
		if (body->Attribute("id"))
		{
			body->QueryUnsignedAttribute("id", &id);
			hasID = true;
		}

		// Récupère la position et la rotation
		body->QueryFloatAttribute("rotation", &rotation);
		if (body->Attribute("pos")) posRot = Parser::string2b2Vec3(body->Attribute("pos"));
		posRot.z = rotation;

		// Récupère les flags
		body->QueryBoolAttribute("osp", &osp);
		body->QueryBoolAttribute("bullet", &bullet);
		if (bullet)
			bodyType = BodyType::Bullet;
		else if (osp)
			bodyType = BodyType::OneSidedPlatform;

		// Récupère les propriétés
		body->QueryFloatAttribute("density", &density);
		body->QueryFloatAttribute("friction", &friction);
		body->QueryFloatAttribute("restitution", &restitution);

		// Crée le body
		if (type == "staticbox")
		{
			b = new StaticBox(mLevel->mWorld, posRot, mLevel->mTextureMap[texture], friction, restitution);
		}
		else if (type == "dynamicbox")
		{
			b = new DynamicBox(mLevel->mWorld, posRot, mLevel->mTextureMap[texture], density, friction, restitution);
		}
		else if (type == "dynamiccircle")
		{
			b = new DynamicCircle(mLevel->mWorld, posRot, mLevel->mTextureMap[texture], density, friction, restitution);
		}
		else if (type == "kinematicbox")
		{
			b = new KinematicBox(mLevel->mWorld, posRot, mLevel->mTextureMap[texture], restitution);
		}

		// Change le type du body
		b->SetType(bodyType);

		// Crée l'Entity correspondante
		mLevel->mEntityManager.RegisterEntity(new RawBody(b));

		// Enregiste l'ID
		if (b != nullptr && hasID)
		{
			// On regarde si l'ID n'est pas déjà utilisé
			if (mBodyIDMap.find(id) != mBodyIDMap.end())
				Dialog::Error("L'ID " + Parser::int2string(id) + " n'est pas unique !", false);
			mBodyIDMap[id] = b;
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
	if (!entities.ToElement())
	{
		Dialog::Error("Une erreur fatale est survenue lors du chargement du niveau.\n<entities> non trouvé (" + mPath + ").", true);
		return false;
	}

	// On crée les attributs
	Entity *e = nullptr;
	b2Vec2 position;
	std::string type;
	
	// Pour toutes les Entities
	tinyxml2::XMLElement *entity = entities.FirstChildElement().ToElement();
	while (entity)
	{
		// Réinitialise les attributs
		e = nullptr;
		position = b2Vec2_zero;

		// Récupère le type
		if (entity->Attribute("type")) type = entity->Attribute("type");

		// Récupère la position et la rotation
		if (entity->Attribute("position")) position = Parser::string2b2Vec2(entity->Attribute("position"));

		// Crée l'Entity
		if (type == "ragdoll")
		{
			e = new Ragdoll(mLevel->mWorld, position);
		}

		// Crée l'Entity correspondante
		mLevel->mEntityManager.RegisterEntity(e);

		// On récupère la prochaine entity
		entity = entity->NextSiblingElement();
	}

	return true;
}
bool LevelLoader::ProcessJoints()
{
	// Récupère <bodies>
	tinyxml2::XMLHandle hdl(mFile);
	tinyxml2::XMLHandle joints = hdl.FirstChildElement("level").FirstChildElement("joints");

	// Vérifie que <bodies> existe
	if (!joints.ToElement())
	{
		Dialog::Error("Une erreur fatale est survenue lors du chargement du niveau.\n<joints> non trouvé (" + mPath + ").", true);
		return false;
	}

	// On crée les attributs
	Joint *j = nullptr;
	bool hasID = false;
	unsigned int id = 0U;
	std::string type;
	unsigned int IDb1 = 0U, IDb2 = 0U;
	Body *b1 = nullptr, *b2 = nullptr;
	b2Vec2 pt1, pt2, anchor;
	bool colision = true;
	sf::Color color = sf::Color::Green;

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
		colision = true;
		color = sf::Color::Green;

		// Récupère l'ID
		if (joint->Attribute("id"))
		{
			joint->QueryUnsignedAttribute("id", &id);
			hasID = true;
		}

		// Récupère le type
		if (joint->Attribute("type")) type = joint->Attribute("type");

		// Change la valeur de colision par défaut
		if (type == "weld" || type == "prismatic" || type == "revolute" || type == "wheel")
		{
			colision = false;
		}

		// Récupère les attributs suivant le type
		joint->QueryUnsignedAttribute("body1", &IDb1);
		joint->QueryUnsignedAttribute("body2", &IDb2);
		if (joint->Attribute("pt1")) pt1 = Parser::string2b2Vec2(joint->Attribute("pt1"));
		if (joint->Attribute("pt2")) pt2 = Parser::string2b2Vec2(joint->Attribute("pt2"));
		if (joint->Attribute("anchor")) anchor = Parser::string2b2Vec2(joint->Attribute("anchor"));
		joint->QueryBoolAttribute("colision", &colision);
		if (joint->Attribute("color")) color = Parser::string2color(joint->Attribute("color"));
		
		// Récupère les bodies
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

		// Crée le joint
		if (type == "gear")
		{
			Joint *j1 = nullptr, *j2 = nullptr;
			unsigned int IDj1 = 0U, IDj2 = 0U;
			float ratio;

			// Récupère les attributs
			joint->QueryUnsignedAttribute("joint1", &IDj1);
			joint->QueryUnsignedAttribute("joint2", &IDj2);
			joint->QueryFloatAttribute("ratio", &ratio);

			// Récupère les joints
			if (mJointIDMap.find(IDj1) == mJointIDMap.end())
			{
				Dialog::Error("Le joint #"+ Parser::uint2string(IDj1) +" est introuvable !");
				continue;
			}
			if (mJointIDMap.find(IDj2) == mJointIDMap.end())
			{
				Dialog::Error("Le joint #"+ Parser::uint2string(IDj2) +" est introuvable !");
				continue;
			}
			j1 = mJointIDMap[IDj1];
			j2 = mJointIDMap[IDj2];

			// Crée le joint
			j = new GearJoint(mLevel->mWorld, b1, b2, j1, j2, ratio, colision, color);
			mLevel->mWorld->RegisterJoint(j);
		}
		else if (type == "distance")
		{
			float frequency = 4.0f, damping = 0.5f;

			// Récupère les attributs
			joint->QueryFloatAttribute("frequency", &frequency);
			joint->QueryFloatAttribute("damping", &damping);

			// Crée le joint
			j = new DistanceJoint(mLevel->mWorld, b1, pt1, b2, pt2, frequency, damping, colision, color);
			mLevel->mWorld->RegisterJoint(j);
		}
		else if (type == "friction")
		{
			float maxForce = 0.f, maxTorque = 0.f;

			// Récupère les attributs
			joint->QueryFloatAttribute("maxForce", &maxForce);
			joint->QueryFloatAttribute("maxTorque", &maxTorque);

			// Crée le joint
			j = new FrictionJoint(mLevel->mWorld, b1, pt1, b2, pt2, maxForce, maxTorque, colision, color);
			mLevel->mWorld->RegisterJoint(j);
		}
		else if (type == "rope")
		{
			float maxlength = 0.f;

			// Récupère les attributs
			joint->QueryFloatAttribute("maxlength", &maxlength);

			// Crée le joint
			j = new RopeJoint(mLevel->mWorld, b1, pt1, b2, pt2, maxlength, colision, color);
			mLevel->mWorld->RegisterJoint(j);
		}
		else if (type == "weld")
		{
			float frequency = 4.f, damping = 0.5f;

			// Récupère les attributs
			joint->QueryFloatAttribute("frequency", &frequency);
			joint->QueryFloatAttribute("damping", &damping);

			// Crée le joint
			j = new WeldJoint(mLevel->mWorld, b1, b2, anchor, frequency, damping, colision, color);
			mLevel->mWorld->RegisterJoint(j);
		}
		else if (type == "pulley")
		{
			b2Vec2 groundpt1, groundpt2;
			float ratio = 1.f;

			// Récupère les attributs
			joint->QueryFloatAttribute("ratio", &ratio);
			if (joint->Attribute("groundpt1")) groundpt1 = Parser::string2b2Vec2(joint->Attribute("groundpt1"));
			if (joint->Attribute("groundpt2")) groundpt2 = Parser::string2b2Vec2(joint->Attribute("groundpt2"));

			// Crée le joint
			j = new PulleyJoint(mLevel->mWorld, b1, pt1, b2, pt2, groundpt1, groundpt2, ratio, colision, color);
			mLevel->mWorld->RegisterJoint(j);
		}
		else if (type == "prismatic")
		{
			b2Vec2 axis;
			bool enableLimits = false;
			float lower = 0.f, upper = 0.f;
			bool enableMotor = false;
			float speed = 0.f, maxForce = 10.f;

			// Récupère les attributs
			axis = Parser::string2b2Vec2(joint->Attribute("axis"));
			joint->QueryBoolAttribute("enableLimits", &enableLimits);
			joint->QueryFloatAttribute("lower", &lower);
			joint->QueryFloatAttribute("upper", &upper);
			joint->QueryBoolAttribute("enableMotor", &enableMotor);
			joint->QueryFloatAttribute("speed", &speed);
			joint->QueryFloatAttribute("maxForce", &maxForce);

			// Crée le joint
			j = new PrismaticJoint(mLevel->mWorld, b1, b2, anchor, axis, enableLimits, lower, upper, enableMotor, speed, maxForce, colision, color);
			mLevel->mWorld->RegisterJoint(j);
		}
		else if (type == "revolute")
		{
			bool enableLimits = false;
			float lower = 0.f, upper = 0.f;
			bool enableMotor = false;
			float speed = 0.f, maxTorque = 10.f;

			// Récupère les attributs
			joint->QueryBoolAttribute("enableLimits", &enableLimits);
			joint->QueryFloatAttribute("lower", &lower);
			joint->QueryFloatAttribute("upper", &upper);
			joint->QueryBoolAttribute("enableMotor", &enableMotor);
			joint->QueryFloatAttribute("speed", &speed);
			joint->QueryFloatAttribute("maxTorque", &maxTorque);

			// Crée le joint
			j = new RevoluteJoint(mLevel->mWorld, b1, b2, anchor, enableLimits, lower, upper, enableMotor, speed, maxTorque, colision, color);
			mLevel->mWorld->RegisterJoint(j);
		}
		else if (type == "wheel")
		{
			b2Vec2 axis;
			float frequency = 4.f, damping = 0.5f;
			bool enableMotor = false;
			float speed = 0.f, maxTorque = 10.f;

			// Récupère les attributs
			if (joint->Attribute("axis")) axis = Parser::string2b2Vec2(joint->Attribute("axis"));
			joint->QueryFloatAttribute("frequency", &frequency);
			joint->QueryFloatAttribute("damping", &damping);
			joint->QueryBoolAttribute("enableMotor", &enableMotor);
			joint->QueryFloatAttribute("speed", &speed);
			joint->QueryFloatAttribute("maxTorque", &maxTorque);

			// Crée le joint
			j = new WheelJoint(mLevel->mWorld, b1, b2, anchor, axis, frequency, damping, enableMotor, speed, maxTorque, colision, color);
			mLevel->mWorld->RegisterJoint(j);
		}

		// Enregiste l'ID
		if (j != nullptr && hasID)
		{
			// On regarde si l'ID n'est pas déjà utilisé
			if (mBodyIDMap.find(id) != mBodyIDMap.end())
				Dialog::Error("L'ID " + Parser::int2string(id) + " n'est pas unique !", false);
			mJointIDMap[id] = j;
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
	tinyxml2::XMLHandle level = deco.FirstChildElement("level");
	while (level.ToElement())
	{
		z = 0;
		level.ToElement()->QueryIntAttribute("z", &z);

		// Pour chaque image
		tinyxml2::XMLElement *img = level.FirstChildElement("img").ToElement();
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
			if (img->Attribute("position")) posRot = Parser::string2b2Vec3(img->Attribute("position"));
			img->QueryFloatAttribute("rotation", &rotation);
			posRot.z = rotation;
			
			// Ajoute la déco
			d = new Deco(z, &*mLevel->mTextureMap[texture], getVec3(b22sfVec(getVec2(posRot), mLevel->mWorld->GetPPM()), posRot.z));
			mLevel->mEntityManager.RegisterEntity(d);

			// On récupère la prochaine image
			img = img->NextSiblingElement();
		}
		// On récupère le prochain level
		level = level.NextSiblingElement();
	}

	return true;
}
bool LevelLoader::ProcessLights()
{
	// Récupère <lights>
	tinyxml2::XMLHandle hdl(mFile);
	tinyxml2::XMLHandle lights = hdl.FirstChildElement("level").FirstChildElement("lights");
	
	// Vérifie que <lights> existe
	if (!lights.ToElement())
	{
		Dialog::Error("Une erreur fatale est survenue lors du chargement du niveau.\n<lights> non trouvé (" + mPath + ").", true);
		return false;
	}
	
	// On crée les attributs
	unsigned int id = 0U;
	std::string parent, type;
	sf::Vector2f position;
	Body *b = nullptr;

	// Pour chaque lumière
	tinyxml2::XMLElement *light = lights.FirstChildElement().ToElement();
	while (light)
	{
		// Réinitialise les attributs
		id = 0U;
		b = nullptr;
		
		// Récupère le type de parent
		if (light->Attribute("parent")) parent = light->Attribute("parent");

		// Récupère l'ID de l'objet associé
		if (light->Attribute("pos")) position = b22sfVec(Parser::string2b2Vec2(light->Attribute("pos")), mLevel->mWorld->GetPPM());
		if (parent == "body")
		{
			light->QueryUnsignedAttribute("id", &id);
			if (mBodyIDMap.find(id) == mBodyIDMap.end())
			{
				Dialog::Error("Le body #"+ Parser::uint2string(id) +" est introuvable !");
				continue;
			}
			b = mBodyIDMap[id];
		}

		// Récupère le type de lampe
		if (light->Attribute("type")) type = light->Attribute("type");
			
		// Ajoute la lampe
		if (type == "point")
		{
			float radius = 0.f;
			bool isStatic = true, activated = true;

			light->QueryFloatAttribute("radius", &radius);
			light->QueryBoolAttribute("static", &isStatic);
			light->QueryBoolAttribute("activated", &activated);

			radius *= mLevel->mWorld->GetPPM();

			LightManager::GetInstance().AddLight(new PointLight(position, radius, isStatic, activated, b));
		}
		else if (type == "spot")
		{
			// TODO
		}

		// On récupère la prochaine image
		light = light->NextSiblingElement();
	}

	return true;
}
