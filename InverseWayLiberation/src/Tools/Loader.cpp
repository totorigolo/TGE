#include "Loader.h"
#include "../Tools/Dialog.h"
#include "../Tools/Parser.h"

// Ctor
Loader::Loader(std::string const& path)
	: mPath(path), mIsValid(false)
{
	// Gestion de crise
	if (mFile.LoadFile(mPath.c_str()))
	{
		// Crée le message d'erreur
		std::string msg = "Impossible d'ouvrir le niveau \""+ mPath + "\".\n";
		msg += "Erreur #" + Parser::intToString(mFile.ErrorID()) + ".\n";

		// Ajoute l'explication 1 si elle existe
		if (mFile.GetErrorStr1())
		{
			msg += "Raison : \"";
			msg += mFile.GetErrorStr1();
			msg += "\".";
		}

		// Ajoute l'explication 2 si elle existe
		if (mFile.GetErrorStr2())
		{
			msg += "\nPlus précisément : \"";
			msg += mFile.GetErrorStr2();
			msg += "\".";
		}

		// Affiche l'erreur
		Dialog::Error(msg);

		// Le fichier n'est pas valide
		mIsValid = false;
	}

	// Le fichier est valide
	else
		mIsValid = true;
}

// Dtor
Loader::~Loader(void)
{
}

// Accesseurs
bool Loader::IsValid() const
{
	return mIsValid;
}
const std::string& Loader::GetPath() const
{
	return mPath;
}
