#include "ScriptManager.h"
#include "add_on/scriptstdstring/scriptstdstring.h"
#include "add_on/scriptbuilder/scriptbuilder.h"
#include <iostream>
#include <cassert>

// Fonction callback pour les messages d'AngelScript
void MessageCallback(const asSMessageInfo *msg, void *param)
{
	const char *type = "ERR ";
	if (msg->type == asMSGTYPE_WARNING)
		type = "WARN";
	else if (msg->type == asMSGTYPE_INFORMATION)
		type = "INFO";
	printf("%s (%d, %d) : %s : %s\n", msg->section, msg->row, msg->col, type, msg->message);
}

void print(const std::string &in)
{
	std::cout << in;
}
void println(const std::string &in)
{
	std::cout << in << std::endl;
}

// Ctor
ScriptManager::ScriptManager()
	: mR(0)
{
	// Crée le moteur de script
	mScriptEngine = asCreateScriptEngine(ANGELSCRIPT_VERSION);

	// Définie le callback pour les messages d'AS
	mR = mScriptEngine->SetMessageCallback(asFUNCTION(MessageCallback), 0, asCALL_CDECL); assert( mR >= 0 );

	// Enregistre le type std::string
	RegisterStdString(mScriptEngine);

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Enregistrement des fonctions C++

	// Enregistre les fonctions C++ que les scripts doivent connaitre
	mR = mScriptEngine->RegisterGlobalFunction("void print(const string &in)", asFUNCTION(print), asCALL_CDECL); assert( mR >= 0 );
	mR = mScriptEngine->RegisterGlobalFunction("void println(const string &in)", asFUNCTION(println), asCALL_CDECL); assert( mR >= 0 );
}

// Dtor
ScriptManager::~ScriptManager()
{
	// Suppression du moteur de script
	mScriptEngine->Release();
}

// Chargement des scripts
bool ScriptManager::LoadScriptFromFile(const std::string &script)
{
	// Pas de multithread ici
	std::lock_guard<std::mutex> lock(mGlobalMutex);

	// Crée un ScriptBuilder
	CScriptBuilder builder;
	mR = builder.StartNewModule(mScriptEngine, script.c_str());
	if (mR < 0)
	{
		// En général, ici, c'est qu'il n'y a plus de RAM
		std::cout << "Erreur fatale lors du lancement d'un module de script (r = " << mR << ")." << std::endl;
		return false;
	}

	// Charge le script
	std::string path = "scripts/" + script + ".as";
	mR = builder.AddSectionFromFile(path.c_str());
	if (mR < 0)
	{
		// Erreur de chargement ou de preprocessing
		std::cout << "Le script contient des erreurs ou n'existe pas (r = " << mR << ")." << std::endl;
		return false;
	}

	// Construit le module
	mR = builder.BuildModule();
	if (mR < 0)
	{
		// Il y a des erreurs dans le script
		std::cout << "Veuillez corriger votre script (r = " << mR << ")." << std::endl;
		return false;
	}

	// Tout s'est bien passé
	return true;
}

// Exécute le script
bool ScriptManager::ExecuteScript(const std::string &script, const std::string &function)
{
	// Récupère la fonction
	asIScriptFunction *func = GetFunction(script, function);
	if (!func)
		return false;

	// Crée et prépare le contexte
	asIScriptContext *ctx = PrepareContextFromPool(func);
	if (!ctx)
		return false;

	// Exécute le contexte
	ExecuteContext(ctx);
	
	// Recyclage du contexte
	ReturnContextToPool(ctx);

	// Tout s'est bien passé
	return true;
}

// Gestion des fonctions
asIScriptFunction* ScriptManager::GetFunction(const std::string &script, const std::string &function)
{
	// Cherche le module
	asIScriptModule *mod = mScriptEngine->GetModule(script.c_str());
	if (!mod)
	{
		// Le module n'a pas été trouvé
		mIOmutex.lock();
		std::cout << "Le module \"" << script << "\" n'a pas \x82""t\x82 trouv\x82""." << std::endl;
		mIOmutex.unlock();
		return nullptr;
	}

	// Cherche la fonction d'entrée du module
	asIScriptFunction *func = mod->GetFunctionByDecl(function.c_str());
	if (!func)
	{
		// La fonction n'a pas été trouvée
		mIOmutex.lock();
		std::cout << "Le module \"" << script << "\" doit contenir la fonction \"" << function << "\" (r = " << mR << ")." << std::endl;
		mIOmutex.unlock();
		return nullptr;
	}
	return func;
}
int ScriptManager::ExecuteContext(asIScriptContext *ctx)
{
	// Exécute le contexte
	mR = ctx->Execute();
	if (mR != asEXECUTION_FINISHED)
	{
		// Le script s'est mal terminé
		if (mR == asEXECUTION_EXCEPTION)
		{
			// On écrit l'erreur
			mIOmutex.lock();
			std::cout << "L'erreur " << ctx->GetExceptionString() << " s'est produite." << std::endl;
			mIOmutex.unlock();
		}
		// Le script a été arreté
		else if (mR == asEXECUTION_ABORTED)
		{
			// On écrit l'erreur
			mIOmutex.lock();
			std::cout << "Le script a \x82""t\x82"" arret\x82""." << std::endl;
			mIOmutex.unlock();
		}
		// Le script a été suspendu
		else if (mR == asEXECUTION_SUSPENDED)
		{
			// On écrit l'erreur
			mIOmutex.lock();
			std::cout << "Le script a \x82""t\x82"" suspendu." << std::endl;
			mIOmutex.unlock();
		}
		// Erreur inconnue
		else
		{
			mIOmutex.lock();
			std::cout << "Erreur inconnue (r = " << mR << ")." << std::endl;
			mIOmutex.unlock();
		}
	}
	return mR;
}

// Gestion du pool de contextes
asIScriptContext *ScriptManager::PrepareContextFromPool(asIScriptFunction *func)
{
	asIScriptContext *ctx = nullptr;
	if (mContexts.size() < 0)
	{
		ctx = mContexts.back();
		mContexts.pop_back();
	}
	else
		ctx = mScriptEngine->CreateContext();

	if (!ctx)
	{
		// Le contexte n'a pas pu être créé
		mIOmutex.lock();
		std::cout << "Le contexte n'a pas pu être créé." << std::endl;
		mIOmutex.unlock();
		return nullptr;
	}

	mR = ctx->Prepare(func);
	if (mR < 0)
	{
		// Le contexte n'a pas pu être préparé
		mIOmutex.lock();
		std::cout << "Le contexte n'a pas pu être préparé. (r = " << mR << ")." << std::endl;
		mIOmutex.unlock();
		return nullptr;
	}

	return ctx;
}

void ScriptManager::ReturnContextToPool(asIScriptContext *ctx)
{
	mContexts.push_back(ctx);

	// Nettoie le contexte
	ctx->Unprepare();
}
