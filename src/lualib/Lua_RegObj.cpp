/*
   Copyright (c) 2016 Adam Dodd

   Permission is hereby granted, free of charge, to any person obtaining a copy
   of this software and associated documentation files (the "Software"), to deal
   in the Software without restriction, including without limitation the rights
   to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
   copies of the Software, and to permit persons to whom the Software is
   furnished to do so, subject to the following conditions:

   The above copyright notice and this permission notice shall be included in
   all copies or substantial portions of the Software.

   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
   IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
   AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
   OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
   SOFTWARE.
*/

#include <Lua_RegObj.hpp>
#include <stdlib.h>

Lua_RegObj::Lua_RegObj()
{
   
}

Lua_RegObj::~Lua_RegObj()
{
   lua_pushlightuserdata(_L, static_cast<void*>(this));
   lua_pushnil(_L);
   lua_settable(_L, LUA_REGISTRYINDEX);
}

void Lua_RegObj::_SetLuaState(lua_State* L)
{
   _L = L;
}

void Lua_RegObj::_Reset()
{
   if (!_L)
   {
      abort();
   }
   
   /* Push registry table */
   
   lua_pushlightuserdata(_L, static_cast<void*>(this));
   lua_gettable(_L, LUA_REGISTRYINDEX);
   
   if (lua_type(_L, -1) == LUA_TTABLE)
   {
      /* Let's empty the table */
      
      lua_pushnil(_L);
      
      while (lua_next(_L, -2))
      {
         /* `key' and `value' are now on the stack; pop `value' and set the
          * key to nil */
         lua_pop(_L, 1);
         lua_pushnil(_L);
         lua_settable(_L, -3);
         
         /* Push nil again to get the next first key */
         lua_pushnil(_L);
      }
   }
   else if (lua_type(_L, -1) == LUA_TNIL)
   {
      lua_pushlightuserdata(_L, static_cast<void*>(this));
      lua_newtable(_L);
      lua_settable(_L, LUA_REGISTRYINDEX);
   }
   else
   {
      abort();
   }
   
   /* Pop registry table */
   lua_pop(_L, 1);
}

void Lua_RegObj::_Get()
{
   /* Get registry table */
   lua_pushlightuserdata(_L, static_cast<void*>(this));
   lua_gettable(_L, LUA_REGISTRYINDEX);

   /* Place registry table behind key */
   lua_insert(_L, -2);

   /* Get value */
   lua_gettable(_L, -2);

   /* Remove registry table from stack */
   lua_remove(_L, -2);
}

void Lua_RegObj::_Set()
{
   /* Get registry table */
   lua_pushlightuserdata(_L, static_cast<void*>(this));
   lua_gettable(_L, LUA_REGISTRYINDEX);

   /* Place registry table behind key and value */
   lua_insert(_L, -3);

   /* Insert value into registry table */
   lua_settable(_L, -3);

   /* Remove registry table from stack */
   lua_remove(_L, -1);
}
