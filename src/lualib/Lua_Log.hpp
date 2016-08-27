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

#ifndef LUA_LOG_HPP
#define LUA_LOG_HPP

#include <Lua_MarlinBase.hpp>
#include <log.h>

/* Static Lua functions */
#define MARLIN_LUA_LOG_INIT_CMD              "init"

/* Instance Lua functions */
#define MARLIN_LUA_LOG_PRINT_CMD             "print"
#define MARLIN_LUA_LOG_HEXDUMP_CMD           "hexdump"
#define MARLIN_LUA_LOG_EXIT_CMD              "exit"
#define MARLIN_LUA_LOG_CBACKTRACE_CMD        "cbacktrace"
#define MARLIN_LUA_LOG_BACKTRACE_CMD         "backtrace"
#define MARLIN_LUA_LOG_ABORT_CMD             "abort"
#define MARLIN_LUA_LOG_ASSERT_CMD            "assert"
#define MARLIN_LUA_LOG_CLEANUP_CMD           "cleanup"

#define MARLIN_LUA_LOG_METATABLE             "_marlinlogmt"
#define MARLIN_LUA_LOG_METATABLE_INDEX       "_marlinlogmt__index"

class Lua_Log
{
public:
   static void initialise(lua_State* L);
   
private:
   Lua_Log() = delete;
   virtual ~Lua_Log() = delete;
   
   /* Static Lua functions */
   static int _init(lua_State* L);
   
   /* Instance Lua functions */
   static int _print(lua_State* L);
   static int _hexdump(lua_State* L);
   static int _exit(lua_State* L);
   static int _cbacktrace(lua_State* L);
   static int _backtrace(lua_State* L);
   static int _abort(lua_State* L);
   static int _assert(lua_State* L);
   static int _cleanup(lua_State* L);
   
   log_t* _log;
   bool _free;
};

extern "C" int luaopen_libmarlinlua(lua_State* L);

#endif /* LUA_LOG_HPP */

