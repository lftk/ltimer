#include "timer.h"

#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>

#include <stdlib.h>

static timer_t *T = NULL;

static int
linit(lua_State *L)
{
    (void)L;
    
    if (T == NULL) {
        T= timer_create();
    }

    return 0;
}

static int
luninit(lua_State *L)
{
    (void)L;

    if (T) {
        timer_destroy(T);
        T = NULL;
    }

    return 0;
}

static int
ladd(lua_State *L)
{
    unsigned long id, elapse;

    id = lua_tointeger(L, -2);
    elapse = lua_tointeger(L, -1);
    timer_add(T, id, elapse);
    lua_settop(L, -2);

    return 0;
}

static int
ldelete(lua_State *L)
{
    unsigned long id;

    id = lua_tointeger(L, -1);
    timer_delete(T, id);
    lua_settop(L, -1);

    return 0;
}

static int
ldeleteall(lua_State *L)
{
    (void)L;
    
    timer_delete_all(T);

    return 0;
}

static void 
timer_execute(unsigned long id, void *userp)
{
    lua_State *L;

    L = (lua_State *)userp;
    lua_pushvalue(L, -1);
    lua_pushinteger(L, id);
    lua_pcall(L, 1, 0, 0);
}

static int
lexpire(lua_State *L)
{
    timer_expire(T, timer_execute, L);
    lua_settop(L, -1);

    return 0;
}

int
luaopen_ltimer(lua_State *L)
{
    luaL_Reg l[] = {
        {"init", linit},
        {"uninit", luninit},
        {"add", ladd},
        {"delete", ldelete},
        {"deleteall", ldeleteall},
        {"expire", lexpire},
        {NULL, NULL},
    };
    luaL_newlib(L, l);

    return 1;
}
