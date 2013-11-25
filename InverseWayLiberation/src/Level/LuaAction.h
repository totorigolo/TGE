#pragma once
#include "../Game/LuaMachine.h"
#include <iostream>
#include <string>

class TriggersManager;
class LuaAction
{
public:
	// Ctors
	LuaAction(const std::string &_name, const std::string &_file);
	LuaAction(const std::string &_name, const std::string &_file, std::string &_function);

	// Exécute l'action
	void Execute(LuaMachine *luaMachine);

	// Accesseurs
	bool HasFunction();
	const std::string& GetName();
	const std::string& GetFile();
	const std::string& GetFunction();

private:
	std::string mName;
	std::string mFile;
	std::string mFunction;
};
