
 /*
 ###############################################################
 #
 #                 This code is part of LUA_EOS
 #
 # Copyrights 2021 - Varanda Labs Inc.
 #
 # License:
 #   Creative Commons: CC BY-NC-SA 4.0
 #   Attribution-NonCommercial-ShareAlike 4.0 International
 #   https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode
 #
 #   Note: for purchasing a commercial license contact:
 #     m@varanda.ca
 #
 ###############################################################
*/

#include "log.h"
#include "lua_eos.h"
#include "lvgl.h"
#include "lauxlib.h"
#include "lv_btn.h"

#ifdef __cplusplus
extern "C" {
#endif


#define UNUSED(x) (void)(x)
// ***********************************************
// * lv_obj_t * lv_btn_create(lv_obj_t * par, const lv_obj_t * copy);
// ***********************************************

static int bind_lv_btn_create(lua_State *L)
{
  void * par = lua_touserdata(L,1);
  void * copy = lua_touserdata(L,2);

  lv_obj_t * ret = lv_btn_create(par, copy);

  if (! ret) {
    lua_pushnil(L);
  }
  else {
    lua_pushlightuserdata(L, ret);
  }
  return 1;
}


// ***********************************************
// * void lv_btn_set_checkable(lv_obj_t * btn, bool tgl);
// ***********************************************

static int bind_lv_btn_set_checkable(lua_State *L)
{
  void * btn = lua_touserdata(L,1);
  int tgl = lua_toboolean(L,2);

  lv_btn_set_checkable(btn, tgl);

  return 0;
}


// ***********************************************
// * void lv_btn_set_state(lv_obj_t * btn, lv_btn_state_t state);
// ***********************************************

static int bind_lv_btn_set_state(lua_State *L)
{
  void * btn = lua_touserdata(L,1);
  int state = lua_tointeger(L,2);

  lv_btn_set_state(btn, state);

  return 0;
}


// ***********************************************
// * void lv_btn_toggle(lv_obj_t * btn);
// ***********************************************

static int bind_lv_btn_toggle(lua_State *L)
{
  void * btn = lua_touserdata(L,1);

  lv_btn_toggle(btn);

  return 0;
}


// ***********************************************
// * static inline void lv_btn_set_layout(lv_obj_t * btn, lv_layout_t layout){    lv_cont_set_layout(btn, layout);
// ***********************************************

static int bind_lv_btn_set_layout(lua_State *L)
{
  void * btn = lua_touserdata(L,1);
  int lv_cont_set_layout = lua_tointeger(L,2);

  lv_btn_set_layout(btn, lv_cont_set_layout);

  return 0;
}


// ***********************************************
// * static inline void lv_btn_set_fit4(lv_obj_t * btn, lv_fit_t left, lv_fit_t right, lv_fit_t top, lv_fit_t bottom){    lv_cont_set_fit4(btn, left, right, top, bottom);
// ***********************************************

static int bind_lv_btn_set_fit4(lua_State *L)
{
  void * btn = lua_touserdata(L,1);
  int left = lua_tointeger(L,2);
  int right = lua_tointeger(L,3);
  int top = lua_tointeger(L,4);
  int lv_cont_set_fit4 = lua_tointeger(L,5);

  lv_btn_set_fit4(btn, left, right, top, lv_cont_set_fit4);

  return 0;
}


// ***********************************************
// * static inline void lv_btn_set_fit2(lv_obj_t * btn, lv_fit_t hor, lv_fit_t ver){    lv_cont_set_fit2(btn, hor, ver);
// ***********************************************

static int bind_lv_btn_set_fit2(lua_State *L)
{
  void * btn = lua_touserdata(L,1);
  int hor = lua_tointeger(L,2);
  int lv_cont_set_fit2 = lua_tointeger(L,3);

  lv_btn_set_fit2(btn, hor, lv_cont_set_fit2);

  return 0;
}


// ***********************************************
// * static inline void lv_btn_set_fit(lv_obj_t * btn, lv_fit_t fit){    lv_cont_set_fit(btn, fit);
// ***********************************************

static int bind_lv_btn_set_fit(lua_State *L)
{
  void * btn = lua_touserdata(L,1);
  int lv_cont_set_fit = lua_tointeger(L,2);

  lv_btn_set_fit(btn, lv_cont_set_fit);

  return 0;
}


// ***********************************************
// * lv_btn_state_t lv_btn_get_state(const lv_obj_t * btn);
// ***********************************************

static int bind_lv_btn_get_state(lua_State *L)
{
  void * btn = lua_touserdata(L,1);

  lv_btn_state_t ret = lv_btn_get_state(btn);

  lua_pushinteger(L, ret);
  return 1;
}


// ***********************************************
// * bool lv_btn_get_checkable(const lv_obj_t * btn);
// ***********************************************

static int bind_lv_btn_get_checkable(lua_State *L)
{
  void * btn = lua_touserdata(L,1);

  bool ret = lv_btn_get_checkable(btn);

  lua_pushboolean(L, ret);
  return 1;
}


// ***********************************************
// * static inline lv_layout_t lv_btn_get_layout(const lv_obj_t * btn){    return lv_cont_get_layout(btn);
// ***********************************************

static int bind_lv_btn_get_layout(lua_State *L)
{
  void * lv_cont_get_layout = lua_touserdata(L,1);

  lv_layout_t ret = lv_btn_get_layout(lv_cont_get_layout);

  lua_pushinteger(L, ret);
  return 1;
}


// ***********************************************
// * static inline lv_fit_t lv_btn_get_fit_left(const lv_obj_t * btn){    return lv_cont_get_fit_left(btn);
// ***********************************************

static int bind_lv_btn_get_fit_left(lua_State *L)
{
  void * lv_cont_get_fit_left = lua_touserdata(L,1);

  lv_fit_t ret = lv_btn_get_fit_left(lv_cont_get_fit_left);

  lua_pushinteger(L, ret);
  return 1;
}


// ***********************************************
// * static inline lv_fit_t lv_btn_get_fit_right(const lv_obj_t * btn){    return lv_cont_get_fit_right(btn);
// ***********************************************

static int bind_lv_btn_get_fit_right(lua_State *L)
{
  void * lv_cont_get_fit_right = lua_touserdata(L,1);

  lv_fit_t ret = lv_btn_get_fit_right(lv_cont_get_fit_right);

  lua_pushinteger(L, ret);
  return 1;
}


// ***********************************************
// * static inline lv_fit_t lv_btn_get_fit_top(const lv_obj_t * btn){    return lv_cont_get_fit_top(btn);
// ***********************************************

static int bind_lv_btn_get_fit_top(lua_State *L)
{
  void * lv_cont_get_fit_top = lua_touserdata(L,1);

  lv_fit_t ret = lv_btn_get_fit_top(lv_cont_get_fit_top);

  lua_pushinteger(L, ret);
  return 1;
}


// ***********************************************
// * static inline lv_fit_t lv_btn_get_fit_bottom(const lv_obj_t * btn){    return lv_cont_get_fit_bottom(btn);
// ***********************************************

static int bind_lv_btn_get_fit_bottom(lua_State *L)
{
  void * lv_cont_get_fit_bottom = lua_touserdata(L,1);

  lv_fit_t ret = lv_btn_get_fit_bottom(lv_cont_get_fit_bottom);

  lua_pushinteger(L, ret);
  return 1;
}



static const luaL_Reg binding_names [] = {
  { "btn_create", bind_lv_btn_create },
  { "btn_set_checkable", bind_lv_btn_set_checkable },
  { "btn_set_state", bind_lv_btn_set_state },
  { "btn_toggle", bind_lv_btn_toggle },
  { "btn_set_layout", bind_lv_btn_set_layout },
  { "btn_set_fit4", bind_lv_btn_set_fit4 },
  { "btn_set_fit2", bind_lv_btn_set_fit2 },
  { "btn_set_fit", bind_lv_btn_set_fit },
  { "btn_get_state", bind_lv_btn_get_state },
  { "btn_get_checkable", bind_lv_btn_get_checkable },
  { "btn_get_layout", bind_lv_btn_get_layout },
  { "btn_get_fit_left", bind_lv_btn_get_fit_left },
  { "btn_get_fit_right", bind_lv_btn_get_fit_right },
  { "btn_get_fit_top", bind_lv_btn_get_fit_top },
  { "btn_get_fit_bottom", bind_lv_btn_get_fit_bottom },

  { NULL, NULL},
};

void lv_append_lib_funcs(lua_State *L, luaL_Reg reg[]);

int bind_lv_btn__init_module(lua_State *L)
{
  lv_append_lib_funcs(L, binding_names);
  return 0;
}


#ifdef __cplusplus
  }
#endif
