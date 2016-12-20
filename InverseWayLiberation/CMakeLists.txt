cmake_minimum_required(VERSION 3.6)
project(inverse_way_liberation)

add_subdirectory(src)

set(IWL_src
        ${main_src}
        ${App_src}
        ${Editor_src}
        ${Entities_src}
        ${Game_src}
        ${Graphics_src}
        ${Level_src}
        ${Lua_src}
        ${Physics_src}
        ${Resources_src}
        ${Sounds_src}
        ${Tools_src}

        PARENT_SCOPE
)
