#pragma once
#include <string>
#include <SFML/Graphics.hpp>
#include <Box2D/Box2D.h>

namespace Parser
{
	// Transforme un string en bool
	bool string2bool(const std::string& value, bool default = false);

	// Transforme un string en int
	int string2int(const std::string& value, int default = -1);

	// Transforme un string en float
	float string2float(const std::string& value, float default = -1.f);

	// Transforme un string en sf::Uint32
	sf::Uint32 string2Uint32(const std::string& value, const sf::Uint32 default = 0);
	
	// Transforme un string en sf::Color
	sf::Color string2color(const std::string& value, const sf::Color& default = sf::Color::White);
	
	// Transforme un string en b2Vec2
	b2Vec2 string2b2Vec2(const std::string& value, const b2Vec2& default = b2Vec2(0.f, 0.f));
	
	// Transforme un string en b2Vec3
	b2Vec3 string2b2Vec3(const std::string& value, const b2Vec3& default = b2Vec3(0.f, 0.f, 0.f));
	
	////////////////////////////////////////////////////////////////////////////////////////////////////////

	// Transforme un int en string
	std::string int2string(int n);

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
