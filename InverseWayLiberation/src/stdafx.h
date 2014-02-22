// Fichier d'en-têtes précompilées
//  - A inclure dans tous les .cpp
#pragma once

// Box2D
#include <Box2D/Box2D.h>

// SFML
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>

// SFGUI
#include <SFGUI/SFGUI.hpp>

// Lua
#include <lua.hpp>
// Désactive les warning "warning C4251: ...nécessite une interface DLL pour être utilisé(e) par les clients..."
#pragma warning (disable : 4251)
#include <luabind/luabind.hpp>
#include <luabind/operator.hpp>

// STL
#include <set>
#include <map>
#include <list>
#include <string>
#include <vector>
#include <memory>
#include <fstream>
#include <sstream>
#include <utility>
#include <iostream>
#include <exception>
#include <unordered_map>

// Project
#include "Tools\Dialog.h"
#include "Tools\Error.h"
#include "Tools\NonCopyable.h"
#include "Tools\Parser.h"
#include "Tools\Singleton.h"
#include "Tools\utils.h"
