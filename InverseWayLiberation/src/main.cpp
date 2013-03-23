#include "App/App.h"
#include <lua.hpp>
#include <luabind/luabind.hpp>
#include <iostream>

int main()
{
	// Crée un Lua State
	lua_State *L = luaL_newstate();
	luaL_openlibs(L);

	// Connecte LuaBind à ce Lua State
	luabind::open(L);

	// Define a lua function that we can call
	luaL_dostring(
		L,
		"function hello()\n"
		"  print [[Hello from Lua !]]\n"
		"end\n"
		);
	
	luabind::call_function<void>(L, "hello");

	lua_close(L);

	/////////////////////////////////////////////////////////////////////////

    App app;
    return app.Execute();
}
