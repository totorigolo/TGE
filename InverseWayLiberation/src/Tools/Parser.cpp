#include "stdafx.h"

namespace Parser
{
bool stringToBool(std::string const& value, bool _default)
{
	if (value.find("true") != std::string::npos || value.find("1") != std::string::npos || value.find("on") != std::string::npos)
		return true;
	if (value.find("false") != std::string::npos || value.find("0") != std::string::npos || value.find("off") != std::string::npos)
		return false;
	return _default;
}

char stringToChar(std::string const& value, char _default)
{
	char result = _default;
	std::istringstream iss(value);
	iss >> result;
	return result;
}

unsigned char stringToUchar(std::string const& value, unsigned char _default)
{
	unsigned int result = _default;
	std::istringstream iss(value);
	iss >> result;
	return static_cast<unsigned char>(result);
}

int stringToInt(std::string const& value, int _default)
{
	int result = _default;
	std::istringstream iss(value);
	iss >> result;
	return result;
}

unsigned int stringToUint(std::string const& value, unsigned int _default)
{
	unsigned int result = _default;
	std::istringstream iss(value);
	iss >> result;
	return result;
}

float stringToFloat(std::string const& value, float _default)
{
	float result = _default;
	std::istringstream iss(value);
	iss >> result;
	return result;
}

sf::Uint32 stringToUint32(std::string const& value, sf::Uint32 _default)
{
	sf::Uint32 result = _default;
	std::istringstream iss(value);
	iss >> result;
	return result;
}

sf::Color stringToColor(std::string const& value, sf::Color const& _default)
{
	sf::Color result = _default;

	// Cherche le rouge
	size_t p1Offset = value.find_first_of('(');
	size_t redOffset = value.find_first_of(',');
	if (redOffset != std::string::npos)
	{
		result.r = (sf::Uint8) stringToUchar(value.substr(p1Offset + 1, redOffset - p1Offset - 1).c_str());

		// Cherche le vert
		size_t greenOffset = value.find_first_of(',', redOffset + 1);
		if (greenOffset != std::string::npos)
		{
			result.g = (sf::Uint8) stringToUchar(value.substr(redOffset + 1, greenOffset - redOffset - 1).c_str());

			// Cherche le bleu
			size_t blueOffset = value.find_first_of(",)", greenOffset + 1);
			if (blueOffset != std::string::npos)
			{
				result.b = (sf::Uint8) stringToUchar(value.substr(greenOffset + 1, blueOffset - greenOffset - 1).c_str());

				// Cherche le canal alpha
				size_t alphaOffset = value.find_first_of(',', blueOffset + 1);
				size_t p2Offset = value.find_first_of(')', blueOffset + 1);
				if (alphaOffset != std::string::npos)
				{
					result.a = (sf::Uint8) stringToUchar(value.substr(alphaOffset + 1, p2Offset - alphaOffset - 1).c_str());
				}
			}
		}
	}
	return result;
}

b2Vec2 stringToB2Vec2(std::string const& value, b2Vec2 const& default)
{
	b2Vec2 result = default;

	// Cherche le x
	size_t p1Offset = value.find_first_of('(');
	size_t xOffset = value.find_first_of(',');
	size_t yOffset = value.find_first_of(')', xOffset + 1);
	if (xOffset != std::string::npos)
	{
		result.x = stringToFloat(value.substr(p1Offset + 1, xOffset - p1Offset - 1).c_str());

		// Cherche le y
		if (yOffset != std::string::npos)
		{
			result.y = stringToFloat(value.substr(xOffset + 1, yOffset - xOffset - 1).c_str());
		}
	}
	return result;
}
	
b2Vec3 stringToB2Vec3(std::string const& value, b2Vec3 const& default)
{
	b2Vec3 result = default;

	// Cherche le x
	size_t p1Offset = value.find_first_of('(');
	size_t xOffset = value.find_first_of(',', p1Offset + 1);
	if (p1Offset != std::string::npos && xOffset != std::string::npos)
	{
		result.x = stringToFloat(value.substr(p1Offset + 1, xOffset - p1Offset - 1).c_str());

		// Cherche le y
		size_t yOffset = value.find_first_of(',', xOffset + 1);
		if (yOffset == std::string::npos)
		{
			yOffset = value.find_first_of(')', xOffset + 1);
		}
		if (yOffset != std::string::npos)
		{
			result.y = stringToFloat(value.substr(xOffset + 1, yOffset - xOffset - 1).c_str());

			// Cherche le z
			size_t zOffset = value.find_first_of(')', yOffset + 1);
			if (zOffset != std::string::npos)
			{
				result.z = stringToFloat(value.substr(yOffset + 1, zOffset - yOffset - 1).c_str());
			}
		}
	}
	return result;
}

