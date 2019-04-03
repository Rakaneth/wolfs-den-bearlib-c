#include "loadlua.h"
#include <stdio.h>
#include <stdlib.h>

lua_State* load_lua_file(const char* filename) {
    lua_State* L = luaL_newstate();
    luaL_openlibs(L);
    int error = luaL_dofile(L, filename);
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
        lua_pushstring(L, index);
        lua_gettable(L, -2);
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
        lua_pushstring(L, index);
        lua_gettable(L, -2);
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

void dump_stack(lua_State* L) {
    int top = lua_gettop(L);
    int t = 0;

    for (int i = 1; i <= top; i++) {
        t = lua_type(L, i);
        switch (t) {
        case LUA_TSTRING:
            printf("%d: String '%s'", i, lua_tostring(L, i));
            break;
        case LUA_TBOOLEAN:
            printf("%d: Boolean %s", i, lua_toboolean(L, i) ? "true" : "false");
            break;
        case LUA_TNUMBER:
            printf("%d: Number %g", i, lua_tonumber(L, i));
            break;
        default:
            printf("%d: Other type: %s", i, lua_typename(L, t));
            break;
        }
        printf(" ");
    }
    printf("\n");
}