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

	// Connecte LuaBind à ce Lua State
	luabind::open(L);

	luabind::module(L) [
		luabind::def("print_hello", (void(*)(int)) &print_hello),
		luabind::def("print_hello", (void(*)(const std::string&)) &print_hello)
	];

	// Define a lua function that we can call
	luaL_dostring(
		L,
		"print_hello(42)\n"
		"print_hello('world')\n"
	);

	lua_close(L);

	/////////////////////////////////////////////////////////////////////////

    App app;
    return app.Execute();
}
