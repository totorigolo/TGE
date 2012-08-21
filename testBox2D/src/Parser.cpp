#include "Parser.h"
#include <sstream>

bool string2bool(const std::string& value, bool default)
{
	if (value == "true" || value == "1" || value == "on")
		return true;
	if (value == "false" || value == "0" || value == "off")
		return false;
	return default;
}

int string2int(const std::string& value, int default)
{
	int result = default;
	std::istringstream iss(value);
	iss >> result;
	return result;
}

float string2float(const std::string& value, float default)
{
	float result = default;
	std::istringstream iss(value);
	iss >> result;
	return result;
}

sf::Uint32 string2Uint32(const std::string& value, const sf::Uint32 default)
{
	sf::Uint32 result = default;
	std::istringstream iss(value);
	iss >> result;
	return result;
}

const sf::Color string2color(const std::string& value, const sf::Color& default)
{
	sf::Color result = default;

	// Cherche le rouge
	size_t redOffset = value.find_first_of(',');
	if (redOffset != std::string::npos)
	{
		result.r = (sf::Uint8) atoi(value.substr(0, redOffset).c_str());

		// Cherche le vert
		size_t greenOffset = value.find_first_of(',', redOffset + 1);
		if (greenOffset != std::string::npos)
		{
			result.g = (sf::Uint8) atoi(value.substr(redOffset + 1, greenOffset).c_str());

			// Cherche le bleu et le canal alpha
			size_t blueOffset = value.find_first_of(',', greenOffset + 1);
			if (blueOffset != std::string::npos)
			{
				result.b = (sf::Uint8) atoi(value.substr(greenOffset + 1, blueOffset).c_str());
				result.a = (sf::Uint8) atoi(value.substr(blueOffset + 1).c_str());
			}
		}
	}
	return result;
}

const std::string int2string(int n)
{
	std::ostringstream oss;
	oss << n;
	std::string s = oss.str();
	return s;
}

const std::string float2string(float n, int nbOfDecimals)
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

const std::string key2string(sf::Keyboard::Key key, bool maj)
{
	std::string str;
	// Les lettres
	if (key >= sf::Keyboard::A && key <= sf::Keyboard::Z)
	{
		str += 'a' + key - sf::Keyboard::A;
		if (maj)
			str.back() -= 32; // majuscule
	}

	// Les chiffres
	else if (key >= sf::Keyboard::Num0 && key <= sf::Keyboard::Num9)
		str += '0' + key - sf::Keyboard::Num0;
	else if (key >= sf::Keyboard::Numpad0 && key <= sf::Keyboard::Numpad9)
		str += '0' + key - sf::Keyboard::Numpad0;

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