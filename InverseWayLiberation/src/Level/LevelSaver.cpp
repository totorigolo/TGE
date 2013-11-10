#include "LevelSaver.h"
#include "../Tools/utils.h"
#include "../Tools/Dialog.h"
#include "../Tools/Parser.h"
#include "../Physics/PhysicManager.h"

#include "../Entities/Entity.h"
#include "../Entities/Deco.h"
#include "../Entities/Player.h"
#include "../Entities/BasicBody.h"
#include "../Entities/LivingBeing.h"
#include "../Entities/EntityManager.h"

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

	// Cr�e les attributs
	world->SetAttribute("gravity", Parser::b2Vec2ToString(mPhysicManager.GetGravity()).c_str());
	world->SetAttribute("PPM", mLevel.mPhysicMgr.GetPPM());
	world->SetAttribute("bckgcolor", Parser::colorToString(mLevel.mBckgC).c_str());
	world->SetAttribute("originview", Parser::b2Vec2ToString(sf2b2Vec(mInputManager.GetDefaultCenter(), mPhysicManager.GetMPP())).c_str());
	world->SetAttribute("defaultzoom", mInputManager.GetDefaultZoom());
	
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
			if (bb->GetBasicBodyType() == BasicBody::Type::DynamicBox)
				type = "dynamicbox";
			else if (bb->GetBasicBodyType() == BasicBody::Type::DynamicCircle)
				type = "dynamiccircle";
			else if (bb->GetBasicBodyType() == BasicBody::Type::StaticBox)
				type = "staticbox";
			else
			{
				Dialog::Error("Erreur lors de la sauvegarde :\nBasicBody::Type == Null");
				type = "null";
			}
			
			// R�cup�re le nom de la texture
			std::string textureName(((Texture*) bb->GetSprite()->getTexture())->GetName());

			// R�cup�re l'ID
			unsigned int id = bb->GetID();

			// R�cup�re la Layer
			int layer = bb->GetLayer();

			// Cherche la position et la rotation
			b2Vec2 pos(bb->GetPosition());
			float rotation = bb->GetRotation();
			if (abs(rotation - 90) < 0.5) rotation = 90.f;
			else if (abs(rotation) < 0.5) rotation = 0.f;

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

			// Cr�e la balise <name>
			tinyxml2::XMLElement *balise = mDoc.NewElement(type.c_str());
			if (id != 0) balise->SetAttribute("id", id);
			balise->SetAttribute("texture", textureName.c_str());
			balise->SetAttribute("pos", Parser::b2Vec2ToString(pos).c_str());
			if (rotation != 0.f) balise->SetAttribute("rotation", rotation);
			if (type != "staticbox" && density != 1.f) balise->SetAttribute("density", density);
			if (friction != 0.2f) balise->SetAttribute("friction", friction);
			if (restitution != 0.f) balise->SetAttribute("restitution", restitution);
			if (layer != 1) balise->SetAttribute("layer", layer);

			if (bb->GetCollisionType() == BasicBody::CollisionType::Bullet)
				balise->SetAttribute("bullet", true);
			else if (bb->GetCollisionType() == BasicBody::CollisionType::OneSidedPlatform)
				balise->SetAttribute("osp", true);
			
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
	/*/ R�cup�re la balise <level>
	tinyxml2::XMLHandle handle(mDoc);
	tinyxml2::XMLNode *level = handle.FirstChildElement("level").ToNode();

	// Cr�e la balise <joints>
	tinyxml2::XMLElement *joints = mDoc.NewElement("joints");
	
	// Ajoute <joints> � <level>
	level->LinkEndChild(joints);

	// Parcours tous les Joints
	for (auto it = mLevel.mPhysicMgr.GetJointList().begin(); it != mLevel.mPhysicMgr.GetJointList().end(); ++it)
	{
		// V�rifie le type de l'Entity
		if ((*it)->GetType() == EntityType::BasicBody)
		{
			BasicBody *bb = ((BasicBody*) *it);

			// D�finit le nom de la balise
			std::string type;
			if (bb->GetBasicBodyType() == BasicBody::Type::DynamicBox)
				type = "dynamicbox";
			else if (bb->GetBasicBodyType() == BasicBody::Type::DynamicCircle)
				type = "dynamiccircle";
			else if (bb->GetBasicBodyType() == BasicBody::Type::StaticBox)
				type = "staticbox";
			else
			{
				Dialog::Error("Erreur lors de la sauvegarde :\nBasicBody::Type == Null");
				type = "null";
			}
			
			// R�cup�re le nom de la texture
			std::string textureName(((Texture*) bb->GetSprite()->getTexture())->GetName());

			// R�cup�re l'ID
			unsigned int id = bb->GetID();

			// R�cup�re la Layer
			int layer = bb->GetLayer();

			// Cherche la position et la rotation
			b2Vec2 pos(bb->GetPosition());
			float rotation = bb->GetRotation();
			if (abs(rotation - 90) < 0.5) rotation = 90.f;
			else if (abs(rotation) < 0.5) rotation = 0.f;

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

			// Cr�e la balise <name>
			tinyxml2::XMLElement *balise = mDoc.NewElement(type.c_str());
			if (id != 0) balise->SetAttribute("id", id);
			balise->SetAttribute("texture", textureName.c_str());
			balise->SetAttribute("pos", Parser::b2Vec2ToString(pos).c_str());
			if (rotation != 0.f) balise->SetAttribute("rotation", rotation);
			if (type != "staticbox" && density != 1.f) balise->SetAttribute("density", density);
			if (friction != 0.2f) balise->SetAttribute("friction", friction);
			if (restitution != 0.f) balise->SetAttribute("restitution", restitution);
			if (layer != 1) balise->SetAttribute("layer", layer);

			if (bb->GetCollisionType() == BasicBody::CollisionType::Bullet)
				balise->SetAttribute("bullet", true);
			else if (bb->GetCollisionType() == BasicBody::CollisionType::OneSidedPlatform)
				balise->SetAttribute("osp", true);
			
			// Ajoute la balise � <basicbodies>
			basicbodies->LinkEndChild(balise);
		}
	}*/

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
	// Tout s'est bien pass�
	return true;
}
bool LevelSaver::ProcessTriggers()
{
	// Tout s'est bien pass�
	return true;
}
