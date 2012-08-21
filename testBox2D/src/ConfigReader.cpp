#include "ConfigReader.h"
#include "Dialog.h"
#include "Parser.h"

ConfigReader::ConfigReader(const std::string& path)
	: mIsOk(false), mPath(path)
{
	// Crée et ouvre le fichier
	mFile = new std::fstream(mPath, std::fstream::in);

	// Vérifie si le fichier existe
	if (mFile->fail())
	{
		Dialog::Error("Impossible d'ouvrir le fichier :\n\""+ mPath +"\"!");
		return;
	}

	// Analyse le fichier de config
	mIsOk = Process();
}

ConfigReader::~ConfigReader(void)
{
	mIsOk = false;
	delete mFile;
}

bool ConfigReader::Process()
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

const std::string ConfigReader::ParseLine(const std::string& line, const std::string& section, int lineNumber)
{
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

				// Supprime les espaces contenus dans le nom de la section
				for (unsigned int i = _section.size() -1; i >= 0 && (_section[i] == ' ' || _section[i] == '\t'); --i)
					_section.pop_back();

				// Enregistre la section seulement si on trouve le ']' avant la fin de la ligne
				if ((offset + index) < line.size() && index > 0)
					lineSection = _section;
				else
					Dialog::Error("ConfigReader::ParseLine("+ int2string(lineNumber) +")\n-> délimiteur ']' manquant.");
			}
			// Lit la paire nom=valeur dans la section courante
			else
			{
				std::string name;

				// Trouve le nom en allant jusqu'au '=' ou ':'
				for (; offset < line.size() && line[offset] != '=' && line[offset] != ':'; ++offset)
					if (line[offset] != ' ' && line[offset] != '\t')
						name += line[offset];

				// Supprime les espaces dans le nom
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

					// Trouve la valeur en allant jusqu'au délimiteurs de commentaires ';' ou '#'
					for (; offset < line.size() && line[offset] != '\r' && line[offset] != '\n' && line[offset] != ';' && line[offset] != '#'; ++offset)
						value += line[offset];

					// Supprime les espaces à la fin de la valeur
					for (unsigned int i = value.size() -1; i >= 0 && (value[i] == ' ' || value[i] == '\t'); --i)
						value.pop_back();
					
					// Enregistre la paire nom=valeur obtenue
					StoreNameValue(lineSection, name, value);
				}
				else
					Dialog::Error("ConfigReader::ParseLine("+ int2string(lineNumber) +")\n-> délimiteur '=' ou ':' manquant.");
			}
		}
	}
	return lineSection;
}

bool ConfigReader::StoreNameValue(const std::string& section, const std::string& name, const std::string& value)
{
	// Regarde si la section existe déjà
	std::map<std::string, std::map<std::string, std::string>*>::iterator iterSection = mSections.find(section);
	// Crée la section
	if (iterSection == mSections.end())
	{
		// Crée la section
		std::map<std::string, std::string>* map = new std::map<std::string, std::string>;
		if (map != nullptr)
		{
			// Insère la paire nom / valeur dans la section
			map->insert(std::pair<const std::string, const std::string>(name, value));

			// Enregistre la nouvelle section créée
			mSections.insert(std::pair<const std::string, std::map<std::string, std::string>*>(section, map));

			return true;
		}
		else
			Dialog::Error("ConfigReader::StoreNameValue("+ section +")\n-> unable to add ("+ name +","+ value +")\nOut of memory!");
	}
	else
	{
		// Récupère la section
		std::map<std::string, std::string>* map = iterSection->second;
		if (map != nullptr)
		{
			// Evite les doublons
			std::map<std::string, std::string>::iterator iterNameValue = map->find(name);
			// La nouvelle entrée n'est pas un doublon : on la crée
			if (iterNameValue == map->end())
				map->insert(std::pair<const std::string, const std::string>(name, value));
			// Sinon, on modifie l'ancienne
			else
				iterNameValue->second = value;

			return true;
		}
	}
	// Si on arrive là : erreur
	return false;
}

bool ConfigReader::SectionExists(const std::string& name) const
{
	// Vérifie si la section existe
	std::map<std::string, std::map<std::string, std::string>*>::const_iterator iter;
	iter = mSections.find(name);
	if (iter != mSections.end())
		return true;
	return false;
}

int ConfigReader::GetInt(const std::string& section, const std::string& name, int default) const
{
	// Vérifie si la section existe
	std::map<std::string, std::map<std::string, std::string>*>::const_iterator iter;
	iter = mSections.find(section);
	if (iter != mSections.end())
	{
		std::map<std::string, std::string>* map = iter->second;
		if (map != nullptr)
		{
			std::map<std::string, std::string>::iterator iterNameValue;
			iterNameValue = map->find(name);
			if (iterNameValue != map->end())
			{
				return string2int(iterNameValue->second, default);
			}
			Dialog::Error("La valeur \""+ name +"\" n'existe pas.");
		}
		Dialog::Error("La section \""+ section +"\" est corrompue.");
	}
	// La section n'existe pas
	Dialog::Error("La section \""+ section +"\" n'existe pas.");
	return default;
}

