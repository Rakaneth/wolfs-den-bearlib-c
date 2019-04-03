#pragma once

#include "lauxlib.h"
#include "lua.h"
#include "lualib.h"

lua_State* load_lua_file(const char* filename);
const char* table_get_string(lua_State* L, const char* index);
int table_get_int(lua_State* L, const char* index);
void dump_stack(lua_State* L);