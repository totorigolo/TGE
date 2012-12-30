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
		// Vide le niveau si il est d�j� charg�
		if (mLevel->IsCharged())
		{
			mLevel->Clear();
			LightManager::GetInstance().DeleteAllLights();
		}

		// Analyse le fichier du niveau et cr�e le Level
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
	// V�rifie que <level> existe dans le fichier
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
		Dialog::Error("Une erreur grave est survenue lors du chargement du niveau.\nMonde non trouv� (" + mPath + ").");
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
		Dialog::Error("Une erreur grave est survenue lors du chargement du niveau.\nLumi�res non valides (" + mPath + ").");
		return false;
	}

	// TODO: Essayer �a -> return false;
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
	bool defaultLightning = false;

	// R�cup�re les valeurs
	if (world)
	{
		if (world->Attribute("gravity")) gravity = Parser::string2b2Vec2(world->Attribute("gravity"));
		world->QueryFloatAttribute("PPM", &PPM);
		if (world->Attribute("bckgcolor")) bckgColor = Parser::string2color(world->Attribute("bckgcolor"));
		if (world->Attribute("originview")) originView = Parser::string2b2Vec2(world->Attribute("originview"));
		world->QueryFloatAttribute("defaultzoom", &defaultZoom);
		world->QueryBoolAttribute("defaultlightning", &defaultLightning);
	}
	// Si le monde n'est pas d�fini, on averti
	else
	{
		Dialog::Information("Les monde n'a pas �t� d�fini dans le niveau.\nLes valeurs par d�faut ont �t� utilis�es.");
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
	// R�cup�re <textures>
	tinyxml2::XMLHandle hdl(mFile);
	tinyxml2::XMLHandle textures = hdl.FirstChildElement("level").FirstChildElement("textures");

	// V�rifie que <textures> existe
	if (!textures.ToElement())
	{
		Dialog::Error("Une erreur fatale est survenue lors du chargement du niveau.\n<textures> non trouv� (" + mPath + ").", true);
		return false;
	}
	
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
			mLevel->mTextureMap[name] = mLevel->mResourceManager.acquire(thor::Resources::fromFile<sf::Texture>(path));
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
bool LevelLoader::ProcessBodies()
{
	// R�cup�re <bodies>
	tinyxml2::XMLHandle hdl(mFile);
	tinyxml2::XMLHandle bodies = hdl.FirstChildElement("level").FirstChildElement("bodies");

	// V�rifie que <bodies> existe
	if (!bodies.ToElement())
	{
		Dialog::Error("Une erreur fatale est survenue lors du chargement du niveau.\n<bodies> non trouv� (" + mPath + ").", true);
		return false;
	}

	// On cr�e les attributs
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
		// R�initialise les attributs
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

		// R�cup�re le type
		if (body->Attribute("type")) type = body->Attribute("type");

		// R�cup�re la texture et v�rifie si elle existe
		if (body->Attribute("texture")) texture = body->Attribute("texture");
		if (mLevel->mTextureMap.find(texture) == mLevel->mTextureMap.end())
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

		// R�cup�re les flags
		body->QueryBoolAttribute("osp", &osp);
		body->QueryBoolAttribute("bullet", &bullet);
		if (bullet)
			bodyType = BodyType::Bullet;
		else if (osp)
			bodyType = BodyType::OneSidedPlatform;

		// R�cup�re les propri�t�s
		body->QueryFloatAttribute("density", &density);
		body->QueryFloatAttribute("friction", &friction);
		body->QueryFloatAttribute("restitution", &restitution);

		// Cr�e le body
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

		// Cr�e l'Entity correspondante
		mLevel->mEntityManager.RegisterEntity(new RawBody(b));

		// Enregiste l'ID
		if (b != nullptr && hasID)
		{
			// On regarde si l'ID n'est pas d�j� utilis�
			if (mBodyIDMap.find(id) != mBodyIDMap.end())
				Dialog::Error("L'ID " + Parser::int2string(id) + " n'est pas unique !", false);
			mBodyIDMap[id] = b;
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
	if (!entities.ToElement())
	{
		Dialog::Error("Une erreur fatale est survenue lors du chargement du niveau.\n<entities> non trouv� (" + mPath + ").", true);
		return false;
	}

	// On cr�e les attributs
	Entity *e = nullptr;
	b2Vec2 position;
	std::string type;
	
	// Pour toutes les Entities
	tinyxml2::XMLElement *entity = entities.FirstChildElement().ToElement();
	while (entity)
	{
		// R�initialise les attributs
		e = nullptr;
		position = b2Vec2_zero;

		// R�cup�re le type
		if (entity->Attribute("type")) type = entity->Attribute("type");

		// R�cup�re la position et la rotation
		if (entity->Attribute("position")) position = Parser::string2b2Vec2(entity->Attribute("position"));

		// Cr�e l'Entity
		if (type == "ragdoll")
		{
			e = new Ragdoll(mLevel->mWorld, position);
		}

		// Cr�e l'Entity correspondante
		mLevel->mEntityManager.RegisterEntity(e);

		// On r�cup�re la prochaine entity
		entity = entity->NextSiblingElement();
	}

	return true;
}
bool LevelLoader::ProcessJoints()
{
	// R�cup�re <bodies>
	tinyxml2::XMLHandle hdl(mFile);
	tinyxml2::XMLHandle joints = hdl.FirstChildElement("level").FirstChildElement("joints");

	// V�rifie que <bodies> existe
	if (!joints.ToElement())
	{
		Dialog::Error("Une erreur fatale est survenue lors du chargement du niveau.\n<joints> non trouv� (" + mPath + ").", true);
		return false;
	}

	// On cr�e les attributs
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
		// R�initialise les attributs
		j = nullptr;
		hasID = false;
		id = 0U;
		IDb1 = 0U, IDb2 = 0U;
		b1 = nullptr, b2 = nullptr;
		colision = true;
		color = sf::Color::Green;

		// R�cup�re l'ID
		if (joint->Attribute("id"))
		{
			joint->QueryUnsignedAttribute("id", &id);
			hasID = true;
		}

		// R�cup�re le type
		if (joint->Attribute("type")) type = joint->Attribute("type");

		// Change la valeur de colision par d�faut
		if (type == "weld" || type == "prismatic" || type == "revolute" || type == "wheel")
		{
			colision = false;
		}

		// R�cup�re les attributs suivant le type
		joint->QueryUnsignedAttribute("body1", &IDb1);
		joint->QueryUnsignedAttribute("body2", &IDb2);
		if (joint->Attribute("pt1")) pt1 = Parser::string2b2Vec2(joint->Attribute("pt1"));
		if (joint->Attribute("pt2")) pt2 = Parser::string2b2Vec2(joint->Attribute("pt2"));
		if (joint->Attribute("anchor")) anchor = Parser::string2b2Vec2(joint->Attribute("anchor"));
		joint->QueryBoolAttribute("colision", &colision);
		if (joint->Attribute("color")) color = Parser::string2color(joint->Attribute("color"));
		
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
			Joint *j1 = nullptr, *j2 = nullptr;
			unsigned int IDj1 = 0U, IDj2 = 0U;
			float ratio;

			// R�cup�re les attributs
			joint->QueryUnsignedAttribute("joint1", &IDj1);
			joint->QueryUnsignedAttribute("joint2", &IDj2);
			joint->QueryFloatAttribute("ratio", &ratio);

			// R�cup�re les joints
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

			// Cr�e le joint
			j = new GearJoint(mLevel->mWorld, b1, b2, j1, j2, ratio, colision, color);
			mLevel->mWorld->RegisterJoint(j);
		}
		else if (type == "distance")
		{
			float frequency = 4.0f, damping = 0.5f;

			// R�cup�re les attributs
			joint->QueryFloatAttribute("frequency", &frequency);
			joint->QueryFloatAttribute("damping", &damping);

			// Cr�e le joint
			j = new DistanceJoint(mLevel->mWorld, b1, pt1, b2, pt2, frequency, damping, colision, color);
			mLevel->mWorld->RegisterJoint(j);
		}
		else if (type == "friction")
		{
			float maxForce = 0.f, maxTorque = 0.f;

			// R�cup�re les attributs
			joint->QueryFloatAttribute("maxForce", &maxForce);
			joint->QueryFloatAttribute("maxTorque", &maxTorque);

			// Cr�e le joint
			j = new FrictionJoint(mLevel->mWorld, b1, pt1, b2, pt2, maxForce, maxTorque, colision, color);
			mLevel->mWorld->RegisterJoint(j);
		}
		else if (type == "rope")
		{
			float maxlength = 0.f;

			// R�cup�re les attributs
			joint->QueryFloatAttribute("maxlength", &maxlength);

			// Cr�e le joint
			j = new RopeJoint(mLevel->mWorld, b1, pt1, b2, pt2, maxlength, colision, color);
			mLevel->mWorld->RegisterJoint(j);
		}
		else if (type == "weld")
		{
			float frequency = 4.f, damping = 0.5f;

			// R�cup�re les attributs
			joint->QueryFloatAttribute("frequency", &frequency);
			joint->QueryFloatAttribute("damping", &damping);

			// Cr�e le joint
			j = new WeldJoint(mLevel->mWorld, b1, b2, anchor, frequency, damping, colision, color);
			mLevel->mWorld->RegisterJoint(j);
		}
		else if (type == "pulley")
		{
			b2Vec2 groundpt1, groundpt2;
			float ratio = 1.f;

			// R�cup�re les attributs
			joint->QueryFloatAttribute("ratio", &ratio);
			if (joint->Attribute("groundpt1")) groundpt1 = Parser::string2b2Vec2(joint->Attribute("groundpt1"));
			if (joint->Attribute("groundpt2")) groundpt2 = Parser::string2b2Vec2(joint->Attribute("groundpt2"));

			// Cr�e le joint
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

			// R�cup�re les attributs
			axis = Parser::string2b2Vec2(joint->Attribute("axis"));
			joint->QueryBoolAttribute("enableLimits", &enableLimits);
			joint->QueryFloatAttribute("lower", &lower);
			joint->QueryFloatAttribute("upper", &upper);
			joint->QueryBoolAttribute("enableMotor", &enableMotor);
			joint->QueryFloatAttribute("speed", &speed);
			joint->QueryFloatAttribute("maxForce", &maxForce);

			// Cr�e le joint
			j = new PrismaticJoint(mLevel->mWorld, b1, b2, anchor, axis, enableLimits, lower, upper, enableMotor, speed, maxForce, colision, color);
			mLevel->mWorld->RegisterJoint(j);
		}
		else if (type == "revolute")
		{
			bool enableLimits = false;
			float lower = 0.f, upper = 0.f;
			bool enableMotor = false;
			float speed = 0.f, maxTorque = 10.f;

			// R�cup�re les attributs
			joint->QueryBoolAttribute("enableLimits", &enableLimits);
			joint->QueryFloatAttribute("lower", &lower);
			joint->QueryFloatAttribute("upper", &upper);
			joint->QueryBoolAttribute("enableMotor", &enableMotor);
			joint->QueryFloatAttribute("speed", &speed);
			joint->QueryFloatAttribute("maxTorque", &maxTorque);

			// Cr�e le joint
			j = new RevoluteJoint(mLevel->mWorld, b1, b2, anchor, enableLimits, lower, upper, enableMotor, speed, maxTorque, colision, color);
			mLevel->mWorld->RegisterJoint(j);
		}
		else if (type == "wheel")
		{
			b2Vec2 axis;
			float frequency = 4.f, damping = 0.5f;
			bool enableMotor = false;
			float speed = 0.f, maxTorque = 10.f;

			// R�cup�re les attributs
			if (joint->Attribute("axis")) axis = Parser::string2b2Vec2(joint->Attribute("axis"));
			joint->QueryFloatAttribute("frequency", &frequency);
			joint->QueryFloatAttribute("damping", &damping);
			joint->QueryBoolAttribute("enableMotor", &enableMotor);
			joint->QueryFloatAttribute("speed", &speed);
			joint->QueryFloatAttribute("maxTorque", &maxTorque);

			// Cr�e le joint
			j = new WheelJoint(mLevel->mWorld, b1, b2, anchor, axis, frequency, damping, enableMotor, speed, maxTorque, colision, color);
			mLevel->mWorld->RegisterJoint(j);
		}

		// Enregiste l'ID
		if (j != nullptr && hasID)
		{
			// On regarde si l'ID n'est pas d�j� utilis�
			if (mBodyIDMap.find(id) != mBodyIDMap.end())
				Dialog::Error("L'ID " + Parser::int2string(id) + " n'est pas unique !", false);
			mJointIDMap[id] = j;
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
			d = new Deco(z, &*mLevel->mTextureMap[texture], getVec3(b22sfVec(getVec2(posRot), mLevel->mWorld->GetPPM()), posRot.z));
			mLevel->mEntityManager.RegisterEntity(d);

			// On r�cup�re la prochaine image
			img = img->NextSiblingElement();
		}
		// On r�cup�re le prochain level
		level = level.NextSiblingElement();
	}

	return true;
}
bool LevelLoader::ProcessLights()
{
	// R�cup�re <lights>
	tinyxml2::XMLHandle hdl(mFile);
	tinyxml2::XMLHandle lights = hdl.FirstChildElement("level").FirstChildElement("lights");
	
	// V�rifie que <lights> existe
	if (!lights.ToElement())
	{
		Dialog::Error("Une erreur fatale est survenue lors du chargement du niveau.\n<lights> non trouv� (" + mPath + ").", true);
		return false;
	}
	
	// On cr�e les attributs
	unsigned int id = 0U;
	std::string parent, type;
	sf::Vector2f position;
	Body *b = nullptr;

	// Pour chaque lumi�re
	tinyxml2::XMLElement *light = lights.FirstChildElement().ToElement();
	while (light)
	{
		// R�initialise les attributs
		id = 0U;
		b = nullptr;
		
		// R�cup�re le type de parent
		if (light->Attribute("parent")) parent = light->Attribute("parent");

		// R�cup�re l'ID de l'objet associ�
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

		// R�cup�re le type de lampe
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

		// On r�cup�re la prochaine image
		light = light->NextSiblingElement();
	}

	return true;
}
