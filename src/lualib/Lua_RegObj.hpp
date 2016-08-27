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

#ifndef LUA_REGOBJ_HPP
#define LUA_REGOBJ_HPP

#include <Lua_MarlinBase.hpp>

class Lua_RegObj
{
public:
   Lua_RegObj();
   virtual ~Lua_RegObj();

   static const char* className()
   {
      return "Lua_RegObj";
   }

protected:
   void _SetLuaState(lua_State*);

   /* Set registry table to be empty */
   void _Reset();

   /* Get an item from this object's registry table. Key must be at the top of
    * the stack (-1). */
   void _Get();

   /* Add an item to this object's registry table. Key must be -2, value must
    * be -1 on stack. */
   void _Set();

   lua_State* _L;

private:
   /* Registry objects must not be copyable */
   Lua_RegObj(const Lua_RegObj&) = delete;
};

#endif /* LUA_REGOBJ_HPP */

