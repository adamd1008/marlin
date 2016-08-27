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

#include <Lua_Utl.hpp>
#include <Lua_Log.hpp>

void Lua_Log::initialise(lua_State* L)
{
   /* Set up metatable index */
   
   lua_newtable(L);
   
   lua_pushliteral(L, MARLIN_LUA_LOG_PRINT_CMD);
   lua_pushcfunction(L, Lua_Log::_print);
   lua_settable(L, -3);
   
   lua_pushliteral(L, MARLIN_LUA_LOG_HEXDUMP_CMD);
   lua_pushcfunction(L, Lua_Log::_hexdump);
   lua_settable(L, -3);
   
   lua_pushliteral(L, MARLIN_LUA_LOG_EXIT_CMD);
   lua_pushcfunction(L, Lua_Log::_exit);
   lua_settable(L, -3);
   
   lua_pushliteral(L, MARLIN_LUA_LOG_CBACKTRACE_CMD);
   lua_pushcfunction(L, Lua_Log::_cbacktrace);
   lua_settable(L, -3);
   
   lua_pushliteral(L, MARLIN_LUA_LOG_BACKTRACE_CMD);
   lua_pushcfunction(L, Lua_Log::_backtrace);
   lua_settable(L, -3);
   
   lua_pushliteral(L, MARLIN_LUA_LOG_ABORT_CMD);
   lua_pushcfunction(L, Lua_Log::_abort);
   lua_settable(L, -3);
   
   lua_pushliteral(L, MARLIN_LUA_LOG_ASSERT_CMD);
   lua_pushcfunction(L, Lua_Log::_assert);
   lua_settable(L, -3);
   
   lua_pushliteral(L, MARLIN_LUA_LOG_CLEANUP_CMD);
   lua_pushcfunction(L, Lua_Log::_cleanup);
   lua_settable(L, -3);
   
   lua_setfield(L, LUA_REGISTRYINDEX, MARLIN_LUA_LOG_METATABLE_INDEX);
   
   /* Set up metatable */
   
   lua_newtable(L);
   
   lua_pushliteral(L, "__index");
   lua_getfield(L, LUA_REGISTRYINDEX, MARLIN_LUA_LOG_METATABLE_INDEX);
   lua_settable(L, -3);
   
   lua_pushliteral(L, "__gc");
   lua_pushcfunction(L, Lua_Log::_cleanup);
   lua_settable(L, -3);
   
   lua_setfield(L, LUA_REGISTRYINDEX, MARLIN_LUA_LOG_METATABLE);
   
   /* Static Lua members */
   
   lua_newtable(L);
   
   lua_pushliteral(L, MARLIN_LUA_LOG_INIT_CMD);
   lua_pushcfunction(L, Lua_Log::_init);
   lua_settable(L, -3);
   
   lua_pushliteral(L, "LOG_INFO");
   lua_pushinteger(L, LOG_INFO);
   lua_settable(L, -3);
   
   lua_pushliteral(L, "LOG_WARN");
   lua_pushinteger(L, LOG_WARN);
   lua_settable(L, -3);
   
   lua_pushliteral(L, "LOG_ERR");
   lua_pushinteger(L, LOG_ERR);
   lua_settable(L, -3);
   
   lua_pushliteral(L, "LOG_BUG");
   lua_pushinteger(L, LOG_BUG);
   lua_settable(L, -3);
   
   lua_pushliteral(L, "LOG_L1");
   lua_pushinteger(L, LOG_L1);
   lua_settable(L, -3);
   
   lua_pushliteral(L, "LOG_L2");
   lua_pushinteger(L, LOG_L2);
   lua_settable(L, -3);
   
   lua_pushliteral(L, "LOG_L3");
   lua_pushinteger(L, LOG_L3);
   lua_settable(L, -3);
   
   lua_pushliteral(L, "LOG_ALL");
   lua_pushinteger(L, LOG_ALL);
   lua_settable(L, -3);
   
   lua_pushliteral(L, "LOG_FLAG_NONE");
   lua_pushinteger(L, LOG_FLAG_NONE);
   lua_settable(L, -3);
   
   lua_pushliteral(L, "LOG_FLAG_CLOEXEC");
   lua_pushinteger(L, LOG_FLAG_CLOEXEC);
   lua_settable(L, -3);
   
   lua_pushliteral(L, "LOG_FLAG_SRC_INFO");
   lua_pushinteger(L, LOG_FLAG_SRC_INFO);
   lua_settable(L, -3);
   
   lua_pushliteral(L, "LOG_FLAG_STDOUT");
   lua_pushinteger(L, LOG_FLAG_STDOUT);
   lua_settable(L, -3);
   
   lua_pushliteral(L, "LOG_FLAG_ENABLE_ASSERTS");
   lua_pushinteger(L, LOG_FLAG_ENABLE_ASSERTS);
   lua_settable(L, -3);
   
   lua_pushliteral(L, "LOG_FLAG_ALL");
   lua_pushinteger(L, LOG_FLAG_ALL);
   lua_settable(L, -3);
   
   /* Return the static Lua funcs table */
}

