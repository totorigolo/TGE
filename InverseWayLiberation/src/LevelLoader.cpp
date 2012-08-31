#include "LevelLoader.h"
#include "utils.h"
#include "Dialog.h"
#include "Parser.h"
#include "Physics/World.h"

#include "Physics/Bodies/StaticBox.h"
#include "Physics/Bodies/DynamicBox.h"
#include "Physics/Bodies/KinematicBox.h"
#include "Physics/Bodies/DynamicCircle.h"

#include "Physics/Joints/DistanceJoint.h"
#include "Physics/Joints/FrictionJoint.h"
#include "Physics/Joints/GearJoint.h"
#include "Physics/Joints/PrismaticJoint.h"
#include "Physics/Joints/PulleyJoint.h"
#include "Physics/Joints/RevoluteJoint.h"
#include "Physics/Joints/RopeJoint.h"
#include "Physics/Joints/WeldJoint.h"
#include "Physics/Joints/WheelJoint.h"

#include <Thor/Resources.hpp>
#include <vector>

// Ctor
LevelLoader::LevelLoader(std::string const& path, Level *level)
	: mPath(path), mLevel(level)
{
	// Si le level n'est pas valide, on ne fait rien
	if (mLevel->IsValid())
	{
		// Vide le fichier si il est déjà chargé
		if (mLevel->IsCharged())
			mLevel->Clear();

		// Crée et ouvre le fichier
		mFile = new std::fstream(mPath, std::fstream::in);

		// Vérifie si le fichier existe
		if (mFile->fail())
		{
			Dialog::Error("Impossible d'ouvrir le fichier :\n\""+ mPath +"\"!", true);
			return;
		}

		// Analyse le fichier de level
		mLevel->mIsCharged = Process();
	}
}

// Dtor
LevelLoader::~LevelLoader(void)
{
	delete mFile;
}

bool LevelLoader::Process()
{
	// Parcours tout le fichier
	std::string line, currentSection = "";
	for (int currentLine = 1; std::getline(*mFile, line); ++currentLine)
	{
		// Analyse la ligne
		currentSection = ParseLine(line, currentSection, currentLine);
	}

	// Vide les listes d'ID
	mBodyIDMap.clear();
	mJointIDMap.clear();

	return true;
}

const std::string LevelLoader::ParseLine(std::string& line, std::string& section, int lineNumber)
{
	// TODO: Utiliser std::string::find_first_of();

	std::string lineSection = section; // valeur retournée
	if (line.size() > 1)
	{
		// Saute les espaces en début de ligne
		size_t offset = 0;
		for (; offset < line.size() && (line[offset] == ' ' || line[offset] == '\t'); ++offset);

		// Ignore les commentaires ("#" or ";")
		if (line[offset] != '#' && line[offset] != ';')
		{
			// Vérifie si c'est le début d'une section
			if (line[offset] == '[')
			{
				// Passe le caractère '['
				offset++;

				// Saute les espaces en début du nom de la section
				for (; offset < line.size() && line[offset] == ' ' || line[offset] == '\t'; ++offset);

				// Trouve le nom de la section
				std::string _section;
				unsigned int index = 0;
				for (; (offset + index) < line.size() && line[offset + index] != ']'; ++index)
					if (line[offset + index] != ' ' && line[offset + index] != '\t')
						_section += line[offset + index];

				// Supprime les espaces contenus à la fin du nom de la section
				for (unsigned int i = _section.size() -1; i >= 0 && (_section[i] == ' ' || _section[i] == '\t'); --i)
					_section.pop_back();

				// Enregistre la section seulement si on trouve le ']' avant la fin de la ligne
				if ((offset + index) < line.size() && index > 0)
					lineSection = _section;
				else
					Dialog::Error("ConfigReader::ParseLine("+ Parser::int2string(lineNumber) +")\n-> délimiteur ']' manquant.", true);
			}
			// Lit la paire nom=valeur dans la section courante
			else
			{
				std::string name;

				// Trouve le nom en allant jusqu'au '=' ou ':'
				for (; offset < line.size() && line[offset] != '=' && line[offset] != ':'; ++offset)
					if (line[offset] != ' ' && line[offset] != '\t')
						name += line[offset];

				// Supprime les espaces à la fin du nom
				for (unsigned int i = name.size() -1; i >= 0 && (name[i] == ' ' || name[i] == '\t'); --i)
					name.pop_back();

				// On recherche la valeur seulement si on a trouvé un délimiteur '=' ou ':'
				if (offset < line.size())
				{
					std::string value;

					// Passe par dessus '=' ou ':'
					offset++;

					// Saute les espaces
					for (; offset < line.size() && (line[offset] == ' ' || line[offset] == '\t'); ++offset);

					// Trouve la valeur en allant jusqu'au délimiteurs de commentaires ';' ou '#' ou la fin de ligne
					for (; offset < line.size() && line[offset] != '\r' && line[offset] != '\n' && line[offset] != ';' && line[offset] != '#'; ++offset)
						value += line[offset];

					// Supprime les espaces à la fin de la valeur
					for (unsigned int i = value.size() -1; i >= 0 && (value[i] == ' ' || value[i] == '\t'); --i)
						value.pop_back();
					
					// Crée la ligne obtenue
					CreateLine(lineSection, name, value);
				}
				else
					Dialog::Error("ConfigReader::ParseLine("+ Parser::int2string(lineNumber) +")\n-> délimiteur '=' ou ':' manquant.");
			}
		}
	}
	return lineSection;
}

