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

void Lua_Utl::getDebug(lua_State* L, lua_Debug* debug)
{
   lua_getstack(L, 1, debug);
   lua_getinfo(L, "Snl", debug);
}

/*
void Lua_Utl::stackDump(lua_State* L, log_t* log)
{
   lua_Debug debug;
   getDebug(L, &debug);
   const char* func = debug.name;
   
   if (!func)
      func = "(top level)";
   
   m_logLuaStackDump(log, debug.source, debug.currentline, func, L);
}

void Lua_Utl::tableDump(lua_State* L, log_t* log, int idx)
{
   lua_Debug debug;
   getDebug(L, &debug);
   const char* func = debug.name;
   
   if (!func)
      func = "(top level)";
   
   m_logLuaTableDump(log, debug.source, debug.currentline, func, L, idx);
}
*/

#if 0
char logLevelStr[8];
   int i, type, top;
   logTime_t tv;

   top = lua_gettop(lua);
   strncpy(logLevelStr, logLevelInfoStr, 8);
   logLevelStr[7] = 0;
   
   tv = m_timeNow();
   tv = m_timeSub(tv, handle->timeAtStart);

   if (LOG_PTR_IS_FLAG(LOG_FLAG_SRC_INFO))
   {
      fprintf(handle->logFile, "[%5ld.%.06ld] <%s:%d> \"%s\" %s: Lua\n",
              (long int) tv.sec, (long int) tv.usec, file, line,
              func, logLevelStr);
   }
   else
   {
      fprintf(handle->logFile, "[%5ld.%.06ld] \"%s\" %s: Lua\n",
              (long int) tv.sec, (long int) tv.usec, func,
              logLevelStr);
   }

   fprintf(handle->logFile, "---BEGIN_STACK_DUMP---\n");

   for (i = 1; i <= top; i++)
   {
      type = lua_type(lua, i);

      switch (type)
      {
         case LUA_TSTRING:
            fprintf(handle->logFile, "[%4d] \"%s\"\n", i,
                    lua_tostring(lua, i));
            break;

         case LUA_TBOOLEAN:
            fprintf(handle->logFile, "[%4d] %s\n", i,
                    lua_toboolean(lua, i) ? "true" : "false");

         case LUA_TNUMBER:
            fprintf(handle->logFile, "[%4d] %g\n", i, lua_tonumber(lua, i));
            break;

         default:
            fprintf(handle->logFile, "[%4d] type(%s)\n", i,
                    lua_typename(lua, type));
            break;
      }
   }

   fprintf(handle->logFile, "---END_STACK_DUMP---\n");
}

void m_logLuaTableDump(const log_t* handle, const char* file, const int line,
                       const char* func, lua_State* lua, int idx)
{
   char logLevelStr[8];
   int type;
   logTime_t tv;
   
   strncpy(logLevelStr, logLevelInfoStr, 8);
   logLevelStr[7] = 0;
   
   tv = m_timeNow();
   tv = m_timeSub(tv, handle->timeAtStart);
   
   if (LOG_PTR_IS_FLAG(LOG_FLAG_SRC_INFO))
   {
      fprintf(handle->logFile,
              "[%5ld.%.06ld] <%s:%d> \"%s\" %s: Lua table (%d)\n",
              (long int) tv.sec, (long int) tv.usec, file, line,
              func, logLevelStr, idx);
   }
   else
   {
      fprintf(handle->logFile, "[%5ld.%.06ld] \"%s\" %s: Lua table (%d)\n",
              (long int) tv.sec, (long int) tv.usec, func,
              logLevelStr, idx);
   }
   
   fprintf(handle->logFile, "---BEGIN_TABLE_DUMP---\n");
   
   lua_pushnil(lua);
   
   while (lua_next(lua, idx))
   {
      type = lua_type(lua, -2);

      switch (type)
      {
         case LUA_TSTRING:
            fprintf(handle->logFile, "\"%s\" = ", lua_tostring(lua, -2));
            break;

         case LUA_TBOOLEAN:
            fprintf(handle->logFile, "%s = ",
                    lua_toboolean(lua, -2) ? "true" : "false");

         case LUA_TNUMBER:
            fprintf(handle->logFile, "%g = ", lua_tonumber(lua, -2));
            break;

         default:
            fprintf(handle->logFile, "type(%s) = ", lua_typename(lua, type));
            break;
      }
      
      type = lua_type(lua, -1);
      
      switch (type)
      {
         case LUA_TSTRING:
            fprintf(handle->logFile, "\"%s\"\n", lua_tostring(lua, -1));
            break;

         case LUA_TBOOLEAN:
            fprintf(handle->logFile, "%s\n",
                    lua_toboolean(lua, -1) ? "true" : "false");

         case LUA_TNUMBER:
            fprintf(handle->logFile, "%g\n", lua_tonumber(lua, -1));
            break;

         default:
            fprintf(handle->logFile, "type(%s)\n", lua_typename(lua, type));
            break;
      }
      
      lua_pop(lua, 1);
   }

   fprintf(handle->logFile, "---END_TABLE_DUMP---\n");
}
#endif
