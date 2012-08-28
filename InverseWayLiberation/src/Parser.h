#pragma once
#include <string>
#include <SFML/Graphics.hpp>
#include <Box2D/Box2D.h>
#include "Dialog.h"

namespace Parser
{
	// Transforme un string en bool
	bool string2bool(std::string const& value, bool default = false);
	
	// Transforme un string en char
	char string2char(std::string const& value, char default = -1);
	
	// Transforme un string en unsigned char
	unsigned char string2uchar(std::string const& value, unsigned char default = 255);
	
	// Transforme un string en int
	int string2int(std::string const& value, int default = -1);

	// Transforme un string en unsigned int
	unsigned int string2uint(std::string const& value, unsigned int default = 0);

	// Transforme un string en float
	float string2float(std::string const& value, float default = -1.f);

	// Transforme un string en sf::Uint32
	sf::Uint32 string2Uint32(std::string const& value, sf::Uint32 default = 0);
	
	// Transforme un string en sf::Color
	sf::Color string2color(std::string const& value, sf::Color const& default = sf::Color::White);
	
	// Transforme un string en b2Vec2
	b2Vec2 string2b2Vec2(std::string const& value, b2Vec2 const& default = b2Vec2(0.f, 0.f));
	
	// Transforme un string en b2Vec3
	b2Vec3 string2b2Vec3(std::string const& value, b2Vec3 const& default = b2Vec3(0.f, 0.f, 0.f));
	
	// Transforme un string en le type donné comme valeur par défaut
	template<typename T>
	T string2template(std::string const& value, T const& default)
	{
		// On regarde si on connait le type et on appelle la fonction associée
		if (decltype(default) == bool)
			return string2bool(value, default);

		else if (decltype(default) == char)
			return string2char(value, default);

		else if (decltype(default) == int)
			return string2int(value, default);

		else if (decltype(default) == unsigned int)
			return string2uint(value, default);

		else if (decltype(default) == float)
			return string2float(value, default);

		else if (decltype(default) == sf::Uint32)
			return string2Uint32(value, default);

		else if (decltype(default) == sf::Color)
			return string2color(value, default);

		else if (decltype(default) == b2Vec2)
			return string2b2Vec2(value, default);

		else if (decltype(default) == b2Vec3)
			return string2b2Vec3(value, default);

		// Sinon, on averti qu'on connait pas
		Dialog::Error("Parser::string2template()\n-> template type unknow!");
		return default;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////
	
	// Transforme un int en string
	std::string int2string(int n);

	// Transforme un unsigned int en string
	std::string uint2string(unsigned int n);

	// Transforme un float en string
	// nbOfDecimals définit le nb de chiffres après la virgule (-1 = toutes)
	std::string float2string(float n, int nbOfDecimals = -1);

	// Transforme une touche en string
	// Uniquement les lettres, sinon un string vide
	std::string key2string(sf::Keyboard::Key key, bool maj = false);

	// Transforme une touche en int
	// Uniquement les chiffres, sinon -1
	int key2int(sf::Keyboard::Key key);
}
