#pragma once
#include "../Editor/GUI/LuaConsoleWindow.h"

// Classe abstraite de base
class OutputInterface : public NonCopyable
{
public:
	// Affichage du texte
	virtual OutputInterface& operator<<(const std::string &text) = 0;

	// Prise en charge du std::endl
	OutputInterface& operator<<(std::ostream& (*f)(std::ostream&))
	{
		f;
		*this << std::string("\n");
		return *this;
	}

	// Prise en charge de tous les types
	OutputInterface& operator<<(const char *text)
	{
		*this << std::string(text);
		return *this;
	}
	OutputInterface& operator<<(bool val)
	{
		if (val)
			*this << std::string("true");
		*this << std::string("false");
		return *this;
	}
	OutputInterface& operator<<(int val)
	{
		*this << Parser::intToString(val);
		return *this;
	}
	OutputInterface& operator<<(unsigned int val)
	{
		*this << Parser::uintToString(val);
		return *this;
	}
	OutputInterface& operator<<(float val)
	{
		*this << Parser::floatToString(val);
		return *this;
	}
	OutputInterface& operator<<(double val)
	{
		*this << Parser::floatToString(static_cast<float>(val));
		return *this;
	}
};

// Sortie console
class ostreamInterface : public OutputInterface
{
public:
	// Affichage du texte
	virtual OutputInterface& operator<<(const std::string &text)
	{
		std::cout << text;
		return *this;
	}
};

// Sortie par la Console Lua
class LuaConsoleWindow;
class LuaConsoleInterface : public OutputInterface
{
public:
	// Ctor
	LuaConsoleInterface(LuaConsoleWindow &luaConsoleWindow)
		: mluaConsoleWindow(luaConsoleWindow)
	{
	}

public:
	// Affichage du texte
	virtual OutputInterface& operator<<(const std::string &text)
	{
		mluaConsoleWindow << text;
		return *this;
	}

private:
	LuaConsoleWindow &mluaConsoleWindow;
};
