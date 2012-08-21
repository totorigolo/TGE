#pragma once
#include <string>
#include <SFML/Graphics.hpp>

// Transforme un string en bool
bool string2bool(const std::string& value, bool default);

// Transforme un string en int
int string2int(const std::string& value, int default);

// Transforme un string en float
float string2float(const std::string& value, float default);

// Transforme un string en sf::Uint32
sf::Uint32 string2Uint32(const std::string& value, const sf::Uint32 default);

// Transforme un string en sf::Color
const sf::Color string2color(const std::string& value, const sf::Color& default);
	
// Transforme un int en string
const std::string int2string(int n);

// Transforme un float en string
// nbOfDecimals définit le nb de chiffres après la virgule (-1 = toutes)
const std::string float2string(float n, int nbOfDecimals = -1);

// Transforme une touche en string
// Uniquement les lettres, sinon un string vide
const std::string key2string(sf::Keyboard::Key key, bool maj = false);

// Transforme une touche en int
// Uniquement les chiffres, sinon -1
int key2int(sf::Keyboard::Key key);