#pragma once
#include <string>
#include <cstdio>
#include <SFML/System.hpp>
#include <zlib-1.2.7/zlib.h>
#include <zip.h>

class ZipStream : public sf::InputStream
{
public:
	// Ctor & dtor
    ZipStream(std::string const& filename);
    ~ZipStream();

	// Fonctions impl�ment�es
    bool open(std::string const& filename);
    virtual sf::Int64 read(void* data, sf::Int64 size);

	// Fonction � finir
    virtual sf::Int64 getSize();

	// Fonctions non impl�ment�es
    virtual sf::Int64 seek(sf::Int64 position);
    virtual sf::Int64 tell();


private:
    struct zip *mZipArchive; // Repr�sente le fichier zip
	struct zip_file *mZipFile; // Repr�sente le fichier compress�
	struct zip_stat mZipFileStats;
};