bool LevelLoader::CreateLine(std::string& section, std::string& name, std::string& value)
{
	// [world] = Définition du monde
	/*
		gravity = (x.f, y.f)
		PPM = ppm.f
	*/
	if (section == "world")
	{
		if (name == "gravity")
		{
			mLevel->mWorld->SetGravity(Parser::string2b2Vec2(value));
			return true;
		}
		
		else if (name == "PPM")
		{
			mLevel->mWorld->SetPPM(Parser::string2float(value));
			return true;
		}
		
		else if (name == "bckgc")
		{
			mLevel->mBckgC = Parser::string2color(value);
			return true;
		}

		else if (name == "originView")
		{
			mLevel->mOriginView = Parser::string2b2Vec2(value);
			return true;
		}

		else if (name == "defaultZoom")
		{
			mLevel->mDefaulfZoom = Parser::string2float(value);
			return true;
		}
		
		Dialog::Error("ConfigReader::CreateLine() - ["+ section +"]\n-> "+ name +" unknow!");
		return false;
	}
	
	// [textures] = Chargement des textures
	/*
		# nom = chemin
		ex: box = tex/box.png
	*/
	else if (section == "textures")
	{
		// Charge la texture dans la textureKeyMap
		try {
			(*mLevel->mTextureMap)[name] = mLevel->mTextureCache->acquire(thor::Resources::fromFile<sf::Texture>(value));
		}
		catch (thor::ResourceLoadingException const& e)
		{
			Dialog::Error(e.what(), true);
		}
		
		return true;
	}
	
	// [bodies] = Création des bodies
	/*
		#   texture = (position + rotation° facultative) type proprietés
		# (les propriétées sont facultatives comme les paramètres C++)
		# types : ( + propriétés)
		#  - db = dynamicBox + densité(1.f) friction(0.2f) restitution(0.0f)
		#  - dc = dynamicCircle + densité(1.f) friction(0.2f) restitution(0.0f)
		#  - sb = staticBox + friction(0.2f) restitution(0.0f)
		#  - kb = kinematicBody + restitution(0.0f)

		# ex : box = (5.2f, 9.f, 45.f) db 2.f
	*/
	else if (section == "bodies")
	{
		// Récupère l'ID et la texture
		bool IDexists = false;
		unsigned int id = 0;
		size_t idOffset = name.find_first_of(',');
		std::string texture = name.substr(0, idOffset).c_str();
		if (idOffset != std::string::npos)
		{
			id = Parser::string2uint(name.substr(idOffset + 1).c_str());
			IDexists = true;
		}

		if (!IDexists || mBodyIDMap.find(id) == mBodyIDMap.end())
		{
			// Vérifie si la texture existe
			if (mLevel->mTextureMap->find(texture) != mLevel->mTextureMap->end())
			{
				// Récupère la position et rotation
				size_t posRotOffset = value.find_first_of(')');
				b2Vec3 posRot = Parser::string2b2Vec3(value.substr(0, posRotOffset + 1).c_str());
				
				// Récupère le "flag" (BodyType)
				BodyType bt = body_fullySimulated;
				if (value.find("[ac]", posRotOffset) != std::string::npos)
					bt = body_actor;
				else if (value.find("[osp]", posRotOffset) != std::string::npos)
					bt = body_oneSidedPlatform;

				// Récupère le type
				size_t isdb = value.find("db", posRotOffset);
				size_t isdc = value.find("dc", posRotOffset);
				size_t issb = value.find("sb", posRotOffset);
				size_t iskb = value.find("kb", posRotOffset);
				size_t list[] = {isdb, isdc, issb, iskb};
				size_t isOffset = first_not_of(list, 4, std::string::npos);

				if (isOffset != std::string::npos)
				{
					Body *b = nullptr;

					// Récupère les propriétés
					float p1 = -1.f, p2 = -1.f, p3 = -1.f;

					// Cherche p1
					size_t pOffset = value.find_first_of(',', isOffset);
					size_t p1Offset = value.find_first_of(',', pOffset + 1);
					if (p1Offset == std::string::npos)
					{
						p1Offset = value.find_first_of('!', pOffset + 1);
					}
					if (pOffset != std::string::npos && p1Offset != std::string::npos)
					{
						p1 = Parser::string2float(value.substr(pOffset + 1, p1Offset - pOffset - 1).c_str());

						// Cherche p2
						size_t p2Offset = value.find_first_of(',', p1Offset + 1);
						if (p2Offset == std::string::npos)
						{
							p2Offset = value.find_first_of('!', p1Offset + 1);
						}
						if (p2Offset != std::string::npos)
						{
							p2 = Parser::string2float(value.substr(p1Offset + 1, p2Offset - p1Offset - 1).c_str());

							// Cherche p3
							size_t p3Offset = value.find_first_of('!', p2Offset + 1);
							if (p3Offset != std::string::npos)
							{
								p3 = Parser::string2float(value.substr(p2Offset + 1, p3Offset - p2Offset - 1).c_str());
							}
						}
					}

					// dynamicBox
					if (isdb != std::string::npos)
					{
						// Récupère les propriétés
						float density = (p1 != -1.f) ? p1 : 1.f;
						float friction = (p2 != -1.f) ? p2 : 0.2f;
						float restitution = (p3 != -1.f) ? p3 : 0.f;

						// Crée le body
						b = new DynamicBox(mLevel->mWorld, posRot, (*mLevel->mTextureMap)[texture], density, friction, restitution);
						b->SetType(bt);
						mLevel->mWorld->RegisterBody(b);
					}
					// dynamicCircle
					else if (isdc != std::string::npos)
					{
						// Récupère les propriétés
						float density = (p1 != -1.f) ? p1 : 1.f;
						float friction = (p2 != -1.f) ? p2 : 0.2f;
						float restitution = (p3 != -1.f) ? p3 : 0.f;

						// Crée le body
						b = new DynamicCircle(mLevel->mWorld, posRot, (*mLevel->mTextureMap)[texture], density, friction, restitution);
						b->SetType(bt);
						mLevel->mWorld->RegisterBody(b);
					}
					// staticBox
					else if (issb != std::string::npos)
					{
						// Récupère les propriétés
						float friction = (p1 != -1.f) ? p1 : 0.2f;
						float restitution = (p2 != -1.f) ? p2 : 0.f;

						// Crée le body
						b = new StaticBox(mLevel->mWorld, posRot, (*mLevel->mTextureMap)[texture], friction, restitution);
						b->SetType(bt);
						mLevel->mWorld->RegisterBody(b);
					}
					// kinematicBox
					else if (iskb != std::string::npos)
					{
						// Récupère les propriétés
						float restitution = (p1 != -1.f) ? p1 : 0.f;

						// Crée le body
						b = new KinematicBox(mLevel->mWorld, posRot, (*mLevel->mTextureMap)[texture], restitution);
						b->SetType(bt);
						mLevel->mWorld->RegisterBody(b);
					}

					// Enregiste l'ID
					if (b != nullptr && IDexists)
						mBodyIDMap[id] = b;

					return true;
				}

				Dialog::Error("ConfigReader::CreateLine() - ["+ section +"]\n-> body type unknow!");
				return false;
			}

			Dialog::Error("ConfigReader::CreateLine() - ["+ section +"]\n-> texture \""+ name +"\" unknow!");
			return false;
		}

		Dialog::Error("ConfigReader::CreateLine() - ["+ section +"]\n-> ID \""+ Parser::uint2string(id) +"\" is not unique!");
		return false;
	}

	// [joints] = Crée les joints
	/*
		# type,ID(facultatif et unique) = proprietés
		# (les dernières propriétés() sont facultatives comme les paramètres C++)
		# types : ( + propriétés) + "!" à la fin // ((col = collideConnected))
		# - dj = distanceJoint + b1, p1, b2, p2, frequency(4.0f), damping(0.5f), col(true), color(magenta)
		# - fj = frictionJoint + b1, p1, b2, p2, maxForce(0.f), maxTorque(0.f), col(true), color(magenta)
		# - gj = gearJoint + b1, b2, j1, j2, ratio(1.f), col(true), color(80, 0, 200)
		# - prj = prismaticJoint + b1, b2, anchorRel2A, axis, enableLimits(false), lower(0.f), upper(0.f)
		#													, enableMotor(false), speed(0.f), maxForce(10.f),
		#													, col(false), color(green)
		# - puj = pulleyJoint + b1, p1, b2, p2, groundP1, groundP2, ratio(1.f), col(true), color(170, 80, 0)
		# - rej = revoluteJoint + b1, b2, anchorRel2A, enableLimits(false), lower(0.f), upper(0.f)
		#											 , enableMotor(false), speed(0.f), maxTorque(10.f)
		#											 , col(false), color(green)
		# - roj = ropeJoint + b1, p1, b2, p2, maxLength, col(true), color(magenta)
		# - wej = weldJoint + b1, b2, anchorRel2A, frequency(4.f), damping(0.5f), col(true), color(magenta)
		# - whj = wheelJoint + car, wheel, pWheel, axis, frequency(4.f), damping(0.5f)
		#											   , enableMotor(false), speed(0.f), maxTorque(10.f)
		#											   , col(false), color(magenta)

		# ex : puj = 4, (0.f, 0.f), 2, (0.f, 0.f), (-1.f, 2.f), (1.f, 2.f), 1.5f!
	*/
	else if (section == "joints")
	{
		// Récupère l'ID et le type
		bool IDexists = false;
		unsigned int id = 0;
		size_t idOffset = name.find_first_of(',');
		std::string type = name.substr(0, idOffset).c_str();
		if (idOffset != std::string::npos)
		{
			id = Parser::string2uint(name.substr(idOffset + 1).c_str());
			IDexists = true;
		}

		if (!IDexists || mJointIDMap.find(id) == mJointIDMap.end())
		{
			// Récupère le type
			size_t isdj = name.find("dj");
			size_t isfj = name.find("fj");
			size_t isgj = name.find("gj");
			size_t isprj = name.find("prj");
			size_t ispuj = name.find("puj");
			size_t isrej = name.find("rej");
			size_t isroj = name.find("roj");
			size_t iswej = name.find("wej");
			size_t iswhj = name.find("whj");
			size_t list[] = { isdj, isfj, isgj, isprj, ispuj, isrej, isroj, iswej, iswhj };
			size_t isOffset = first_not_of(list, 9, std::string::npos);

			if (isOffset != std::string::npos)
			{
				Joint *j = nullptr;

				// Cherche les virgules (12 max)
				std::vector<size_t> commaOffsets;
				size_t offset = 0;
				while ((offset = value.find_first_of("_!", offset + 1)) != std::string::npos)
				{
					commaOffsets.push_back(offset);
				}

				// distanceJoint
				if (isdj != std::string::npos)
				{
					// Récupère les propriétés
					unsigned IDb1 = 0, IDb2 = 0;
					Body *b1 = nullptr;
					b2Vec2 p1;
					Body *b2 = nullptr;
					b2Vec2 p2;
					float frequencyHz = 4.f;
					float damping = 5.f;
					bool col = true;
					sf::Color color = sf::Color::Magenta;

					// Vérifie que les propriétés obligatoires soient présentes
					if (commaOffsets.size() < 4)
					{
						Dialog::Error("ConfigReader::CreateLine()\n-> "+ Parser::int2string(4 - commaOffsets.size()) +" required properties missing!");
						return false;
					}

					// Récupère les propriétés obligatoires
					IDb1 = Parser::string2uint(value.substr(0, commaOffsets[0]).c_str());
					p1 = Parser::string2b2Vec2(value.substr(commaOffsets[0] + 1, commaOffsets[1] - commaOffsets[0] - 1).c_str());
					IDb2 = Parser::string2uint(value.substr(commaOffsets[1] + 1, commaOffsets[2] - commaOffsets[1] - 1).c_str());
					p2 = Parser::string2b2Vec2(value.substr(commaOffsets[2] + 1, commaOffsets[3] - commaOffsets[2] - 1).c_str());

					// Récupère les bodies
					if (mBodyIDMap.find(IDb1) == mBodyIDMap.end())
					{
						Dialog::Error("ConfigReader::CreateLine()\n-> body with ID "+ Parser::uint2string(IDb1) +" missing!");
						return false;
					}
					if (mBodyIDMap.find(IDb2) == mBodyIDMap.end())
					{
						Dialog::Error("ConfigReader::CreateLine()\n-> body with ID "+ Parser::uint2string(IDb2) +" missing!");
						return false;
					}
					b1 = mBodyIDMap[IDb1];
					b2 = mBodyIDMap[IDb2];

					// Récupère les propriétés facultatives
					for (unsigned int i = 4; i < commaOffsets.size() && i < 8; ++i)
					{
						if (i == 4) // frequencyHz
							frequencyHz = Parser::string2float(value.substr(commaOffsets[i - 1] + 1, commaOffsets[i] - commaOffsets[i - 1] - 1).c_str());
						else if (i == 5) // damping
							damping = Parser::string2float(value.substr(commaOffsets[i - 1] + 1, commaOffsets[i] - commaOffsets[i - 1] - 1).c_str());
						else if (i == 6) // collideConnected
							col = Parser::string2bool(value.substr(commaOffsets[i - 1] + 1, commaOffsets[i] - commaOffsets[i - 1] - 1).c_str());
						else if (i == 7) // color
							color = Parser::string2color(value.substr(commaOffsets[i - 1] + 1, commaOffsets[i] - commaOffsets[i - 1] - 1).c_str());
					}

					// Crée le joint
					j = new DistanceJoint(mLevel->mWorld, b1, p1, b2, p2, frequencyHz, damping, col, color);
					mLevel->mWorld->RegisterJoint(j);
				}
				
				// frictionJoint
				else if (isfj != std::string::npos)
				{
					// Récupère les propriétés
					unsigned IDb1 = 0, IDb2 = 0;
					Body *b1 = nullptr;
					b2Vec2 p1;
					Body *b2 = nullptr;
					b2Vec2 p2;
					float maxForce = 0.f;
					float maxTorque = 0.f;
					bool col = true;
					sf::Color color = sf::Color::Magenta;

					// Vérifie que les propriétés obligatoires soient présentes
					if (commaOffsets.size() < 4)
					{
						Dialog::Error("ConfigReader::CreateLine()\n-> "+ Parser::int2string(4 - commaOffsets.size()) +" required properties missing!");
						return false;
					}

					// Récupère les propriétés obligatoires
					IDb1 = Parser::string2uint(value.substr(0, commaOffsets[0]).c_str());
					p1 = Parser::string2b2Vec2(value.substr(commaOffsets[0] + 1, commaOffsets[1] - commaOffsets[0] - 1).c_str());
					IDb2 = Parser::string2uint(value.substr(commaOffsets[1] + 1, commaOffsets[2] - commaOffsets[1] - 1).c_str());
					p2 = Parser::string2b2Vec2(value.substr(commaOffsets[2] + 1, commaOffsets[3] - commaOffsets[2] - 1).c_str());

					// Récupère les bodies
					if (mBodyIDMap.find(IDb1) == mBodyIDMap.end())
					{
						Dialog::Error("ConfigReader::CreateLine()\n-> body with ID "+ Parser::uint2string(IDb1) +" missing!");
						return false;
					}
					if (mBodyIDMap.find(IDb2) == mBodyIDMap.end())
					{
						Dialog::Error("ConfigReader::CreateLine()\n-> body with ID "+ Parser::uint2string(IDb2) +" missing!");
						return false;
					}
					b1 = mBodyIDMap[IDb1];
					b2 = mBodyIDMap[IDb2];

					// Récupère les propriétés facultatives
					for (unsigned int i = 4; i < commaOffsets.size() && i < 8; ++i)
					{
						if (i == 4) // maxForce
							maxForce = Parser::string2float(value.substr(commaOffsets[i - 1] + 1, commaOffsets[i] - commaOffsets[i - 1] - 1).c_str());
						else if (i == 5) // maxTorque
							maxTorque = Parser::string2float(value.substr(commaOffsets[i - 1] + 1, commaOffsets[i] - commaOffsets[i - 1] - 1).c_str());
						else if (i == 6) // collideConnected
							col = Parser::string2bool(value.substr(commaOffsets[i - 1] + 1, commaOffsets[i] - commaOffsets[i - 1] - 1).c_str());
						else if (i == 7) // color
							color = Parser::string2color(value.substr(commaOffsets[i - 1] + 1, commaOffsets[i] - commaOffsets[i - 1] - 1).c_str());
					}

					// Crée le joint
					j = new FrictionJoint(mLevel->mWorld, b1, p1, b2, p2, maxForce, maxTorque, col, color);
					mLevel->mWorld->RegisterJoint(j);
				}

				// gearJoint
				else if (isgj != std::string::npos)
				{
					// Récupère les propriétés
					unsigned IDb1 = 0, IDb2 = 0;
					Body *b1 = nullptr;
					Body *b2 = nullptr;
					unsigned IDj1 = 0, IDj2 = 0;
					Joint *j1 = nullptr;
					Joint *j2 = nullptr;
					float ratio = 1.f;
					bool col = true;
					sf::Color color = sf::Color(80, 0, 200);

					// Vérifie que les propriétés obligatoires soient présentes
					if (commaOffsets.size() < 4)
					{
						Dialog::Error("ConfigReader::CreateLine()\n-> "+ Parser::int2string(4 - commaOffsets.size()) +" required properties missing!");
						return false;
					}

					// Récupère les propriétés obligatoires
					IDb1 = Parser::string2uint(value.substr(0, commaOffsets[0]).c_str());
					IDb2 = Parser::string2uint(value.substr(commaOffsets[0] + 1, commaOffsets[1] - commaOffsets[0] - 1).c_str());
					IDj1 = Parser::string2uint(value.substr(commaOffsets[1] + 1, commaOffsets[2] - commaOffsets[1] - 1).c_str());
					IDj2 = Parser::string2uint(value.substr(commaOffsets[2] + 1, commaOffsets[3] - commaOffsets[2] - 1).c_str());
					
					// Récupère les bodies
					if (mBodyIDMap.find(IDb1) == mBodyIDMap.end())
					{
						Dialog::Error("ConfigReader::CreateLine()\n-> body with ID "+ Parser::uint2string(IDb1) +" missing!");
						return false;
					}
					if (mBodyIDMap.find(IDb2) == mBodyIDMap.end())
					{
						Dialog::Error("ConfigReader::CreateLine()\n-> body with ID "+ Parser::uint2string(IDb2) +" missing!");
						return false;
					}
					b1 = mBodyIDMap[IDb1];
					b2 = mBodyIDMap[IDb2];

					// Récupère les joints
					if (mJointIDMap.find(IDj1) == mJointIDMap.end())
					{
						Dialog::Error("ConfigReader::CreateLine()\n-> joint with ID "+ Parser::uint2string(IDb1) +" missing!");
						return false;
					}
					if (mJointIDMap.find(IDj2) == mJointIDMap.end())
					{
						Dialog::Error("ConfigReader::CreateLine()\n-> joint with ID "+ Parser::uint2string(IDb2) +" missing!");
						return false;
					}
					j1 = mJointIDMap[IDj1];
					j2 = mJointIDMap[IDj2];

					// Récupère les propriétés facultatives
					for (unsigned int i = 4; i < commaOffsets.size() && i < 7; ++i)
					{
						if (i == 4) // ratio
							ratio = Parser::string2float(value.substr(commaOffsets[i - 1] + 1, commaOffsets[i] - commaOffsets[i - 1] - 1).c_str());
						else if (i == 5) // collideConnected
							col = Parser::string2bool(value.substr(commaOffsets[i - 1] + 1, commaOffsets[i] - commaOffsets[i - 1] - 1).c_str());
						else if (i == 6) // color
							color = Parser::string2color(value.substr(commaOffsets[i - 1] + 1, commaOffsets[i] - commaOffsets[i - 1] - 1).c_str());
					}

					// Crée le joint
					j = new GearJoint(mLevel->mWorld, b1, b2, j1, j2, ratio, col, color);
					mLevel->mWorld->RegisterJoint(j);
				}

				// prismaticJoint
				else if (isprj != std::string::npos)
				{
					// Récupère les propriétés
					unsigned IDb1 = 0, IDb2 = 0;
					Body *b1 = nullptr;
					Body *b2 = nullptr;
					b2Vec2 anchor;
					b2Vec2 axis;
					bool enableLimits = false;
					float lower = 0.f;
					float upper = 0.f;
					bool enableMotor = false;
					float speed = 0.f;
					float maxForce = 10.f;
					bool col = false;
					sf::Color color = sf::Color::Green;

					// Vérifie que les propriétés obligatoires soient présentes
					if (commaOffsets.size() < 4)
					{
						Dialog::Error("ConfigReader::CreateLine()\n-> "+ Parser::int2string(4 - commaOffsets.size()) +" required properties missing!");
						return false;
					}

					// Récupère les propriétés obligatoires
					IDb1 = Parser::string2uint(value.substr(0, commaOffsets[0]).c_str());
					IDb2 = Parser::string2uint(value.substr(commaOffsets[0] + 1, commaOffsets[1] - commaOffsets[0] - 1).c_str());
					anchor = Parser::string2b2Vec2(value.substr(commaOffsets[1] + 1, commaOffsets[2] - commaOffsets[1] - 1).c_str());
					axis = Parser::string2b2Vec2(value.substr(commaOffsets[2] + 1, commaOffsets[3] - commaOffsets[2] - 1).c_str());

					// Récupère les bodies
					if (mBodyIDMap.find(IDb1) == mBodyIDMap.end())
					{
						Dialog::Error("ConfigReader::CreateLine()\n-> body with ID "+ Parser::uint2string(IDb1) +" missing!");
						return false;
					}
					if (mBodyIDMap.find(IDb2) == mBodyIDMap.end())
					{
						Dialog::Error("ConfigReader::CreateLine()\n-> body with ID "+ Parser::uint2string(IDb2) +" missing!");
						return false;
					}
					b1 = mBodyIDMap[IDb1];
					b2 = mBodyIDMap[IDb2];

					// Récupère les propriétés facultatives
					for (unsigned int i = 4; i < commaOffsets.size() && i < 12; ++i)
					{
						if (i == 4) // enableLimits
							enableLimits = Parser::string2bool(value.substr(commaOffsets[i - 1] + 1, commaOffsets[i] - commaOffsets[i - 1] - 1).c_str());
						else if (i == 5) // lower
							lower = Parser::string2float(value.substr(commaOffsets[i - 1] + 1, commaOffsets[i] - commaOffsets[i - 1] - 1).c_str());
						else if (i == 6) // upper
							upper = Parser::string2float(value.substr(commaOffsets[i - 1] + 1, commaOffsets[i] - commaOffsets[i - 1] - 1).c_str());

						if (i == 7) // enableMotor
							enableMotor = Parser::string2bool(value.substr(commaOffsets[i - 1] + 1, commaOffsets[i] - commaOffsets[i - 1] - 1).c_str());
						else if (i == 8) // speed
							speed = Parser::string2float(value.substr(commaOffsets[i - 1] + 1, commaOffsets[i] - commaOffsets[i - 1] - 1).c_str());
						else if (i == 9) // maxForce
							maxForce = Parser::string2float(value.substr(commaOffsets[i - 1] + 1, commaOffsets[i] - commaOffsets[i - 1] - 1).c_str());

						else if (i == 10) // collideConnected
							col = Parser::string2bool(value.substr(commaOffsets[i - 1] + 1, commaOffsets[i] - commaOffsets[i - 1] - 1).c_str());
						else if (i == 11) // color
							color = Parser::string2color(value.substr(commaOffsets[i - 1] + 1, commaOffsets[i] - commaOffsets[i - 1] - 1).c_str());
					}

					// Crée le joint
					j = new PrismaticJoint(mLevel->mWorld, b1, b2, anchor, axis, enableLimits, lower, upper, enableMotor, speed, maxForce, col, color);
					mLevel->mWorld->RegisterJoint(j);
				}

				// pulleyJoint
				else if (ispuj != std::string::npos)
				{
					// Récupère les propriétés
					unsigned IDb1 = 0, IDb2 = 0;
					Body *b1 = nullptr;
					b2Vec2 p1;
					Body *b2 = nullptr;
					b2Vec2 p2;
					b2Vec2 groundP1;
					b2Vec2 groundP2;
					float ratio = 1.f;
					bool col = true;
					sf::Color color = sf::Color(170, 80, 0);

					// Vérifie que les propriétés obligatoires soient présentes
					if (commaOffsets.size() < 6)
					{
						Dialog::Error("ConfigReader::CreateLine()\n-> "+ Parser::int2string(6 - commaOffsets.size()) +" required properties missing!");
						return false;
					}

					// Récupère les propriétés obligatoires
					IDb1 = Parser::string2uint(value.substr(0, commaOffsets[0]).c_str());
					p1 = Parser::string2b2Vec2(value.substr(commaOffsets[0] + 1, commaOffsets[1] - commaOffsets[0] - 1).c_str());
					IDb2 = Parser::string2uint(value.substr(commaOffsets[1] + 1, commaOffsets[2] - commaOffsets[1] - 1).c_str());
					p2 = Parser::string2b2Vec2(value.substr(commaOffsets[2] + 1, commaOffsets[3] - commaOffsets[2] - 1).c_str());
					groundP1 = Parser::string2b2Vec2(value.substr(commaOffsets[3] + 1, commaOffsets[4] - commaOffsets[3] - 1).c_str());
					groundP2 = Parser::string2b2Vec2(value.substr(commaOffsets[4] + 1, commaOffsets[5] - commaOffsets[4] - 1).c_str());

					// Récupère les bodies
					if (mBodyIDMap.find(IDb1) == mBodyIDMap.end())
					{
						Dialog::Error("ConfigReader::CreateLine()\n-> body with ID "+ Parser::uint2string(IDb1) +" missing!");
						return false;
					}
					if (mBodyIDMap.find(IDb2) == mBodyIDMap.end())
					{
						Dialog::Error("ConfigReader::CreateLine()\n-> body with ID "+ Parser::uint2string(IDb2) +" missing!");
						return false;
					}
					b1 = mBodyIDMap[IDb1];
					b2 = mBodyIDMap[IDb2];

					// Récupère les propriétés facultatives
					for (unsigned int i = 6; i < commaOffsets.size() && i < 9; ++i)
					{
						if (i == 6) // ratio
							ratio = Parser::string2float(value.substr(commaOffsets[i - 1] + 1, commaOffsets[i] - commaOffsets[i - 1] - 1).c_str());
						else if (i == 7) // collideConnected
							col = Parser::string2bool(value.substr(commaOffsets[i - 1] + 1, commaOffsets[i] - commaOffsets[i - 1] - 1).c_str());
						else if (i == 8) // color
							color = Parser::string2color(value.substr(commaOffsets[i - 1] + 1, commaOffsets[i] - commaOffsets[i - 1] - 1).c_str());
					}

					// Crée le joint
					j = new PulleyJoint(mLevel->mWorld, b1, p1, b2, p2, groundP1, groundP2, ratio, col, color);
					mLevel->mWorld->RegisterJoint(j);
				}

				// revoluteJoint
				else if (isrej != std::string::npos)
				{
					// Récupère les propriétés
					unsigned IDb1 = 0, IDb2 = 0;
					Body *b1 = nullptr;
					Body *b2 = nullptr;
					b2Vec2 anchor;
					bool enableLimits = false;
					float lower = 0.f;
					float upper = 0.f;
					bool enableMotor = false;
					float speed = 0.f;
					float maxTorque = 10.f;
					bool col = false;
					sf::Color color = sf::Color::Green;

					// Vérifie que les propriétés obligatoires soient présentes
					if (commaOffsets.size() < 3)
					{
						Dialog::Error("ConfigReader::CreateLine()\n-> "+ Parser::int2string(3 - commaOffsets.size()) +" required properties missing!");
						return false;
					}

					// Récupère les propriétés obligatoires
					IDb1 = Parser::string2uint(value.substr(0, commaOffsets[0]).c_str());
					IDb2 = Parser::string2uint(value.substr(commaOffsets[0] + 1, commaOffsets[1] - commaOffsets[0] - 1).c_str());
					anchor = Parser::string2b2Vec2(value.substr(commaOffsets[1] + 1, commaOffsets[2] - commaOffsets[1] - 1).c_str());

					// Récupère les bodies
					if (mBodyIDMap.find(IDb1) == mBodyIDMap.end())
					{
						Dialog::Error("ConfigReader::CreateLine()\n-> body with ID "+ Parser::uint2string(IDb1) +" missing!");
						return false;
					}
					if (mBodyIDMap.find(IDb2) == mBodyIDMap.end())
					{
						Dialog::Error("ConfigReader::CreateLine()\n-> body with ID "+ Parser::uint2string(IDb2) +" missing!");
						return false;
					}
					b1 = mBodyIDMap[IDb1];
					b2 = mBodyIDMap[IDb2];

					// Récupère les propriétés facultatives
					for (unsigned int i = 3; i < commaOffsets.size() && i < 11; ++i)
					{
						if (i == 3) // enableLimits
							enableLimits = Parser::string2bool(value.substr(commaOffsets[i - 1] + 1, commaOffsets[i] - commaOffsets[i - 1] - 1).c_str());
						else if (i == 4) // lower
							lower = Parser::string2float(value.substr(commaOffsets[i - 1] + 1, commaOffsets[i] - commaOffsets[i - 1] - 1).c_str());
						else if (i == 5) // upper
							upper = Parser::string2float(value.substr(commaOffsets[i - 1] + 1, commaOffsets[i] - commaOffsets[i - 1] - 1).c_str());

						if (i == 6) // enableMotor
							enableMotor = Parser::string2bool(value.substr(commaOffsets[i - 1] + 1, commaOffsets[i] - commaOffsets[i - 1] - 1).c_str());
						else if (i == 7) // speed
							speed = Parser::string2float(value.substr(commaOffsets[i - 1] + 1, commaOffsets[i] - commaOffsets[i - 1] - 1).c_str());
						else if (i == 8) // maxTorque
							maxTorque = Parser::string2float(value.substr(commaOffsets[i - 1] + 1, commaOffsets[i] - commaOffsets[i - 1] - 1).c_str());

						else if (i == 9) // collideConnected
							col = Parser::string2bool(value.substr(commaOffsets[i - 1] + 1, commaOffsets[i] - commaOffsets[i - 1] - 1).c_str());
						else if (i == 10) // color
							color = Parser::string2color(value.substr(commaOffsets[i - 1] + 1, commaOffsets[i] - commaOffsets[i - 1] - 1).c_str());
					}

					// Crée le joint
					j = new RevoluteJoint(mLevel->mWorld, b1, b2, anchor, enableLimits, lower, upper, enableMotor, speed, maxTorque, col, color);
					mLevel->mWorld->RegisterJoint(j);
				}

				// ropeJoint
				else if (isroj != std::string::npos)
				{
					// Récupère les propriétés
					unsigned IDb1 = 0, IDb2 = 0;
					Body *b1 = nullptr;
					b2Vec2 p1;
					Body *b2 = nullptr;
					b2Vec2 p2;
					float maxLength;
					bool col = true;
					sf::Color color = sf::Color::Magenta;

					// Vérifie que les propriétés obligatoires soient présentes
					if (commaOffsets.size() < 5)
					{
						Dialog::Error("ConfigReader::CreateLine()\n-> "+ Parser::int2string(5 - commaOffsets.size()) +" required properties missing!");
						return false;
					}

					// Récupère les propriétés obligatoires
					IDb1 = Parser::string2uint(value.substr(0, commaOffsets[0]).c_str());
					p1 = Parser::string2b2Vec2(value.substr(commaOffsets[0] + 1, commaOffsets[1] - commaOffsets[0] - 1).c_str());
					IDb2 = Parser::string2uint(value.substr(commaOffsets[1] + 1, commaOffsets[2] - commaOffsets[1] - 1).c_str());
					p2 = Parser::string2b2Vec2(value.substr(commaOffsets[2] + 1, commaOffsets[3] - commaOffsets[2] - 1).c_str());
					maxLength = Parser::string2float(value.substr(commaOffsets[3] + 1, commaOffsets[4] - commaOffsets[3] - 1).c_str());

					// Récupère les bodies
					if (mBodyIDMap.find(IDb1) == mBodyIDMap.end())
					{
						Dialog::Error("ConfigReader::CreateLine()\n-> body with ID "+ Parser::uint2string(IDb1) +" missing!");
						return false;
					}
					if (mBodyIDMap.find(IDb2) == mBodyIDMap.end())
					{
						Dialog::Error("ConfigReader::CreateLine()\n-> body with ID "+ Parser::uint2string(IDb2) +" missing!");
						return false;
					}
					b1 = mBodyIDMap[IDb1];
					b2 = mBodyIDMap[IDb2];

					// Récupère les propriétés facultatives
					for (unsigned int i = 5; i < commaOffsets.size() && i < 7; ++i)
					{
						if (i == 5) // collideConnected
							col = Parser::string2bool(value.substr(commaOffsets[i - 1] + 1, commaOffsets[i] - commaOffsets[i - 1] - 1).c_str());
						else if (i == 6) // color
							color = Parser::string2color(value.substr(commaOffsets[i - 1] + 1, commaOffsets[i] - commaOffsets[i - 1] - 1).c_str());
					}

					// Crée le joint
					j = new RopeJoint(mLevel->mWorld, b1, p1, b2, p2, maxLength, col, color);
					mLevel->mWorld->RegisterJoint(j);
				}

				// weldJoint
				else if (iswej != std::string::npos)
				{
					// Récupère les propriétés
					unsigned IDb1 = 0, IDb2 = 0;
					Body *b1 = nullptr;
					Body *b2 = nullptr;
					b2Vec2 anchor;
					float frequencyHz = 4.f;
					float damping = 5.f;
					bool col = false;
					sf::Color color = sf::Color::Magenta;

					// Vérifie que les propriétés obligatoires soient présentes
					if (commaOffsets.size() < 3)
					{
						Dialog::Error("ConfigReader::CreateLine()\n-> "+ Parser::int2string(3 - commaOffsets.size()) +" required properties missing!");
						return false;
					}

					// Récupère les propriétés obligatoires
					IDb1 = Parser::string2uint(value.substr(0, commaOffsets[0]).c_str());
					IDb2 = Parser::string2uint(value.substr(commaOffsets[0] + 1, commaOffsets[1] - commaOffsets[0] - 1).c_str());
					anchor = Parser::string2b2Vec2(value.substr(commaOffsets[1] + 1, commaOffsets[2] - commaOffsets[1] - 1).c_str());

					// Récupère les bodies
					if (mBodyIDMap.find(IDb1) == mBodyIDMap.end())
					{
						Dialog::Error("ConfigReader::CreateLine()\n-> body with ID "+ Parser::uint2string(IDb1) +" missing!");
						return false;
					}
					if (mBodyIDMap.find(IDb2) == mBodyIDMap.end())
					{
						Dialog::Error("ConfigReader::CreateLine()\n-> body with ID "+ Parser::uint2string(IDb2) +" missing!");
						return false;
					}
					b1 = mBodyIDMap[IDb1];
					b2 = mBodyIDMap[IDb2];

					// Récupère les propriétés facultatives
					for (unsigned int i = 3; i < commaOffsets.size() && i < 7; ++i)
					{
						if (i == 3) // frequencyHz
							frequencyHz = Parser::string2float(value.substr(commaOffsets[i - 1] + 1, commaOffsets[i] - commaOffsets[i - 1] - 1).c_str());
						else if (i == 4) // damping
							damping = Parser::string2float(value.substr(commaOffsets[i - 1] + 1, commaOffsets[i] - commaOffsets[i - 1] - 1).c_str());
						else if (i == 5) // collideConnected
							col = Parser::string2bool(value.substr(commaOffsets[i - 1] + 1, commaOffsets[i] - commaOffsets[i - 1] - 1).c_str());
						else if (i == 6) // color
							color = Parser::string2color(value.substr(commaOffsets[i - 1] + 1, commaOffsets[i] - commaOffsets[i - 1] - 1).c_str());
					}

					// Crée le joint
					j = new WeldJoint(mLevel->mWorld, b1, b2, anchor, frequencyHz, damping, col, color);
					mLevel->mWorld->RegisterJoint(j);
				}

				// wheelJoint
				else if (iswhj != std::string::npos)
				{
					// Récupère les propriétés
					unsigned IDb1 = 0, IDb2 = 0;
					Body *car = nullptr;
					Body *wheel = nullptr;
					b2Vec2 pWheel;
					b2Vec2 axis;
					float frequencyHz = 4.f;
					float damping = 5.f;
					bool enableMotor = false;
					float speed = 0.f;
					float maxTorque = 10.f;
					bool col = false;
					sf::Color color = sf::Color::Magenta;

					// Vérifie que les propriétés obligatoires soient présentes
					if (commaOffsets.size() < 4)
					{
						Dialog::Error("ConfigReader::CreateLine()\n-> "+ Parser::int2string(4 - commaOffsets.size()) +" required properties missing!");
						return false;
					}

					// Récupère les propriétés obligatoires
					IDb1 = Parser::string2uint(value.substr(0, commaOffsets[0]).c_str());
					IDb2 = Parser::string2uint(value.substr(commaOffsets[0] + 1, commaOffsets[1] - commaOffsets[0] - 1).c_str());
					pWheel = Parser::string2b2Vec2(value.substr(commaOffsets[1] + 1, commaOffsets[2] - commaOffsets[1] - 1).c_str());
					axis = Parser::string2b2Vec2(value.substr(commaOffsets[2] + 1, commaOffsets[3] - commaOffsets[2] - 1).c_str());

					// Récupère les bodies
					if (mBodyIDMap.find(IDb1) == mBodyIDMap.end())
					{
						Dialog::Error("ConfigReader::CreateLine()\n-> body with ID "+ Parser::uint2string(IDb1) +" missing!");
						return false;
					}
					if (mBodyIDMap.find(IDb2) == mBodyIDMap.end())
					{
						Dialog::Error("ConfigReader::CreateLine()\n-> body with ID "+ Parser::uint2string(IDb2) +" missing!");
						return false;
					}
					car = mBodyIDMap[IDb1];
					wheel = mBodyIDMap[IDb2];

					// Récupère les propriétés facultatives
					for (unsigned int i = 4; i < commaOffsets.size() && i < 11; ++i)
					{
						if (i == 4) // frequencyHz
							frequencyHz = Parser::string2float(value.substr(commaOffsets[i - 1] + 1, commaOffsets[i] - commaOffsets[i - 1] - 1).c_str());
						else if (i == 5) // damping
							damping = Parser::string2float(value.substr(commaOffsets[i - 1] + 1, commaOffsets[i] - commaOffsets[i - 1] - 1).c_str());

						if (i == 6) // enableMotor
							enableMotor = Parser::string2bool(value.substr(commaOffsets[i - 1] + 1, commaOffsets[i] - commaOffsets[i - 1] - 1).c_str());
						else if (i == 7) // speed
							speed = Parser::string2float(value.substr(commaOffsets[i - 1] + 1, commaOffsets[i] - commaOffsets[i - 1] - 1).c_str());
						else if (i == 8) // maxTorque
							maxTorque = Parser::string2float(value.substr(commaOffsets[i - 1] + 1, commaOffsets[i] - commaOffsets[i - 1] - 1).c_str());

						else if (i == 9) // collideConnected
							col = Parser::string2bool(value.substr(commaOffsets[i - 1] + 1, commaOffsets[i] - commaOffsets[i - 1] - 1).c_str());
						else if (i == 10) // color
							color = Parser::string2color(value.substr(commaOffsets[i - 1] + 1, commaOffsets[i] - commaOffsets[i - 1] - 1).c_str());
					}

					// Crée le joint
					j = new WheelJoint(mLevel->mWorld, car, wheel, pWheel, axis, frequencyHz, damping, enableMotor, speed, maxTorque, col, color);
					mLevel->mWorld->RegisterJoint(j);
				}

				// Enregiste l'ID
				if (j != nullptr && IDexists)
					mJointIDMap[id] = j;

				return true;
			}

			Dialog::Error("ConfigReader::CreateLine() - ["+ section +"]\n-> joint type unknow!");
			return false;
		}

		Dialog::Error("ConfigReader::CreateLine() - ["+ section +"]\n-> ID \""+ Parser::uint2string(id) +"\" is not unique!");
		return false;
	}

	// [deco] = Création des textures sans colisions
	/*
		# Crée des textures sans colisions
		# "deco_" + nombre == z-index // parallax
		# ex: [deco_5]
		[deco_5]
		#   texture = (position + rotation° facultative), z-index

		# ex : lampadere = (5.2f, 9.f, 45.f), 5
	*/
	else if (section.find("deco") != std::string::npos)
	{
		std::string texture = name;
		
		// Récupère le z-index de la déco
		int levelZindex = 1;
		size_t decoZindexOffset = section.find_first_of('_');
		if (decoZindexOffset != std::string::npos)
		{
			levelZindex = Parser::string2int(section.substr(decoZindexOffset + 1).c_str());
		}

		// Vérifie si la texture existe
		if (mLevel->mTextureMap->find(name) != mLevel->mTextureMap->end())
		{
			// Récupère la position et rotation
			size_t posRotOffset = value.find_first_of(')');
			if (posRotOffset != std::string::npos)
			{
				b2Vec3 posRot = Parser::string2b2Vec3(value.substr(0, posRotOffset + 1).c_str());

				// Récupère le z-index
				int zindex = -1;
				size_t zindexOffset = value.find_first_of(',', value.find_last_of(')') + 1);
			
				if (zindexOffset != std::string::npos)
				{
					zindex = Parser::string2int(value.substr(zindexOffset + 1).c_str());

					mLevel->AddDeco(levelZindex, posRot, texture, zindex);

					return true;
				}

				Dialog::Error("ConfigReader::CreateLine() - ["+ section +"]\n-> z-index not found!");
				return false;
			}

			Dialog::Error("ConfigReader::CreateLine() - ["+ section +"]\n-> posRot not found!");
			return false;
		}
		
		Dialog::Error("ConfigReader::CreateLine() - ["+ section +"]\n-> texture \""+ name +"\" unknow!");
		return false;
	}

	// Si on arrive là : section inconnue
	Dialog::Error("ConfigReader::CreateLine()\n-> Section ["+ section +"] unknow!");
	return false;
}
