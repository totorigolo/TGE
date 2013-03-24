#pragma once
#include <SFML/Graphics.hpp>
#include <string>

namespace Dialog
{
	void Dialog(const std::string &image, const std::string &title, const sf::String &message, bool quit = false, int output = 0);

	void Error(const sf::String &message, bool quit = false, int output = 0);

	void Information(const sf::String &message);

	const sf::String TextInput(const std::string &titre, const sf::String &message, const sf::String& _default = sf::String());

	int NumberInput(const std::string &titre, const sf::String &message, int _default = -1);

	int ButtonChoice(const std::string &title, const sf::String &message, const sf::String &one, const sf::String &two, int _default = 0);
}
