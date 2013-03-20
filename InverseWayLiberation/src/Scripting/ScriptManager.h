#pragma once
#include "../Tools/NonCopyable.h"
#include <angelscript.h>
#include <string>
#include <vector>
#include <mutex>
#include <map>

class ScriptManager : public NonCopyable
{
public:
	// Ctor & dtor
	ScriptManager();
	virtual ~ScriptManager();

	// Chargement des scripts
	bool LoadScriptFromFile(const std::string &script);
	//bool LoadScriptFromFile(const std::string &module, const std::string &script);

	// Exécute le script
	bool ExecuteScript(const std::string &script, const std::string &function);

protected:
	// Gestion des fonctions
	asIScriptFunction* GetFunction(const std::string &script, const std::string &function);
	int ExecuteContext(asIScriptContext *ctx);

	// Gestion du pool de contextes
	asIScriptContext* PrepareContextFromPool(asIScriptFunction *func);
	void ReturnContextToPool(asIScriptContext *ctx);

private:
	// Variable pour les erreurs
	int mR;
	
	// Moteur de script
	asIScriptEngine *mScriptEngine;

	// Mutex
	std::mutex mIOmutex; // Entrée / Sortie
	std::mutex mGlobalMutex;
	
	// Pool de Contexts
	std::vector<asIScriptContext *> mContexts;
};