 ////////////////////////////////////////////////////////////////////////////////////////////////////////

std::string intToString(int n)
{
	std::ostringstream oss;
	oss << n;
	std::string s = oss.str();
	return s;
}

std::string uintToString(unsigned int n)
{
	std::ostringstream oss;
	oss << n;
	std::string s = oss.str();
	return s;
}

std::string boolToString(bool b)
{
	if (b)
		return std::string("true");
	else
		return std::string("false");
}

std::string floatToString(float n, int nbOfDecimals)
{
	std::ostringstream oss;
	oss.precision(nbOfDecimals);
	oss << n;
	std::string s = oss.str();
	return s;
}

std::string Uint32ToString(sf::Uint32 value)
{
	return uintToString(value);
}

std::string colorToString(sf::Color const& value)
{
	std::string s("(");
	s.append(uintToString(value.r));
	s.append(", ");
	s.append(uintToString(value.g));
	s.append(", ");
	s.append(uintToString(value.b));
	s.append(")");
	return s;
}

std::string sfVec2fToString(sf::Vector2f const& value)
{
	std::string s("(");
	s.append(floatToString(value.x));
	s.append(", ");
	s.append(floatToString(value.y));
	s.append(")");
	return s;
}

std::string b2Vec2ToString(b2Vec2 const& value)
{
	std::string s("(");
	s.append(floatToString(value.x));
	s.append(", ");
	s.append(floatToString(value.y));
	s.append(")");
	return s;
}

std::string b2Vec3ToString(b2Vec3 const& value)
{
	std::string s("(");
	s.append(floatToString(value.x));
	s.append(", ");
	s.append(floatToString(value.y));
	s.append(", ");
	s.append(floatToString(value.z));
	s.append(")");
	return s;
}

std::string keyToString(sf::Keyboard::Key key, bool maj)
{
	std::string str;
	// Les lettres
	if (key >= sf::Keyboard::A && key <= sf::Keyboard::Z)
	{
		str += 'a' + static_cast<char>(key - sf::Keyboard::A);
		if (maj)
			*str.begin() -= 32; // majuscule
	}

	// Les chiffres
	else if (key >= sf::Keyboard::Num0 && key <= sf::Keyboard::Num9)
		str += '0' + static_cast<char>(key - sf::Keyboard::Num0);
	else if (key >= sf::Keyboard::Numpad0 && key <= sf::Keyboard::Numpad9)
		str += '0' + static_cast<char>(key - sf::Keyboard::Numpad0);

	// Les autres caractères
	// Le point
	else if (key == sf::Keyboard::Period)
		str += '.';
	// Les signes opératoires
	else if (key == sf::Keyboard::Add)
		str += '+';
	else if (key == sf::Keyboard::Subtract || key == sf::Keyboard::Dash)
		str += '-';
	else if (key == sf::Keyboard::Multiply)
		str += '*';
	else if (key == sf::Keyboard::Divide)
		str += '/';
	// BackSlash
	else if (key == sf::Keyboard::BackSlash)
		str += '\\';
	// TODO : Trouver les bonnes combinaisons
	// Les parenthèses ( et )
	else if (key == sf::Keyboard::LBracket)
		str += '(';
	else if (key == sf::Keyboard::RBracket)
		str += ')';
	// Les caractères [ et ]
	else if (key == sf::Keyboard::LBracket && (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift) || sf::Keyboard::isKeyPressed(sf::Keyboard::RShift)))
		str += '[';
	else if (key == sf::Keyboard::RBracket && (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift) || sf::Keyboard::isKeyPressed(sf::Keyboard::RShift)))
		str += ']';
	// Les caractères { et }
	else if (key == sf::Keyboard::Quote && (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift) || sf::Keyboard::isKeyPressed(sf::Keyboard::RShift)))
		str += '{';
	else if (key == sf::Keyboard::Equal && (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift) || sf::Keyboard::isKeyPressed(sf::Keyboard::RShift)))
		str += '}';

	return str;
}

int keyToInt(sf::Keyboard::Key key)
{
	if (key >= sf::Keyboard::Num0 && key <= sf::Keyboard::Num9)
		return key - sf::Keyboard::Num0;
	else if (key >= sf::Keyboard::Numpad0 && key <= sf::Keyboard::Numpad9)
		return key - sf::Keyboard::Numpad0;
	else
		return -1;
}
} // namespace Parser
