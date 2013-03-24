#include "Parser.h"
#include "Dialog.h"
#include <sstream>

namespace Parser
{
bool string2bool(std::string const& value, bool _default)
{
	if (value.find("true") != std::string::npos || value.find("1") != std::string::npos || value.find("on") != std::string::npos)
		return true;
	if (value.find("false") != std::string::npos || value.find("0") != std::string::npos || value.find("off") != std::string::npos)
		return false;
	return _default;
}

char string2char(std::string const& value, char _default)
{
	char result = _default;
	std::istringstream iss(value);
	iss >> result;
	return result;
}

unsigned char string2uchar(std::string const& value, unsigned char _default)
{
	unsigned int result = _default;
	std::istringstream iss(value);
	iss >> result;
	return static_cast<unsigned char>(result);
}

int string2int(std::string const& value, int _default)
{
	int result = _default;
	std::istringstream iss(value);
	iss >> result;
	return result;
}

unsigned int string2uint(std::string const& value, unsigned int _default)
{
	unsigned int result = _default;
	std::istringstream iss(value);
	iss >> result;
	return result;
}

float string2float(std::string const& value, float _default)
{
	float result = _default;
	std::istringstream iss(value);
	iss >> result;
	return result;
}

sf::Uint32 string2Uint32(std::string const& value, sf::Uint32 _default)
{
	sf::Uint32 result = _default;
	std::istringstream iss(value);
	iss >> result;
	return result;
}

sf::Color string2color(std::string const& value, sf::Color const& _default)
{
	sf::Color result = _default;

	// Cherche le rouge
	size_t p1Offset = value.find_first_of('(');
	size_t redOffset = value.find_first_of(',');
	if (redOffset != std::string::npos)
	{
		result.r = (sf::Uint8) string2uchar(value.substr(p1Offset + 1, redOffset - p1Offset - 1).c_str());

		// Cherche le vert
		size_t greenOffset = value.find_first_of(',', redOffset + 1);
		if (greenOffset != std::string::npos)
		{
			result.g = (sf::Uint8) string2uchar(value.substr(redOffset + 1, greenOffset - redOffset - 1).c_str());

			// Cherche le bleu
			size_t blueOffset = value.find_first_of(',', greenOffset + 1);
			if (blueOffset != std::string::npos)
			{
				result.b = (sf::Uint8) string2uchar(value.substr(greenOffset + 1, blueOffset - greenOffset - 1).c_str());

				// Cherche le canal alpha
				size_t alphaOffset = value.find_first_of(',', blueOffset + 1);
				size_t p2Offset = value.find_first_of(')', blueOffset + 1);
				if (alphaOffset != std::string::npos)
				{
					result.a = (sf::Uint8) string2uchar(value.substr(alphaOffset + 1, p2Offset - alphaOffset - 1).c_str());
				}
			}
		}
	}
	return result;
}

b2Vec2 string2b2Vec2(std::string const& value, b2Vec2 const& default)
{
	b2Vec2 result = default;

	// Cherche le x
	size_t p1Offset = value.find_first_of('(');
	size_t xOffset = value.find_first_of(',');
	size_t yOffset = value.find_first_of(')', xOffset + 1);
	if (xOffset != std::string::npos)
	{
		result.x = string2float(value.substr(p1Offset + 1, xOffset - p1Offset - 1).c_str());

		// Cherche le y
		if (yOffset != std::string::npos)
		{
			result.y = string2float(value.substr(xOffset + 1, yOffset - xOffset - 1).c_str());
		}
	}
	return result;
}
	
b2Vec3 string2b2Vec3(std::string const& value, b2Vec3 const& default)
{
	b2Vec3 result = default;

	// Cherche le x
	size_t p1Offset = value.find_first_of('(');
	size_t xOffset = value.find_first_of(',', p1Offset + 1);
	if (p1Offset != std::string::npos && xOffset != std::string::npos)
	{
		result.x = string2float(value.substr(p1Offset + 1, xOffset - p1Offset - 1).c_str());

		// Cherche le y
		size_t yOffset = value.find_first_of(',', xOffset + 1);
		if (yOffset == std::string::npos)
		{
			yOffset = value.find_first_of(')', xOffset + 1);
		}
		if (yOffset != std::string::npos)
		{
			result.y = string2float(value.substr(xOffset + 1, yOffset - xOffset - 1).c_str());

			// Cherche le z
			size_t zOffset = value.find_first_of(')', yOffset + 1);
			if (zOffset != std::string::npos)
			{
				result.z = string2float(value.substr(yOffset + 1, zOffset - yOffset - 1).c_str());
			}
		}
	}
	return result;
}

 ////////////////////////////////////////////////////////////////////////////////////////////////////////

std::string int2string(int n)
{
	std::ostringstream oss;
	oss << n;
	std::string s = oss.str();
	return s;
}

std::string uint2string(unsigned int n)
{
	std::ostringstream oss;
	oss << n;
	std::string s = oss.str();
	return s;
}

 std::string float2string(float n, int nbOfDecimals)
{
	if (nbOfDecimals != -1)
	{
		float m = 1.f;
		for (int i = 0; i < nbOfDecimals; ++i)
			m *= 10.f;
		n = static_cast<float>(static_cast<int>(n * m)) / m;
	}

	std::ostringstream oss;
	oss << n;
	std::string s = oss.str();
	return s;
}

 std::string key2string(sf::Keyboard::Key key, bool maj)
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

int key2int(sf::Keyboard::Key key)
{
	if (key >= sf::Keyboard::Num0 && key <= sf::Keyboard::Num9)
		return key - sf::Keyboard::Num0;
	else if (key >= sf::Keyboard::Numpad0 && key <= sf::Keyboard::Numpad9)
		return key - sf::Keyboard::Numpad0;
	else
		return -1;
}
} // namespace Parser