int Lua_Log::_init(lua_State* L)
{
   int argc = lua_gettop(L);
   const char* fileName = nullptr;
   int logLevels = 0;
   int logFlags = 0;
   
   if ((argc > 3) || (argc < 2))
   {
      lua_settop(L, 0);
      luaL_error(L, "Usage: %s([fileName], logLevels, logFlags)",
                 MARLIN_LUA_LOG_INIT_CMD);
   }
   
   if (argc == 3)
   {
      fileName = luaL_checkstring(L, 1);
      logLevels = luaL_checkint(L, 2);
      logFlags = luaL_checkint(L, 3);
   }
   else
   {
      logLevels = luaL_checkint(L, 1);
      logFlags = luaL_checkint(L, 2);
   }
   
   Lua_Log* ret = static_cast<Lua_Log*>(
           lua_newuserdata(L, sizeof(Lua_Log)));
   
   lua_Debug debug;
   Lua_Utl::getDebug(L, &debug);
   const char* func = debug.name;
   
   if (!func)
      func = "(top level)";
   
   ret->_log = m_logInit(fileName, debug.source, debug.currentline, func,
                         logLevels, logFlags);
   ret->_free = true;
   
   lua_remove(L, 1);
   
   /* Set metatable */
   
   lua_getfield(L, LUA_REGISTRYINDEX, MARLIN_LUA_LOG_METATABLE);
   lua_setmetatable(L, -2);
   
   return 1;
}

int Lua_Log::_print(lua_State* L)
{
   int argc = lua_gettop(L);
   int logLevel = LOG_INFO;
   const char* str;
   
   if ((argc > 3) || (argc < 2))
   {
      lua_settop(L, 0);
      luaL_error(L, "Usage: log:%s([logLevel = LOG_INFO], text)",
                 MARLIN_LUA_LOG_PRINT_CMD);
   }
   
   Lua_Log* log = static_cast<Lua_Log*>(
         luaL_checkudata(L, 1, MARLIN_LUA_LOG_METATABLE));
   
   if (argc == 3)
   {
      logLevel = luaL_checkint(L, 2);
      str = luaL_checkstring(L, 3);
   }
   else
      str = luaL_checkstring(L, 2);
   
   lua_Debug debug;
   Lua_Utl::getDebug(L, &debug);
   const char* func = debug.name;
   
   if (!func)
      func = "(top level)";
   
   m_logPrint(log->_log, debug.source, debug.currentline, func, logLevel, str);
   
   lua_settop(L, 0);
   return 0;
}

int Lua_Log::_hexdump(lua_State* L)
{
   int argc = lua_gettop(L);
   int logLevel = LOG_INFO;
   const char* str;
   size_t len;
   
   if ((argc > 3) || (argc < 2))
   {
      lua_settop(L, 0);
      luaL_error(L, "Usage: log:%s([logLevel], string)",
                 MARLIN_LUA_LOG_HEXDUMP_CMD);
   }
   
   Lua_Log* log = static_cast<Lua_Log*>(
         luaL_checkudata(L, 1, MARLIN_LUA_LOG_METATABLE));
   
   if (argc == 3)
   {
      logLevel = luaL_checkint(L, 2);
      str = lua_tolstring(L, 3, &len);
   }
   else
      str = lua_tolstring(L, 2, &len);
   
   lua_Debug debug;
   Lua_Utl::getDebug(L, &debug);
   const char* func = debug.name;
   
   if (!func)
      func = "(top level)";
   
   m_logHexdump(log->_log, debug.source, debug.currentline, func, logLevel,
                str, len);
   
   lua_settop(L, 0);
   return 0;
}

int Lua_Log::_exit(lua_State* L)
{
   int argc = lua_gettop(L);
   int exitCode = 0;
   
   if ((argc > 2) || (argc < 1))
   {
      lua_settop(L, 0);
      luaL_error(L, "Usage: log:%s([exitCode = 0])", MARLIN_LUA_LOG_EXIT_CMD);
   }
   
   Lua_Log* log = static_cast<Lua_Log*>(
         luaL_checkudata(L, 1, MARLIN_LUA_LOG_METATABLE));
   
   if (argc == 2)
      exitCode = luaL_checkint(L, 2);
   
   lua_Debug debug;
   Lua_Utl::getDebug(L, &debug);
   const char* func = debug.name;
   
   if (!func)
      func = "(top level)";
   
   m_logExit(log->_log, debug.source, debug.currentline, func, exitCode);
   
   lua_settop(L, 0);
   return 0;
}

