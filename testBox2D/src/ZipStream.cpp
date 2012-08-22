#include "ZipStream.h"
#include <iostream>

// Ctor & dtor
ZipStream::ZipStream(std::string const& filename)
	: mZipArchive(nullptr), mZipFile(nullptr)
{
	int erreur = 0;
	mZipArchive = zip_open(filename.c_str(), ZIP_CHECKCONS, &erreur);

	// Erreur ?
	if (!mZipArchive || erreur != 0)
	{
		char buffer[512];
		zip_error_to_str(buffer, sizeof(buffer), erreur, errno);

        fprintf(stderr, "«%s» %d in function «%s» ,err=%d\n«%s»\n", __FILE__, __LINE__, __FUNCTION__, erreur, buffer);
		system("PAUSE");

        exit(EXIT_FAILURE);
	}
}
ZipStream::~ZipStream()
{
	if (mZipFile)
		std::cout << zip_fclose(mZipFile) << std::endl;
	if (mZipArchive)
		std::cout << zip_close(mZipArchive);
}

// Fonctions implémentées
bool ZipStream::open(const std::string& filename)
{
	if (mZipArchive)
	{
		char buffer[512];
		mZipFile = zip_fopen(mZipArchive, filename.c_str(), ZIP_FL_UNCHANGED);
		if (!mZipFile)
		{
			strcpy_s(buffer, zip_strerror(mZipArchive));
			fprintf(stderr, "«%s» %d in function «%s»\n«%s»\n", __FILE__, __LINE__, __FUNCTION__, buffer);
			return false;
		}

		if(zip_stat(mZipArchive, filename.c_str(), 0, &mZipFileStats) == -1)
		{
			strcpy_s(buffer, zip_strerror(mZipArchive));
			fprintf(stderr, "«%s» %d in function «%s»\n«%s»\n", __FILE__, __LINE__, __FUNCTION__, buffer);
			zip_fclose(mZipFile);
			mZipFile = nullptr;
			return false;
		}
		return true;
	}
	return false;
}
sf::Int64 ZipStream::read(void* data, sf::Int64 size) // TODO: Finir
{
	if (mZipFile && mZipArchive)
	{
		sf::Int64 returnedInt = zip_fread(mZipFile, data, size);

		/*if (returnedInt != size)
		{
			fprintf(stderr, "\"%s\" %d in function \"%s\"\n\"read error ...\"\n", __FILE__, __LINE__, __FUNCTION__);
			//zip_fclose(mZipFile);
			return -1;
		}*/
		return returnedInt;
	}
	return -1;
}

// Fonction à finir
sf::Int64 ZipStream::getSize()
{
    if (mZipFile && mZipArchive)
    {
		return mZipFileStats.size;
    }
	return -1;
}

// Fonctions non implémentées
sf::Int64 ZipStream::seek(sf::Int64 position) // TODO
{
	if (mZipFile && mZipArchive)
		return -1;
	return -1;
}
sf::Int64 ZipStream::tell() // TODO
{
	if (mZipFile && mZipArchive)
		return -1;
	return -1;
}
