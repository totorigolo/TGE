#include "App/App.h"
#include <lua.hpp>
#include <luabind/luabind.hpp>
#include <iostream>
#include <exception>

void print_hello(int number)
{
	std::cout << "hello " << number << " !" << std::endl;
}

void print_hello(const std::string &name)
{
	std::cout << "hello, " << name << " !" << std::endl;
}

int main()
{
	// Crée un Lua State
	lua_State *L = luaL_newstate();
	luaL_openlibs(L);

	try {
		// Connecte LuaBind à ce Lua State
		luabind::open(L);

		// Enregistre les fonctions
		luabind::module(L) [
			luabind::def("print_hello", (void(*)(int)) &print_hello),
			luabind::def("print_hello", (void(*)(const std::string&)) &print_hello)
		];

		// Exécute une fonction Lua
		luaL_dostring(
			L,
			"print_hello(42)\n"
			"print_hello('world')\n"
		);

		// Demande et exécute une fonction
		std::string command;
		std::getline(std::cin, command);
		luaL_dostring(
			L,
			command.c_str()
		);
	}
	catch (const std::exception &e)
	{
		std::cerr << e.what() << std::endl;
	}

	lua_close(L);

	/////////////////////////////////////////////////////////////////////////

    App app;
    return app.Execute();
}
