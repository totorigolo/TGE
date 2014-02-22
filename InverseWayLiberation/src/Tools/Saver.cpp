#include "stdafx.h"
#include "Saver.h"

// Ctor
Saver::Saver(std::string const& path, bool overwrite)
	: mPath(path), mExists(false)
{
	// Vérifie si le fichier existe déjà
	std::ifstream file(path);
	mExists = file.good();
	file.close();

	// Si il existe
	if (mExists)
	{
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