bool ConfigReader::GetBool(const std::string& section, const std::string& name, bool default) const
{
	// Vérifie si la section existe
	std::map<std::string, std::map<std::string, std::string>*>::const_iterator iter;
	iter = mSections.find(section);
	if (iter != mSections.end())
	{
		std::map<std::string, std::string>* map = iter->second;
		if (map != nullptr)
		{
			std::map<std::string, std::string>::iterator iterNameValue;
			iterNameValue = map->find(name);
			if (iterNameValue != map->end())
			{
				return string2bool(iterNameValue->second, default);
			}
			Dialog::Error("La valeur \""+ name +"\" n'existe pas.");
		}
		Dialog::Error("La section \""+ section +"\" est corrompue.");
	}
	// La section n'existe pas
	Dialog::Error("La section \""+ section +"\" n'existe pas.");
	return default;
}

float ConfigReader::GetFloat(const std::string& section, const std::string& name, float default) const
{
	// Vérifie si la section existe
	std::map<std::string, std::map<std::string, std::string>*>::const_iterator iter;
	iter = mSections.find(section);
	if (iter != mSections.end())
	{
		std::map<std::string, std::string>* map = iter->second;
		if (map != nullptr)
		{
			std::map<std::string, std::string>::iterator iterNameValue;
			iterNameValue = map->find(name);
			if (iterNameValue != map->end())
			{
				return string2float(iterNameValue->second, default);
			}
			Dialog::Error("La valeur \""+ name +"\" n'existe pas.");
		}
		Dialog::Error("La section \""+ section +"\" est corrompue.");
	}
	// La section n'existe pas
	Dialog::Error("La section \""+ section +"\" n'existe pas.");
	return default;
}

sf::Uint32 ConfigReader::GetUint32(const std::string& section, const std::string& name, const sf::Uint32 default) const
{
	// Vérifie si la section existe
	std::map<std::string, std::map<std::string, std::string>*>::const_iterator iter;
	iter = mSections.find(section);
	if (iter != mSections.end())
	{
		std::map<std::string, std::string>* map = iter->second;
		if (map != nullptr)
		{
			std::map<std::string, std::string>::iterator iterNameValue;
			iterNameValue = map->find(name);
			if (iterNameValue != map->end())
			{
				return string2Uint32(iterNameValue->second, default);
			}
			Dialog::Error("La valeur \""+ name +"\" n'existe pas.");
		}
		Dialog::Error("La section \""+ section +"\" est corrompue.");
	}
	// La section n'existe pas
	Dialog::Error("La section \""+ section +"\" n'existe pas.");
	return default;
}

const sf::Color ConfigReader::GetColor(const std::string& section, const std::string& name, const sf::Color& default) const
{
	// Vérifie si la section existe
	std::map<std::string, std::map<std::string, std::string>*>::const_iterator iter;
	iter = mSections.find(section);
	if (iter != mSections.end())
	{
		std::map<std::string, std::string>* map = iter->second;
		if (map != nullptr)
		{
			std::map<std::string, std::string>::iterator iterNameValue;
			iterNameValue = map->find(name);
			if (iterNameValue != map->end())
			{
				return string2color(iterNameValue->second, default);
			}
			Dialog::Error("La valeur \""+ name +"\" n'existe pas.");
		}
		Dialog::Error("La section \""+ section +"\" est corrompue.");
	}
	// La section n'existe pas
	Dialog::Error("La section \""+ section +"\" n'existe pas.");
	return default;
}

const std::string ConfigReader::GetString(const std::string& section, const std::string& name, const std::string& default) const
{
	// Vérifie si la section existe
	std::map<std::string, std::map<std::string, std::string>*>::const_iterator iter;
	iter = mSections.find(section);
	if (iter != mSections.end())
	{
		std::map<std::string, std::string>* map = iter->second;
		if (map != nullptr)
		{
			std::map<std::string, std::string>::iterator iterNameValue;
			iterNameValue = map->find(name);
			if (iterNameValue != map->end())
			{
				return iterNameValue->second;
			}
			Dialog::Error("La valeur \""+ name +"\" n'existe pas.");
		}
		Dialog::Error("La section \""+ section +"\" est corrompue.");
	}
	// La section n'existe pas
	Dialog::Error("La section \""+ section +"\" n'existe pas.");
	return default;
}