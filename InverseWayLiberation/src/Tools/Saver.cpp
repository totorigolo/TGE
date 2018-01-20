#include <fstream>
#include "Saver.h"
#include "Dialog.h"

// Ctor
Saver::Saver(std::string const& path, bool check)
	: mExists(false), mPath(path)
{
	// Vérifie si le fichier existe déjà
	std::ifstream file(path);
	mExists = file.good();
	file.close();

	// Si il existe
	if (mExists)
	{
		// Si check == true, on demande quoi faire
		bool overwrite = true;
		if (check)
		{
			overwrite = (1 == Dialog::ButtonChoice("Ecraser la sauvegarde ?",
												   L"Une sauvegarde du même nom existe déjà.\nEcraser la sauvegarde ?",
												   "Oui", "Non"));
		}

		// On efface le fichier si overwrite == true
		if (overwrite)
		{
			file.open(path, std::ios_base::trunc);
			file.close();
			mIsValid = true;
		}

		// Sinon le fichier n'est pas valide
		else
		{
			mIsValid = false;
		}
	}
	else
		mIsValid = true;
}

// Dtor
Saver::~Saver(void)
{
}

// Accesseurs
bool Saver::Exists() const
{
	return mExists;
}
bool Saver::IsValid() const
{
	return mIsValid;
}
const std::string& Saver::GetPath() const
{
	return mPath;
}
