#include "stdafx.h"
#include "Saver.h"

// Ctor
Saver::Saver(std::string const& path, bool check)
	: mPath(path), mExists(false)
{
	// V�rifie si le fichier existe d�j�
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
												   "Une sauvegarde du m�me nom existe d�j�.\nEcraser la sauvegarde ?",
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
