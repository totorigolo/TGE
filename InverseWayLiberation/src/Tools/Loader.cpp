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
		Dialog::Error("Impossible d'ouvrir le niveau :\n\""+ mPath + "\"\n"
			+ "Erreur #" + Parser::int2string(mFile.ErrorID()) + "\n"
			+ "\"" + mFile.GetErrorStr1() + "\". Plus précisément :\n"
			+ mFile.GetErrorStr2(), true);
	}

	mIsValid = true;
}

// Dtor
Loader::~Loader(void)
{
}
