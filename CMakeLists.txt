cmake_minimum_required(VERSION 3.6)
project(inverse_way_liberation)
set(CMAKE_COLOR_MAKEFILE ON)
set(CMAKE_VERBOSE_MAKEFILE OFF)

# Demande C++14
set(CMAKE_CXX_STANDARD 14)

# g++ flags
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -pipe -Wpointer-arith -Winit-self -Wl,-O1 -Wl,--discard-all -Wl,--no-undefined")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Og -g")
#set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3 -Werror")

# Compilation Time Reducer
set (CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/CMake")
include(cotire)

# Dépendances
set(EXTRA_LIBS
        lua
        luabind
        tinyxml2
        sfml-system
        sfml-audio
        sfml-window
        sfml-graphics
        sfgui
        Box2D
)

# Fichiers d'en-tête
include_directories(
        "/usr/local/include/"
        "/usr/include"
#        "../luwra-master/lib"
)

# Bibliothèques
link_directories(
        "/usr/local/lib/"
)

# Inclu les sources
add_subdirectory(InverseWayLiberation)

add_definitions(-D_DEBUG)

# Crée l'exécutable
add_executable(IWL
        ${IWL_src}
)
target_link_libraries(IWL
        ${EXTRA_LIBS}
)
set_target_properties(IWL
        PROPERTIES COTIRE_CXX_PREFIX_HEADER_INIT "InverseWayLiberation/src/stdafx.h"
)
cotire(IWL)
install(TARGETS IWL
        RUNTIME DESTINATION bin
)
#install(FILES
#        ${rem_SOURCE_DIR}/../lib/poco/lib/Linux/i686/libPocoFoundation.so.5
#        ${rem_SOURCE_DIR}/../build/linux/install.sh
#        DESTINATION bin
#)
