#include "LevelLoader.h"
#include "Dialog.h"
#include "Parser.h"
#include "Physics/World.h"
#include "Physics/StaticBox.h"
#include "Physics/DynamicBox.h"
#include "Physics/DynamicCircle.h"
#include "Physics/DistanceJoint.h"
#include <Thor/Resources.hpp>

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
			mLevel->mTextureMap->operator[](name) = mLevel->mTextureCache->acquire(thor::Resources::fromFile<sf::Texture>(value));
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

		# ex : box = (5.2f, 9.f, 45.f) db 2.f
	*/
	else if (section == "bodies")
	{
		// Vérifie si la texture existe
		if (mLevel->mTextureMap->find(name) != mLevel->mTextureMap->end())
		{
			// Récupère la position et rotation
			size_t posRotOffset = value.find_first_of(')');
			b2Vec3 posRot = Parser::string2b2Vec3(value.substr(0, posRotOffset + 1).c_str());

			// Récupère le type
			size_t isdb = value.find("db", posRotOffset);
			size_t isdc = value.find("dc", posRotOffset);
			size_t issb = value.find("sb", posRotOffset);
			
			// dynamicBox
			if (isdb != std::string::npos)
			{
				mLevel->mWorld->RegisterBody(new DynamicBox(mLevel->mWorld, b2Vec2(posRot.x, posRot.y), mLevel->mTextureMap->operator[](name)));
				return true;
			}
			// dynamicCircle
			else if (isdc != std::string::npos)
			{
				mLevel->mWorld->RegisterBody(new DynamicCircle(mLevel->mWorld, b2Vec2(posRot.x, posRot.y), mLevel->mTextureMap->operator[](name)));
				return true;
			}
			// staticBox
			else if (issb != std::string::npos)
			{
				mLevel->mWorld->RegisterBody(new StaticBox(mLevel->mWorld, b2Vec2(posRot.x, posRot.y), mLevel->mTextureMap->operator[](name)));
				return true;
			}

			Dialog::Error("ConfigReader::CreateLine() - ["+ section +"]\n-> body type unknow!");
			return false;
		}
		
		Dialog::Error("ConfigReader::CreateLine() - ["+ section +"]\n-> texture \""+ name +"\" unknow!");
		return false;
	}

	// Si on arrive là : section inconnue
	Dialog::Error("ConfigReader::CreateLine()\n-> Section \""+ section +"\" unknow!");
	return false;
}
