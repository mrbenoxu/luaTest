#include "lauxlib.h"
#include "ldo.h"

void luaL_pushinteger(struct lua_State* L, int integer) {
    lua_pushinteger(L, integer);
}

void luaL_pushnumber(struct lua_State* L, float number) {
    lua_pushnumber(L, number);
}

void luaL_pushlightuserdata(struct lua_State* L, void* userdata) {
    lua_pushlightuserdata(L, userdata);
}

void luaL_pushnil(struct lua_State* L) {
    lua_pushnil(L);
}

void luaL_pushcfunction(struct lua_State* L, lua_CFunction f) {
    lua_pushcfunction(L, f);
}

void luaL_pushstring(struct lua_State* L, const char* str) {
    lua_pushstring(L, str);
}

void luaL_pushboolean(struct lua_State* L, bool boolean) {
    lua_pushboolean(L, boolean);
}

static void* l_alloc(void* ud, void* ptr, size_t osize, size_t nsize) {
    (void)ud;
    (void)osize;

    // printf("l_alloc nsize:%ld\n", nsize);
    if (nsize == 0) {
        free(ptr);
        return NULL;
    }

    return realloc(ptr, nsize);
}

struct lua_State* luaL_newstate() {
    struct lua_State* L = lua_newstate(&l_alloc, NULL);
    return L;
}

// function call
typedef struct CallS {
    StkId func;
    int nresult;
} CallS;

static int f_call(lua_State* L, void* ud) {
    CallS* c = cast(CallS*, ud);
    luaD_call(L, c->func, c->nresult);
    return LUA_OK;
}

int luaL_pcall(struct lua_State* L, int narg, int nresult) {
    int status = LUA_OK;
    CallS c;
    c.func = L->top - (narg + 1);
    c.nresult = nresult; 

    status = luaD_pcall(L, &f_call, &c, savestack(L, L->top), 0);
    return status;
}

void luaL_close(struct lua_State* L) {
    lua_close(L);
}

bool luaL_checkinteger(struct lua_State* L, int idx) {
    int isnum = 0;
    lua_tointegerx(L, idx, &isnum);
    if (isnum) {
        return true;
    }
    else {
        return false;
    }
}

lua_Integer luaL_tointeger(struct lua_State* L, int idx) {
    int isnum = 0;
    lua_Integer ret = lua_tointegerx(L, idx, &isnum);
    return ret;
}

lua_Number luaL_tonumber(struct lua_State* L, int idx) {
    int isnum = 0;
    lua_Number ret = lua_tonumberx(L, idx, &isnum);
    return ret;
}

void* luaL_touserdata(struct lua_State* L, int idx) {
    return NULL;
}

bool luaL_toboolean(struct lua_State* L, int idx) {
    return lua_toboolean(L, idx);
}

int luaL_isnil(struct lua_State* L, int idx) {
    return lua_isnil(L, idx);
}

void luaL_pop(struct lua_State* L) {
    lua_pop(L); 
}

int luaL_stacksize(struct lua_State* L) {
    return lua_gettop(L);
}
