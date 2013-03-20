#include "ScriptManager.h"
#include "add_on/scriptstdstring/scriptstdstring.h"
#include "add_on/scriptbuilder/scriptbuilder.h"
#include <iostream>
#include <cassert>
#include <ctime>

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

// Fonction callback qui gère le timeout
void LineCallback(asIScriptContext *ctx, time_t *timeOut)
{
	// Arrête le script si le timeout est dépassé
	if(*timeOut < time(nullptr))
		ctx->Abort();

	// Possible de faire Suspend(), pour le reprendre avec Execute()
	//ctx->Suspend();
	//ctx->Execute();
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
bool ScriptManager::LoadScriptFromFile(const std::string &module, const std::string &script)
{
	// Pas de multithread ici
	std::lock_guard<std::mutex> lock(mGlobalMutex);

	// Crée un ScriptBuilder
	CScriptBuilder builder;
	mR = builder.StartNewModule(mScriptEngine, module.c_str()); 
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
bool ScriptManager::ExecuteScript(const std::string &module, const std::string &fonction)
{
	// Cherche le module
	asIScriptModule *mod = mScriptEngine->GetModule(module.c_str());
	if (!mod)
	{
		// Le module n'a pas été trouvé
		mIOmutex.lock();
		std::cout << "Le module \"" << module << "\" n'a pas \x82""t\x82 trouv\x82""." << std::endl;
		mIOmutex.unlock();
		return false;
	}

	// Cherche la fonction d'entrée du module
	asIScriptFunction *func = mod->GetFunctionByDecl(fonction.c_str());
	if (!func)
	{
		// La fonction n'a pas été trouvée
		mIOmutex.lock();
		std::cout << "Le module \"" << module << "\" doit contenir la fonction \"" << fonction << "\" (r = " << mR << ")." << std::endl;
		mIOmutex.unlock();
		return false;
	}

	// Crée et prépare le contexte
	asIScriptContext *ctx = mScriptEngine->CreateContext();

	// Défini un timeout
	time_t timeOut;
	mR = ctx->SetLineCallback(asFUNCTION(LineCallback), &timeOut, asCALL_CDECL);
	
	// Définie la fonction d'entrée du contexte d'exécution
	ctx->Prepare(func);

	//!++ Ahhhhh !
	std::string str("world");
	ctx->SetArgObject(0, &str);
	
	// On donne une seconde max au script
	timeOut = time(nullptr) + 1;

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
	
	// Suppression du contexte
	ctx->Release();

	// Tout s'est bien passé
	return true;
}
