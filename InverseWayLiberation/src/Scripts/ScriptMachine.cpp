#include "ScriptMachine.h"
#include "../Game/Game.h"
#include "../App/InputManager.h"
#include "../Level/LevelManager.h"
#include "../Physics/JointFactory.h"
#include "../Physics/PhysicManager.h"
#include "../Entities/EntityFactory.h"
#include "../Entities/Player.h"

// Ctor & dtor
ScriptMachine::ScriptMachine()
        : mScriptConsoleWindow(nullptr) {
    Init();
}

ScriptMachine::~ScriptMachine() {
}

// Réinitialisation
void ScriptMachine::Reset() {
    // TODO
}

// Enregistrement des attributs
void ScriptMachine::UnregisterGlobalVar(const std::string &name) {
    try {
        luabind::globals(mLuaState)[name] = luabind::nil;
    }
    catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
    }
}

// Exécution
int ScriptMachine::DoFile(const std::string &path, OutputInterface *_interface) {
    // Normalise l'interface
    auto output = GetInterface(_interface);

    try {
#ifdef _DEBUG
        sf::Clock c;
        int r = ReportError(luaL_dofile(mLuaState, path.c_str()), output);
        sf::String msg = "Script \"" + path + "\" exécuté en : "
                + Parser::realToString(c.getElapsedTime().asSeconds()) + " sec\n\n";
        *output << msg;
        return r;
#else
        return ReportError(mLuaState.runFile(path.c_str()), output);
#endif
    }
    catch (const std::exception &e) {
        *output << e.what() << "\n";
    }

    // Si on est là, c'est à cause d'une erreur
    return -1;
}

int ScriptMachine::DoString(const std::string &command, OutputInterface *_interface) {
    // Normalise l'interface
    auto output = GetInterface(_interface);

    try {
#ifdef _DEBUG
        sf::Clock c;
        int r = ReportError(luaL_dostring(mLuaState, command.c_str()), output);
        sf::String msg = "Commande \"" + command + "\" exécutée en : "
                + Parser::realToString(c.getElapsedTime().asSeconds()) + " sec\n";
        *output << msg;
        return r;
#else
        return ReportError(mLuaState.runString(command.c_str()), output);
#endif
    }
    catch (const std::exception &e) {
        *output << e.what() << "\n";
    }

    // Si on est là, c'est à cause d'une erreur
    return -1;
}

static ScriptConsoleWindow* luaConsolePrint_console{nullptr};

// Enregistre la console de l'IHM
void ScriptMachine::SetConsole(ScriptConsoleWindow *window) {
    myCheckError_v(window, "ScriptConsoleWindow passée invalide.");
    mScriptConsoleWindow = window;
    luaConsolePrint_console = mScriptConsoleWindow;
}

OutputInterface *ScriptMachine::GetInterface(OutputInterface *_interface) {
    auto tmp = _interface;

    // Crée une interface si on en a pas
    if (!tmp) {
        if (mScriptConsoleWindow)
            tmp = new ScriptConsoleInterface(*mScriptConsoleWindow);
        else
            tmp = new ostreamInterface();
    }
    return tmp;
}

void luaConsolePrint(const char* toPrint)
{
    if (!luaConsolePrint_console) return;
    *luaConsolePrint_console << toPrint;
}

void luaConsolePrintln(const char* toPrint)
{
    if (!luaConsolePrint_console) return;
    *luaConsolePrint_console << toPrint << "\n";
}

// Initialisation
void ScriptMachine::Init() {
    // Crée un Lua State
    mLuaState = luaL_newstate();
    luaL_openlibs(mLuaState);

    // Connecte LuaBind à ce Lua State
    luabind::open(mLuaState);

    // Enregistre les classes
    RegisterBox2D();
    RegisterSFML();

    // Enregistre la fonction consolePrint()
    luabind::open(mLuaState);
    luabind::module(mLuaState)
    [
            luabind::def("cprint", &luaConsolePrint),
            luabind::def("cprintln", &luaConsolePrintln)
    ];
}

// Affichage des erreurs
int ScriptMachine::ReportError(int errorCode, OutputInterface *_interface) {
    // Pas d'erreur
    if (errorCode == 0) {
        //*_interface << "Aucune erreur.\n";
        return errorCode;
    }

        // Erreurs connus
    else if (errorCode == LUA_ERRSYNTAX)
        *_interface << "Script: Erreur de syntaxe (LUA_ERRSYNTAX).\n";

    else if (errorCode == LUA_ERRRUN)
        *_interface << "Script: Erreur de Runtime (LUA_ERRRUN).\n";

    else if (errorCode == LUA_ERRMEM)
        *_interface << "Script: Pas assez de mémoire (LUA_ERRMEM).\n";

    else if (errorCode == LUA_ERRERR)
        *_interface << "Script: Erreur intraitable (LUA_ERRERR).\n";

    else if (errorCode == LUA_ERRFILE)
        *_interface << "Script: Fichier introuvable (LUA_ERRFILE).\n";

        // Erreur inconnu
    else
        *_interface << "Script: Erreur inconnue (" << errorCode << ").\n";

    // Affiche les détails de l'erreur
    *_interface << lua_tostring(mLuaState, -1) << "\n";
    lua_pop(mLuaState, 1);

//	// Affiche l'erreur sur la pile
//	lua_Debug debugInfo;
//	int r = lua_getstack(mLuaState, -1, &debugInfo);
//	if (r == 0)
//	{
//		*_interface << "Requested stack depth exceeded current stack depth\n";
//	}
//	lua_getinfo(mLuaState, "Slun", &debugInfo);
//	const char * er = lua_tostring(mLuaState, -1);
//	lua_pop(mLuaState, 1);
//
//	std::string error = "Failure to convert error to string";
//	if (er)
//	{
//		error = er;
//	}
//	*_interface <<
//		"Error " << debugInfo.event << " at (line " << debugInfo.currentline << "): " << error << "\n" <<
//		"\tIn a " << debugInfo.namewhat << " " << debugInfo.source << " function called " << debugInfo.name << ".\n" <<
//		"\tIn " << debugInfo.short_src << " defined at " << debugInfo.linedefined << ".\n";

    return errorCode;
}

lua_State *ScriptMachine::GetLuaState() {
    return mLuaState;
}
