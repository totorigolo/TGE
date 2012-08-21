#ifndef DIALOG_H
#define DIALOG_H

#include <SFML/Graphics.hpp>
#include <string>
#include <vector>

namespace Dialog
{
	void Dialog(const std::string &image, const std::string &title, const std::string &message, bool quit = false, int output = 0);

	void Error(const std::string &message, bool quit = false, int output = 0);

	void Information(const std::string &message);
	
	const std::string TextInput(const std::string &titre, const std::string &message, const std::string& default = std::string());

	int NumberInput(const std::string &titre, const std::string &message, int default = -1);

	int ButtonChoice(const std::string &title, const std::string &message, const std::string &one, const std::string &two, int default = 0);
}

#endif
