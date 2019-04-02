#include "loadlua.h"
#include <stdio.h>
#include <stdlib.h>

lua_State* load_lua_file(const char* filename) {
    lua_State* L = luaL_newstate();
    luaL_openlibs(L);
    int error = luaL_dofile(L, "lua/test.lua");
    if (error) {
        fprintf(stderr, "Error loading Lua file: %s\n", lua_tostring(L, -1));
        exit(1);
    }
    return L;
}

const char* table_get_string(lua_State* L, const char* index) {
    const char* result = NULL;
    if (!lua_istable(L, -1)) {
        printf("Top of stack is not a table; aborting");
        fflush(stdout);
    } else {
        lua_getfield(L, -1, index);
        if (!lua_isstring(L, -1)) {
            printf("Value of table at %s is not a string; aborting", index);
            fflush(stdout);
        } else {
            result = lua_tostring(L, -1);
        }
        lua_pop(L, 1);
    }
    return result;
}

int table_get_int(lua_State* L, const char* index) {
    int result = 0;
    if (!lua_istable(L, -1)) {
        printf("Top of stack is not a table; aborting");
        fflush(stdout);
    } else {
        lua_getfield(L, -1, index);
        if (!lua_isinteger(L, -1)) {
            printf("Value of table at %s is not an integer; aborting", index);
            fflush(stdout);
        } else {
            result = lua_tointeger(L, -1);
        }
        lua_pop(L, 1);
    }
    return result;
}