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
#include <luabind/object.hpp>
#include <luabind/operator.hpp>
#include <luabind/adopt_policy.hpp>
#include <luabind/dependency_policy.hpp>
#include <luabind/out_value_policy.hpp>
#include <luabind/return_reference_to_policy.hpp>
#include <luabind/copy_policy.hpp>
#include <luabind/discard_result_policy.hpp>
#include <luabind/iterator_policy.hpp>
#include <luabind/raw_policy.hpp>
#include <luabind/yield_policy.hpp>

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
#include "Tools/Dialog.h"
#include "Tools/Error.h"
#include "Tools/NonCopyable.h"
#include "Tools/Parser.h"
#include "Tools/Singleton.h"
#include "Tools/utils.h"
