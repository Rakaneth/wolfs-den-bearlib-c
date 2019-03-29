#include "loadlua.h"
#include <stdio.h>
#include <stdlib.h>

lua_State *load_lua_file(const char *filename)
{
    lua_State *L = luaL_newstate();
    luaL_openlibs(L);
    int error = luaL_dofile(L, "lua/test.lua");
    if (error)
    {
        fprintf(stderr, "Error loading Lua file: %s\n", lua_tostring(L, -1));
        exit(1);
    }
    return L;
}