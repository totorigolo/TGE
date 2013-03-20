#pragma once
#include "../Tools/NonCopyable.h"
#include <angelscript.h>
#include <string>
#include <map>
#include <mutex>

class ScriptManager : public NonCopyable
{
public:
	// Ctor & dtor
	ScriptManager();
	virtual ~ScriptManager();

	// Chargement des scripts
	bool LoadScriptFromFile(const std::string &module, const std::string &script);

	// Exécute le script
	bool ExecuteScript(const std::string &module, const std::string &fonction);

private:
	// Variable pour les erreurs
	int mR;
	
	// Moteur de script
	asIScriptEngine *mScriptEngine;

	// Mutex
	std::mutex mIOmutex; // Entrée / Sortie
	std::mutex mGlobalMutex;
};