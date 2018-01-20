#pragma once

#include "../Tools/Parser.h"
#include "../Editor/GUI/ScriptConsoleWindow.h"

// Classe abstraite de base
class OutputInterface : public NonCopyable {
public:
    // Affichage du texte
    virtual OutputInterface &operator<<(const std::string &text) = 0;

    // Prise en charge de tous les types
    OutputInterface &operator<<(const char *text) {
		return *this << std::string(text);
    }

    OutputInterface &operator<<(bool val) {
        return *this << std::string(val ? "true" : "false");
    }

    OutputInterface &operator<<(int val) {
        return *this << Parser::intToString(val);
    }

    OutputInterface &operator<<(unsigned int val) {
        return *this << Parser::uintToString(val);
    }

    OutputInterface &operator<<(float val) {
        return *this << Parser::floatToString(val);
    }

    OutputInterface &operator<<(double val) {
        return *this << Parser::floatToString(static_cast<float>(val));
    }
};

// Sortie console
class ostreamInterface : public OutputInterface {
public:
    // Affichage du texte
    virtual OutputInterface &operator<<(const std::string &text) {
        std::cout << text;
        return *this;
    }
};

// Sortie par la Console Script
class ScriptConsoleWindow;

class ScriptConsoleInterface : public OutputInterface {
public:
    // Ctor
    ScriptConsoleInterface(ScriptConsoleWindow &luaConsoleWindow)
            : mluaConsoleWindow(luaConsoleWindow) {
    }

public:
    // Affichage du texte
    virtual OutputInterface &operator<<(const std::string &text) {
        std::cout << text;
        mluaConsoleWindow << text;
        return *this;
    }

private:
    ScriptConsoleWindow &mluaConsoleWindow;
};
