#pragma once
#include <SFML/Graphics.hpp>
#include <Box2D/Box2D.h>
#include <string>

namespace Parser
{
	// Transforme un string en bool
	bool stringToBool(std::string const& value, bool _default = false);

	// Transforme un string en char
	char stringToChar(std::string const& value, char _default = -1);

	// Transforme un string en unsigned char
	unsigned char stringToUchar(std::string const& value, unsigned char _default = 255);

	// Transforme un string en int
	int stringToInt(std::string const& value, int _default = -1);

	// Transforme un string en unsigned int
	unsigned int stringToUint(std::string const& value, unsigned int _default = 0);

	// Transforme un string en float
	float stringToFloat(std::string const& value, float _default = -1.f);

	// Transforme un string en sf::Uint32
	sf::Uint32 stringToUint32(std::string const& value, sf::Uint32 _default = 0);

	// Transforme un string en sf::Color
	sf::Color stringToColor(std::string const& value, sf::Color const& _default = sf::Color::White);
	
	// Transforme un string en b2Vec2
	b2Vec2 stringToB2Vec2(std::string const& value, b2Vec2 const& default = b2Vec2(0.f, 0.f));
	
	// Transforme un string en b2Vec3
	b2Vec3 stringToB2Vec3(std::string const& value, b2Vec3 const& default = b2Vec3(0.f, 0.f, 0.f));
	
	////////////////////////////////////////////////////////////////////////////////////////////////////////

	// Transforme un int en string
	std::string intToString(int n);

	// Transforme un unsigned int en string
	std::string uintToString(unsigned int n);

	// Transforme un float en string
	// nbOfDecimals définit le nb de chiffres après la virgule (-1 = toutes)
	std::string floatToString(float n, int nbOfDecimals = -1);
	
	// Transforme un sf::Uint32 en string
	std::string Uint32ToString(sf::Uint32 value);

	// Transforme un sf::Color en string
	std::string colorToString(sf::Color const& value);
	
	// Transforme un sf::Vec2f en string
	std::string sfVec2fToString(sf::Vector2f const& value);
	
	// Transforme un b2Vec2 en string
	std::string b2Vec2ToString(b2Vec2 const& value);
	
	// Transforme un b2Vec3 en string
	std::string b2Vec3ToString(b2Vec3 const& value);
	
	// Transforme une touche en string
	// Uniquement les lettres, sinon un string vide
	std::string keyToString(sf::Keyboard::Key key, bool maj = false);

	// Transforme une touche en int
	// Uniquement les chiffres, sinon -1
	int keyToInt(sf::Keyboard::Key key);
}
