#pragma once

#include <list>
#include "../Lua/LuaMachine.h"

class LuaArea;
class TriggersManager;
class LuaAction
{
public:
	// Ctors
	LuaAction(const std::string &_name, const std::string &_file, bool once = false);
	LuaAction(const std::string &_name, const std::string &_file, std::string &_function, bool once = false);

	// Exécute l'action
	void Execute(LuaMachine *luaMachine);

	// Gestion des Areas
	void RegisterArea(std::weak_ptr<LuaArea> area);

	// Accesseurs
	bool IsOnce();
	bool IsDone();
	bool HasFunction();
	const std::string& GetName();
	const std::string& GetFile();
	const std::string& GetFunction();

private:
	bool mDone;
	bool mOnce; // TODO: Les actions connaissent les areas : une fois faite, toutes les areas sont supprimées (faire une classe Area)
	std::string mName;
	std::string mFile;
	std::string mFunction;
	std::list <std::weak_ptr<LuaArea>> mAreas;
};