int Lua_Log::_cbacktrace(lua_State* L)
{
   int argc = lua_gettop(L);
   
   if (argc != 1)
   {
      lua_settop(L, 0);
      luaL_error(L, "Usage: log:%s()", MARLIN_LUA_LOG_CBACKTRACE_CMD);
   }
   
   Lua_Log* log = static_cast<Lua_Log*>(
         luaL_checkudata(L, 1, MARLIN_LUA_LOG_METATABLE));
   
   lua_Debug debug;
   Lua_Utl::getDebug(L, &debug);
   const char* func = debug.name;
   
   if (!func)
      func = "(top level)";
   
   m_logBacktrace(log->_log, debug.source, debug.currentline, func);
   
   lua_settop(L, 0);
   return 0;
}

int Lua_Log::_backtrace(lua_State* L)
{
   int argc = lua_gettop(L);
   
   if (argc != 1)
   {
      lua_settop(L, 0);
      luaL_error(L, "Usage: log:%s()", MARLIN_LUA_LOG_BACKTRACE_CMD);
   }
   
   Lua_Log* log = static_cast<Lua_Log*>(
         luaL_checkudata(L, 1, MARLIN_LUA_LOG_METATABLE));
   
   lua_Debug debug;
   Lua_Utl::getDebug(L, &debug);
   const char* func = debug.name;
   
   if (!func)
      func = "(top level)";
   
   if ((luaL_dostring(L, "return debug.traceback(nil, 2)")) != LUA_OK)
   {
      m_logPrint(log->_log, debug.source, debug.currentline, func, LOG_ERR,
                 "luaL_dostring: %s", lua_tostring(L, -1));
      m_logAbort(log->_log, debug.source, debug.currentline, func);
   }
   
   const char* backtrace = luaL_checkstring(L, -1);
   
   m_logPrint(log->_log, debug.source, debug.currentline, func, LOG_INFO,
              "Lua %s", backtrace);
   
   lua_settop(L, 0);
   return 0;
}

int Lua_Log::_abort(lua_State* L)
{
   int argc = lua_gettop(L);
   
   if (argc != 1)
   {
      lua_settop(L, 0);
      luaL_error(L, "Usage: log:%s()", MARLIN_LUA_LOG_ABORT_CMD);
   }
   
   Lua_Log* log = static_cast<Lua_Log*>(
         luaL_checkudata(L, 1, MARLIN_LUA_LOG_METATABLE));
   
   lua_Debug debug;
   Lua_Utl::getDebug(L, &debug);
   const char* func = debug.name;
   
   if (!func)
      func = "(top level)";
   
   m_logAbort(log->_log, debug.source, debug.currentline, func);
   
   lua_settop(L, 0);
   return 0;
}

int Lua_Log::_assert(lua_State* L)
{
   int argc = lua_gettop(L);
   
   if (argc != 2)
   {
      lua_settop(L, 0);
      luaL_error(L, "Usage: log:%s(assertion)", MARLIN_LUA_LOG_ASSERT_CMD);
   }
   
   Lua_Log* log = static_cast<Lua_Log*>(
         luaL_checkudata(L, 1, MARLIN_LUA_LOG_METATABLE));
   int assertion = lua_toboolean(L, 2);
   const char* assertionStr = lua_tostring(L, 2);
   
   lua_Debug debug;
   Lua_Utl::getDebug(L, &debug);
   const char* func = debug.name;
   
   if (!func)
      func = "(top level)";
   
   if (!assertion)
      __logAssert(log->_log, debug.source, debug.currentline, func,
                  assertionStr);
   
   lua_settop(L, 0);
   return 0;
}

int Lua_Log::_cleanup(lua_State* L)
{
   int argc = lua_gettop(L);
   
   if (argc != 1)
   {
      lua_settop(L, 0);
      luaL_error(L, "Usage: log:%s()", MARLIN_LUA_LOG_CLEANUP_CMD);
   }
   
   Lua_Log* log = static_cast<Lua_Log*>(
         luaL_checkudata(L, 1, MARLIN_LUA_LOG_METATABLE));
   
   if (log->_free)
   {
      lua_Debug debug;
      Lua_Utl::getDebug(L, &debug);
      const char* func = debug.name;
      
      if (!func)
         func = "(top level)";
      
      m_logCleanup(log->_log, debug.source, debug.currentline, func);
      
      log->_log = nullptr;
      log->_free = false;
   }
   
   lua_settop(L, 0);
   return 0;
}

int luaopen_libmarlinlua(lua_State* L)
{
   Lua_Log::initialise(L);
   
   return 1;
}
