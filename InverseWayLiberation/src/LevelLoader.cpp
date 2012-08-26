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
#include <Thor/Resources.hpp>

// Ctor
LevelLoader::LevelLoader(std::string const& path, Level *level)
	: mPath(path), mLevel(level)
{
	// Si le level n'est pas valide, on ne fait rien
	if (mLevel->IsValid())
	{
		// Vide le fichier si il est d�j� charg�
		if (mLevel->IsCharged())
			mLevel->Clear();

		// Cr�e et ouvre le fichier
		mFile = new std::fstream(mPath, std::fstream::in);

		// V�rifie si le fichier existe
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

	return true;
}

const std::string LevelLoader::ParseLine(std::string& line, std::string& section, int lineNumber)
{
	// TODO: Utiliser std::string::find_first_of();

	std::string lineSection = section; // valeur retourn�e
	if (line.size() > 1)
	{
		// Saute les espaces en d�but de ligne
		size_t offset = 0;
		for (; offset < line.size() && (line[offset] == ' ' || line[offset] == '\t'); ++offset);

		// Ignore les commentaires ("#" or ";")
		if (line[offset] != '#' && line[offset] != ';')
		{
			// V�rifie si c'est le d�but d'une section
			if (line[offset] == '[')
			{
				// Passe le caract�re '['
				offset++;

				// Saute les espaces en d�but du nom de la section
				for (; offset < line.size() && line[offset] == ' ' || line[offset] == '\t'; ++offset);

				// Trouve le nom de la section
				std::string _section;
				unsigned int index = 0;
				for (; (offset + index) < line.size() && line[offset + index] != ']'; ++index)
					if (line[offset + index] != ' ' && line[offset + index] != '\t')
						_section += line[offset + index];

				// Supprime les espaces contenus � la fin du nom de la section
				for (unsigned int i = _section.size() -1; i >= 0 && (_section[i] == ' ' || _section[i] == '\t'); --i)
					_section.pop_back();

				// Enregistre la section seulement si on trouve le ']' avant la fin de la ligne
				if ((offset + index) < line.size() && index > 0)
					lineSection = _section;
				else
					Dialog::Error("ConfigReader::ParseLine("+ Parser::int2string(lineNumber) +")\n-> d�limiteur ']' manquant.", true);
			}
			// Lit la paire nom=valeur dans la section courante
			else
			{
				std::string name;

				// Trouve le nom en allant jusqu'au '=' ou ':'
				for (; offset < line.size() && line[offset] != '=' && line[offset] != ':'; ++offset)
					if (line[offset] != ' ' && line[offset] != '\t')
						name += line[offset];

				// Supprime les espaces � la fin du nom
				for (unsigned int i = name.size() -1; i >= 0 && (name[i] == ' ' || name[i] == '\t'); --i)
					name.pop_back();

				// On recherche la valeur seulement si on a trouv� un d�limiteur '=' ou ':'
				if (offset < line.size())
				{
					std::string value;

					// Passe par dessus '=' ou ':'
					offset++;

					// Saute les espaces
					for (; offset < line.size() && (line[offset] == ' ' || line[offset] == '\t'); ++offset);

					// Trouve la valeur en allant jusqu'au d�limiteurs de commentaires ';' ou '#' ou la fin de ligne
					for (; offset < line.size() && line[offset] != '\r' && line[offset] != '\n' && line[offset] != ';' && line[offset] != '#'; ++offset)
						value += line[offset];

					// Supprime les espaces � la fin de la valeur
					for (unsigned int i = value.size() -1; i >= 0 && (value[i] == ' ' || value[i] == '\t'); --i)
						value.pop_back();
					
					// Cr�e la ligne obtenue
					CreateLine(lineSection, name, value);
				}
				else
					Dialog::Error("ConfigReader::ParseLine("+ Parser::int2string(lineNumber) +")\n-> d�limiteur '=' ou ':' manquant.");
			}
		}
	}
	return lineSection;
}

bool LevelLoader::CreateLine(std::string& section, std::string& name, std::string& value)
{
	// [world] = D�finition du monde
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

	// [bodies] = Cr�ation des bodies
	/*
		#   texture = (position + rotation� facultative) type propriet�s
		# (les propri�t�es sont facultatives comme les param�tres C++)
		# types : ( + propri�t�s)
		#  - db = dynamicBox + densit�(1.f) friction(0.2f) restitution(0.0f)
		#  - dc = dynamicCircle + densit�(1.f) friction(0.2f) restitution(0.0f)
		#  - sb = staticBox + friction(0.2f) restitution(0.0f)
		#  - kb = kinematicBody + restitution(0.0f)

		# ex : box = (5.2f, 9.f, 45.f) db 2.f
	*/
	else if (section == "bodies")
	{
		// V�rifie si la texture existe
		if (mLevel->mTextureMap->find(name) != mLevel->mTextureMap->end())
		{
			// R�cup�re la position et rotation
			size_t posRotOffset = value.find_first_of(')');
			b2Vec3 posRot = Parser::string2b2Vec3(value.substr(0, posRotOffset + 1).c_str());

			// R�cup�re le type
			size_t isdb = value.find("db", posRotOffset);
			size_t isdc = value.find("dc", posRotOffset);
			size_t issb = value.find("sb", posRotOffset);
			size_t iskb = value.find("kb", posRotOffset);
			size_t list[] = {isdb, isdc, issb, iskb};
			size_t isOffset = first_not_of(list, 4, std::string::npos);
			
			if (isOffset != std::string::npos)
			{
				// R�cup�re les propri�t�s
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
					p1 = Parser::string2float(value.substr(pOffset + 1, p1Offset).c_str());

					// Cherche p2
					size_t p2Offset = value.find_first_of(',', p1Offset + 1);
					if (p2Offset == std::string::npos)
					{
						p2Offset = value.find_first_of('!', p1Offset + 1);
					}
					if (p2Offset != std::string::npos)
					{
						p2 = Parser::string2float(value.substr(p1Offset + 1, p2Offset).c_str());

						// Cherche p3
						size_t p3Offset = value.find_first_of('!', p2Offset + 1);
						if (p3Offset != std::string::npos)
						{
							p3 = Parser::string2float(value.substr(p2Offset + 1, p3Offset).c_str());
						}
					}
				}
			
				// dynamicBox
				if (isdb != std::string::npos)
				{
					// R�cup�re les propri�t�s
					float density = (p1 != -1.f) ? p1 : 1.f;
					float friction = (p2 != -1.f) ? p2 : 0.2f;
					float restitution = (p3 != -1.f) ? p3 : 0.f;

					// Cr�e le body
					mLevel->mWorld->RegisterBody(new DynamicBox(mLevel->mWorld, posRot, (*mLevel->mTextureMap)[name], density, friction, restitution));
					return true;
				}
				// dynamicCircle
				else if (isdc != std::string::npos)
				{
					// R�cup�re les propri�t�s
					float density = (p1 != -1.f) ? p1 : 1.f;
					float friction = (p2 != -1.f) ? p2 : 0.2f;
					float restitution = (p3 != -1.f) ? p3 : 0.f;

					// Cr�e le body
					mLevel->mWorld->RegisterBody(new DynamicCircle(mLevel->mWorld, posRot, (*mLevel->mTextureMap)[name], density, friction, restitution));
					return true;
				}
				// staticBox
				else if (issb != std::string::npos)
				{
					// R�cup�re les propri�t�s
					float friction = (p1 != -1.f) ? p1 : 0.2f;
					float restitution = (p2 != -1.f) ? p2 : 0.f;

					// Cr�e le body
					mLevel->mWorld->RegisterBody(new StaticBox(mLevel->mWorld, posRot, (*mLevel->mTextureMap)[name], friction, restitution));
					return true;
				}
				// kinematicBox
				else if (iskb != std::string::npos)
				{
					// R�cup�re les propri�t�s
					float restitution = (p1 != -1.f) ? p1 : 0.f;

					// Cr�e le body
					mLevel->mWorld->RegisterBody(new KinematicBox(mLevel->mWorld, posRot, (*mLevel->mTextureMap)[name], restitution));
					return true;
				}
			}

			Dialog::Error("ConfigReader::CreateLine() - ["+ section +"]\n-> body type unknow!");
			return false;
		}
		
		Dialog::Error("ConfigReader::CreateLine() - ["+ section +"]\n-> texture \""+ name +"\" unknow!");
		return false;
	}

	// [deco] = Cr�ation des textures sans colisions
	/*
		# Cr�e des textures sans colisions
		# "deco_" + nombre == z-index // parallax
		# ex: [deco_5]
		[deco_5]
		#   texture = (position + rotation� facultative), z-index

		# ex : lampadere = (5.2f, 9.f, 45.f), 5
	*/
	else if (section.find("deco") != std::string::npos)
	{
		std::string texture = name;
		
		// R�cup�re le z-index de la d�co
		int levelZindex = 1;
		size_t decoZindexOffset = section.find_first_of('_');
		if (decoZindexOffset != std::string::npos)
		{
			levelZindex = Parser::string2int(section.substr(decoZindexOffset + 1).c_str());
		}

		// V�rifie si la texture existe
		if (mLevel->mTextureMap->find(name) != mLevel->mTextureMap->end())
		{
			// R�cup�re la position et rotation
			size_t posRotOffset = value.find_first_of(')');
			if (posRotOffset != std::string::npos)
			{
				b2Vec3 posRot = Parser::string2b2Vec3(value.substr(0, posRotOffset + 1).c_str());

				// R�cup�re le z-index
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

	// Si on arrive l� : section inconnue
	Dialog::Error("ConfigReader::CreateLine()\n-> Section ["+ section +"] unknow!");
	return false;
}
