#pragma once

#include "lualib.h"
#include "lauxlib.h"
#include "lua.h"

lua_State *load_lua_file(const char *filename);